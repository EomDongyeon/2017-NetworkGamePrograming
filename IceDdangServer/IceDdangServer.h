#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <iostream>

#define SERVERPORT	9000
#define BUFSIZE		512

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

class Player {
public:
	int x = 5;
	int y = 9;
	int speed = 10;
	float r, g, b;
	int itemState;
	bool status = true;
	bool tagger;
	float size = 20;
};

struct Item {
	int PosX;
	int PosY;
	int ItemType;
};

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
