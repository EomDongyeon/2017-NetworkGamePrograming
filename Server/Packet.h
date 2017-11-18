#pragma once

struct PlayerLobby {
	char playerID;
	char playerIP;
	bool IsReady;
	int playerNum;
};

struct Player {
	int PosX;
	int PosY;
	int speed;
	int itemstate;		// 소지한 아이템 상태
	bool status;		// 얼음 상태 확인
	bool tagger;		// 술래 여부 확인
};

struct Item {
	int PosX;
	int PosY;
	int ItemType;
};