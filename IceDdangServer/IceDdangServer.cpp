#include "IceDdangServer.h"


// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

void PlayerItemCollid()
{
	/*
	for (int i = 0; i < clientNum; ++i)
	{
		for (int j = 0; j < idxItem; ++j)
		{
			if (player[i]->x == item[j]->x &&player[i]->y == item[j]->y)
			{
				cout << "Player - Item Coliision!!" << endl;
				player[i]->itemState = 1;
				item[j]->status = 0;
			}
		}
	}
	*/
}
void PlayerPlayerCollid()
{
	for (int i = 0; i < clientNum; ++i)
	{
		for (int j = 0; j < clientNum; ++j)
		{
			if (i == j)
				continue;
			else
			{
				if (player[i]->x == player[j]->x &&player[i]->y == player[j]->y)
				{
					if (player[i]->tagger == true)
					{
						cout << "Player - Player Collision!!" << endl;
						player[j]->status = false;
					}
					else if (player[j]->tagger == true)
					{
						cout << "Player - Player Collision!!" << endl;
						player[i]->status = false;
					}
				}
			}
		}
	}
}

void sendInitPlayer(SOCKET sock, int idx) {
	// ������ ��ſ� ����� ����
	int type = 0;
	int len = sizeof(Player);
	int retval;

	// ������ ������(���� ����) - Ÿ��
	retval = send(sock, (char *)&type, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ������ ������(���� ����) - ����
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ������ ������(���� ����)
	retval = send(sock, (char*)player[idx], sizeof(Player), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}



}

void sendPlayer(SOCKET sock, int idx) {
	// ������ ��ſ� ����� ����
	int type = 0;
	int len = sizeof(Player);
	int retval;

	// ������ ������(���� ����) - Ÿ��
	retval = send(sock, (char *)&type, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ������ ������(���� ����) - ����
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}

	// ������ ������(���� ����)
	retval = send(sock, (char*)player[idx], sizeof(Player), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
}

// Ŭ���̾�Ʈ�� ������ ���
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


	InputData input;

	// Ŭ���̾�Ʈ ���� ���
	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR *)&clientaddr, &addrlen);
	EnterCriticalSection(&PlayerCS);

	if (ThreadSlot[0] == false) {
		ThreadSlot[0] = true;
		printf("[%s:%d] 0�� ������ �Ҵ� �Ϸ�!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		player[0] = new Player(Player(0,5, 9, 1, 1, 0, false));
		sendInitPlayer(client_sock, 0);
		clientNum++;
		LeaveCriticalSection(&PlayerCS);
	}
	else if (ThreadSlot[1] == false) {
		ThreadSlot[1] = true;
		printf("[%s:%d] 1�� ������ �Ҵ� �Ϸ�!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		player[1] = new Player(Player(1,5, 10, 1, 0, 0, true));
		sendInitPlayer(client_sock, 1);
		clientNum++;
		LeaveCriticalSection(&PlayerCS);
	}
	else if (ThreadSlot[2] == false) {
		ThreadSlot[2] = true;
		printf("[%s:%d] 2�� ������ �Ҵ� �Ϸ�!\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		player[2] = new Player(Player(2, 5, 11, 1, 0, 0, true));
		sendInitPlayer(client_sock, 2);
		clientNum++;
		LeaveCriticalSection(&PlayerCS);
	}

	while (1) {
		// ������ �ޱ� - ����� ��������
		retval = recv(client_sock, (char*)&type, sizeof(int), 0);
		retval = recv(client_sock, (char*)&len, sizeof(int), 0);

		cout << "Ÿ��" << type << endl;
		cout << "������" << len << endl;
		if (type == 0)
		{
			retval = recv(client_sock, (char*)PlayerData, len, 0);
			cout << "(x, y): (" << PlayerData->x << ", " << PlayerData->y << ")" << endl;
		}
		if (type == 1)
		{
			retval = recv(client_sock, (char*)&input, len, 0);
			//cout <<"Ű���� ��:" << InputData << endl;
			player[input.idx]->MapCollision(input.input);
			sendPlayer(client_sock, input.idx);
		}

		//retval = recv(client_sock, buf,BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
	else if (retval == 0)
		break;
		// ������ ������
//		retval = send(client_sock, buf, retval, 0);
//		if (retval == SOCKET_ERROR) {
//			err_display("send()");
//			break;
//		}
	}

	// closesocket()
	closesocket(client_sock);
	printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}


int main(int argc, char *argv[])
{
	InitializeCriticalSection(&ItemCS);
	InitializeCriticalSection(&PlayerCS);
	int retval;

	// ���� �ʱ�ȭ
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

	// ������ ��ſ� ����� ����
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

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		// ������ ����
		hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) { closesocket(client_sock); }
		else { CloseHandle(hThread); }
	}

	DeleteCriticalSection(&ItemCS);
	DeleteCriticalSection(&PlayerCS);

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}