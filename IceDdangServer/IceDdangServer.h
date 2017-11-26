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
	int itemstate;		// 소지한 아이템 상태
	bool status;		// 얼음 상태 확인
	bool tagger;		// 술래 여부 확인
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

// 멀티 스레드
bool ThreadSlot[3] = { false, };

// 아이템 동기화 임계영역
CRITICAL_SECTION ItemCS;
// 플레이어 동기화 임계영역
CRITICAL_SECTION PlayerCS;

// 스레드 함수
DWORD WINAPI ClientThread(SOCKET cliect_sock);
DWORD WINAPI CalculThread();

// 송신 함수
void SendPlayerLobbyInfo(int playerNum, char playerID, char playerIP, bool IsReady);
void SendMapInfo(int Stage);
void SendGameData(int x, int y, int speed, bool icestate, int itemstate, bool tagger);
void SendTimeInfo(int time);
void SendResultInfo(int playerNum, bool GameResult);
void SendChangeGameState(int GameState);

// 수신 함수
void RecvPlayerAccessInfo(char playerID, char playerIP);
void RecvInputKey(int inputkey);

using namespace std;
