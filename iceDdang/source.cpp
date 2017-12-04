#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <gl/glut.h> // includes gl.h glu.h
#include <random>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>

#define WIN_HIEGHT 600
#define WIN_WIDTH 600

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

#define MAP_SIZE 20
#define MAX_PLAYER 3

double elapsed_time;
using namespace std;
int self = 0;
int idx = 0;
bool firstLoop = true;
void err_quit(char *msg);
void err_display(char *msg);

int map[MAP_SIZE][MAP_SIZE] = {
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1 },
	{ 0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1 },
	{ 0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1 },
	{ 0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0 },
	{ 0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1 },
	{ 0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1 },
	{ 0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1 },
	{ 0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1 },
	{ 0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1 },
	{ 0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1 },
	{ 0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1 },
	{ 0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0 },
	{ 0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1 },
	{ 0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1 },
	{ 0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1 },
	{ 0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1 },
	{ 0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1 },
	{ 0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1 },
	{ 0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1 },
};
class Player {
public:
	int idx = 0;
	int x = 5;
	int y = 9;
	int speed = 10;
	float r, g, b;
	int itemState;
	bool status = true;
	bool tagger;
	float size = 20;
	Player() {}
	Player(int paramIdx, int paramX, int paramY, float red, float green, float blue, bool paramTagger) {
		idx = paramIdx;
		x = paramX;
		y = paramY;
		r = red;
		g = green;
		b = blue;
		tagger = paramTagger;
	}


	void draw() {
		float xPos = x * 30;
		float yPos = 600 - (y * 30) - 30;
		if (status == true)
		{
			glColor3f(r, g, b);
			glPushMatrix();
			glBegin(GL_POLYGON);
			glVertex3f(xPos, yPos, 0.0);
			glVertex3f(xPos + size, yPos, 0.0);
			glVertex3f(xPos + size, yPos + size, 0.0);
			glVertex3f(xPos, yPos + size, 0.0);
			glEnd();
			glPopMatrix();
		}
	}
	void setPosition(char c) {
		if (c == 'a')
			x -= 1;
		if (c == 'w')
			y -= 1;
		if (c == 'd')
			x += 1;
		if (c == 's')
			y += 1;
	}
	void MapCollision(char key)
	{
		if (key == 'a')
		{
			if (map[y][x - 1] == 1)
				setPosition('a');
		}
		if (key == 'w')
		{
			if (map[y - 1][x] == 1)
				setPosition('w');
		}
		if (key == 'd')
		{
			if (map[y][x + 1] == 1)
				setPosition('d');
		}
		if (key == 's')
		{
			if (map[y + 1][x] == 1)
				setPosition('s');
		}
	}

};
class Item {
public:
	int x = 15;
	int y = 9;
	int status = 1; // 1,2 아이템 종류 0 아이템 없음
	float size = 20;
	Item(int paramX, int paramY, int paramStatus) {
		x = paramX;
		y = paramY;
		status = paramStatus;
	}
	void draw() {
		if (status > 0)
		{
			float xPos = x * 30;
			float yPos = 600 - (y * 30) - 30;
			glColor3f(1, 1, 1);
			glPushMatrix();
			glBegin(GL_POLYGON);
			glVertex3f(xPos, yPos, 0.0);
			glVertex3f(xPos + size, yPos, 0.0);
			glVertex3f(xPos + size, yPos + size, 0.0);
			glVertex3f(xPos, yPos + size, 0.0);
			glEnd();
			glPopMatrix();
		}
	}

};
class InputData {
public:
	int idx;
	char input;
	InputData(int paramIdx, char paramInput) : idx(paramIdx), input(paramInput) { }
};


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void GameTimer(int value);
void drawMap();
SOCKET sock;
DWORD WINAPI processClient(LPVOID arg);
void drawRect(float x, float y);


class SceneMgr {
public:
	Player *player[MAX_PLAYER];
	Item *item[2];
	int idxPlayer;
	int idxItem;

	void initScene() {
		Player* PlayerData = new Player;
			// 데이터 통신에 사용할 변수
			int type;
			int len;
			int retval;

			// 데이터 받기 - 사용자 프로토콜8
			retval = recv(sock, (char*)&type, sizeof(int), 0);
			retval = recv(sock, (char*)&len, sizeof(int), 0);
			retval = recv(sock, (char*)PlayerData, len, 0);

			self = PlayerData->idx;
			player[self] = PlayerData;

			cout << " [" << player[self]->idx << "] 초기화 " << player[self]->x <<endl;

			if (retval == SOCKET_ERROR) {
				err_display("recv()");
			}

		item[0] = new Item(Item(15, 9, 1));
		item[1] = new Item(Item(6, 11, 1));
		idxItem = 2;
	}
	void drawAllObjects() {
		for (int i = 0; i < MAX_PLAYER; ++i)
		{
			if (player[idx] == NULL)
				continue;
			player[idx]->draw();
		}
		for (int i = 0; i < idxItem; ++i)
			item[i]->draw();
	}
	void PlayerItemCollid()
	{
		for (int i = 0; i < idxPlayer; ++i)
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
	}
	void PlayerPlayerCollid()
	{
		for (int i = 0; i < idxPlayer; ++i)
		{
			for (int j = 0; j < idxPlayer; ++j)
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
	void MovePlayer(int idx, char input)
	{
		//sendPlayerData();
		sendInputData(input);
	}
	void sendInputData(char input) {
		// 데이터 통신에 사용할 변수
		InputData buf(self, input);
		int type = 1;
		int len = sizeof(InputData);
		int retval;

		// 데이터 보내기(고정 길이) - 타입
		retval = send(sock, (char *)&type, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		// 데이터 보내기(고정 길이) - 길이
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		// 데이터 보내기(가변 길이)
		retval = send(sock, (char*)&buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
	}
	void recvPlayerData() {
		Player* PlayerData = new Player;
		// 데이터 통신에 사용할 변수
		int type;
		int len;
		int retval;

		// 데이터 받기 - 사용자 프로토콜8
		retval = recv(sock, (char*)&type, sizeof(int), 0);
		retval = recv(sock, (char*)&len, sizeof(int), 0);
		retval = recv(sock, (char*)PlayerData, len, 0);

		self = PlayerData->idx;
		player[self] = PlayerData;

		cout << " [" << player[self]->idx << "] 초기화 " << player[self]->x << endl;

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
		}
	}
	void sendPlayerData() {
		// 데이터 통신에 사용할 변수
		int type = 0;
		int len = sizeof(Player);
		int retval;

		// 데이터 보내기(고정 길이) - 타입
		retval = send(sock, (char *)&type, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		// 데이터 보내기(고정 길이) - 길이
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}

		// 데이터 보내기(가변 길이)
		retval = send(sock, (char*)player[self], sizeof(Player), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
		}
	}

}*sceneMgr;


void main(int argc, char *argv[])
{
	//초기화 함수들
	CreateThread(NULL, 0, processClient, NULL, 0, NULL);	// 스레드 생성
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(WIN_WIDTH, WIN_HIEGHT); // 윈도우의 크기 지정
	glutInit(&argc, argv);
	glutCreateWindow("얼음땡"); // 윈도우 생성 (윈도우 이름)
	sceneMgr = new SceneMgr;

	if (firstLoop)
	{
		sceneMgr->initScene();
		firstLoop = false;
	}

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutTimerFunc(100, GameTimer, 1);


	glutMainLoop();
}
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	drawMap();
	sceneMgr->drawAllObjects();
	sceneMgr->PlayerItemCollid();
	sceneMgr->PlayerPlayerCollid();


	glFlush(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glOrtho(0, WIN_WIDTH, 0.0, WIN_HIEGHT, -1.0, 1.0);
}

void Keyboard(unsigned char key, int x, int y) {
//	if (key == 'a' || key == 'A')
		//sceneMgr->MovePlayer(0, 'a');

}

void SpecialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT)
	{
		sceneMgr->MovePlayer(1, 'a');
		sceneMgr->recvPlayerData();
		//sceneMgr->sendPlayerData(1);
	}
	if (key == GLUT_KEY_UP)
	{
		sceneMgr->MovePlayer(1, 'w');
		//sceneMgr->MovePlayer(1, 'w');
	}
	if (key == GLUT_KEY_RIGHT)
	{
		sceneMgr->MovePlayer(1, 'd');
		sceneMgr->recvPlayerData();
		//sceneMgr->MovePlayer(1, 'd');
	}
	if (key == GLUT_KEY_DOWN)
	{
		sceneMgr->MovePlayer(1, 's');
		//sceneMgr->MovePlayer(1, 's');
	}
	glutPostRedisplay();
}

void GameTimer(int value) {
	elapsed_time = double(glutGet(GLUT_ELAPSED_TIME));
	glutTimerFunc(100, GameTimer, 1); // 타이머함수 재 설정 

	glutPostRedisplay();   // 화면 재 출력 
}

void drawMap() {
	for (int i = 0; i<MAP_SIZE; i++)
		for (int j = 0; j<MAP_SIZE; j++)
		{
			if (map[i][j] == 0)
				drawRect(j * 30, (600 - i * 30) - 30);
		}
}

void err_quit(char *msg) {
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


DWORD WINAPI processClient(LPVOID arg) {
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");


	return 0;
}

void drawRect(float x, float y) {
	float size = 30;
	glColor3f(0, 0, 1);
	glPushMatrix();
	glBegin(GL_POLYGON);
	glVertex3f(x, y, 0.0);
	glVertex3f(x + size, y, 0.0);
	glVertex3f(x + size, y + size, 0.0);
	glVertex3f(x, y + size, 0.0);
	glEnd();
	glPopMatrix();

};