#include "IceDdangServer.h"


// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;
	int downSize = len;

	while (left > 0) {
		if (left - downSize < 0)
			downSize = left;
		received = recv(s, ptr, len, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}
	return (len - left);
}

// 클라이언트와 데이터 통신
DWORD WINAPI ProcessClient(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	int retval;
	int type = 0;
	int len = 0;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	Player* PlayerData = new Player;

	// 클라이언트 정보 얻기
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	EnterCriticalSection(&PlayerCS);

	if (ThreadSlot[0] == false) {
		ThreadSlot[0] = true;
		printf("[%s:%d] 0번 스레드 할당 완료!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		LeaveCriticalSection(&PlayerCS);
	}
	else if (ThreadSlot[1] == false) {
		ThreadSlot[1] = true;
		printf("[%s:%d] 1번 스레드 할당 완료!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		LeaveCriticalSection(&PlayerCS);
	}
	else if (ThreadSlot[2] == false) {
		ThreadSlot[2] = true;
		printf("[%s:%d] 2번 스레드 할당 완료!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		LeaveCriticalSection(&PlayerCS);
	}

	while (1) {
		// 데이터 받기 - 사용자 프로토콜
		retval = recv(client_sock, (char*)&type, sizeof(int), 0);
		retval = recv(client_sock, (char*)&len, sizeof(int), 0);
		retval = recv(client_sock, (char*)PlayerData, len, 0);
		cout << "타입" << type << endl;
		cout << "사이즈"  << len << endl;
		cout << "(x, y): (" << PlayerData->x<< ", "<< PlayerData->y << ")" << endl;
		//retval = recv(client_sock, buf,BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
	else if (retval == 0)
		break;
		// 데이터 보내기
//		retval = send(client_sock, buf, retval, 0);
//		if (retval == SOCKET_ERROR) {
//			err_display("send()");
//			break;
//		}
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}

int main(int argc, char *argv[])
{
	InitializeCriticalSection(&ItemCS);
	InitializeCriticalSection(&PlayerCS);
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	HANDLE hThread;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	DeleteCriticalSection(&ItemCS);
	DeleteCriticalSection(&PlayerCS);

	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}