#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <iostream>
#define MAP_SIZE 20
#define SERVERPORT	9000
#define BUFSIZE		512
#define MAX_PLAYER 3

int clientNum = 0;
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


struct PlayerLobby {
	char playerID;
	char playerIP;
	bool IsReady;
	int playerNum;
};
/*
struct Player {
	int PosX;
	int PosY;
	int speed;
	int itemstate;		// ������ ������ ����
	bool status;		// ���� ���� Ȯ��
	bool tagger;		// ���� ���� Ȯ��
};
*/
class InputData {
public:
	int idx;
	char input;
	InputData() { idx = NULL; input = NULL; }
	InputData(int paramIdx, char paramInput) : idx(paramIdx), input(paramInput) { }
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

struct Item {
	int PosX;
	int PosY;
	int ItemType;
};


Player *player[MAX_PLAYER];
Item *item[2];


// ��Ƽ ������
bool ThreadSlot[3] = { false, };

// ������ ����ȭ �Ӱ迵��
CRITICAL_SECTION ItemCS;
// �÷��̾� ����ȭ �Ӱ迵��
CRITICAL_SECTION PlayerCS;

// ������ �Լ�
DWORD WINAPI ClientThread(SOCKET cliect_sock);
DWORD WINAPI CalculThread();



// �۽� �Լ�
void SendPlayerLobbyInfo(int playerNum, char playerID, char playerIP, bool IsReady);
void SendMapInfo(int Stage);
void SendGameData(int x, int y, int speed, bool icestate, int itemstate, bool tagger);
void SendTimeInfo(int time);
void SendResultInfo(int playerNum, bool GameResult);
void SendChangeGameState(int GameState);

// ���� �Լ�
void RecvPlayerAccessInfo(char playerID, char playerIP);
void RecvInputKey(int inputkey);

using namespace std;
