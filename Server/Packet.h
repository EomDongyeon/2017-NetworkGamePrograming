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
	int itemstate;		// ������ ������ ����
	bool status;		// ���� ���� Ȯ��
	bool tagger;		// ���� ���� Ȯ��
};

struct Item {
	int PosX;
	int PosY;
	int ItemType;
};