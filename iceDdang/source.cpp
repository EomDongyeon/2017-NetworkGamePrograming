#include <iostream>
#include <fstream>
#include <string>
#include <glut.h> // includes gl.h glu.h
#include <random>
#include <time.h>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

#define WIN_HIEGHT 600
#define WIN_WIDTH 600
#define MAP_SIZE 20
#define PLAYER_NUM 2

double elapsed_time;
using namespace std;

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
	Player(int paramX, int paramY, float red, float green, float blue, bool paramTagger) {
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

}*player[2];

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

}*item[2];
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void GameTimer(int value);
void drawMap();
void PlayerItemCollid(Player *p, Item *i);
void PlayerPlayerCollid(Player *p, Player *q);

void main(int argc, char *argv[])
{
	//초기화 함수들

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(WIN_WIDTH, WIN_HIEGHT); // 윈도우의 크기 지정
	glutCreateWindow("얼음땡"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutTimerFunc(100, GameTimer, 1);

	player[0] = new Player(Player(5, 9, 1, 1, 0, false));
	player[1] = new Player(Player(5, 10, 1, 0, 0, true));
	item[0] = new Item(Item(15, 9, 1));
	item[1] = new Item(Item(6, 11, 1));
	glutMainLoop();
}
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	drawMap();

	item[0]->draw();
	item[1]->draw();
	cout << "Item Status" << item[0]->status << endl;
	cout << "Player의 Item Status" << player[0]->itemState << endl;
	for (int i = 0; i < PLAYER_NUM; ++i)
	{
		player[0]->draw();
		player[1]->draw();
	}

	PlayerItemCollid(player[0], item[0]);
	PlayerItemCollid(player[0], item[1]);
	PlayerItemCollid(player[1], item[0]);
	PlayerItemCollid(player[1], item[1]);

	PlayerPlayerCollid(player[0], player[1]);
	
	//cout << player[0]->itemState << endl;

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
	if (key == 'a' || key == 'A')
		player[0]->MapCollision('a');
	//player[0].setPosition('a');
	if (key == 'w' || key == 'W')
		player[0]->MapCollision('w');
	if (key == 'd' || key == 'D')
		player[0]->MapCollision('d');
	if (key == 's' || key == 'S')
		player[0]->MapCollision('s');
	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT)
		player[1]->MapCollision('a');
	if (key == GLUT_KEY_UP)
		player[1]->MapCollision('w');
	if (key == GLUT_KEY_RIGHT)
		player[1]->MapCollision('d');
	if (key == GLUT_KEY_DOWN)
		player[1]->MapCollision('s');
	glutPostRedisplay();
}

void GameTimer(int value) {
	elapsed_time = double(glutGet(GLUT_ELAPSED_TIME));
	//cout << elapsed_time << endl;
	//if (elapsed_time > 6000)
	{
		//	cout << "게임 종료" << endl;
	}
	//else {
	glutTimerFunc(100, GameTimer, 1); // 타이머함수 재 설정 
									  //}
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

void PlayerItemCollid(Player *p, Item *i)
{
	cout << p->x << ", " << p->y << "		Item :" << i->x << ", " << i->y << endl;
	if (p->x == i->x && p->y == i->y)
	{
		cout << "Player - Item Coliision!!" << endl;
		i->status = 0;
		p->itemState = 1;
	}
}

void PlayerPlayerCollid(Player *p, Player *tag)
{
	if (p->x == tag->x && p->y == tag->y)
	{
		cout << "Player - Player Collision!!" << endl;
		p->status = false;
	}
}