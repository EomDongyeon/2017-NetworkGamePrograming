#include <iostream>
#include <fstream>
#include <string>
#include <GL/glut.h> // includes gl.h glu.h
#include <random>
#include <time.h>
#include<stdio.h>
#include<vector>
#include<stdlib.h>
#include<stdio.h>

#define WINHIEGHT 600
#define WINWIDTH 600
#define MAXNUM 20
#define PI 3.141592

using namespace std;

int map[20][20] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
	{0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1},
	{0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1},
	{0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,0},
	{0,1,1,1,0,1,0,1,1,1,1,1,1,1,0,1,0,1,0,1},
	{0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,1},
	{0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1},
	{0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,0,0,0,1},
	{0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1},
	{0,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,1},
	{0,1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1},
	{0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0},
	{0,1,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,1,1,1},
	{0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
	{0,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0,1},
	{0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1},
	{0,1,1,1,0,1,1,1,0,1,1,1,1,1,1,1,0,1,0,1},
	{0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1},
	{0,1,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,1},
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
	float size = 20;
	float angle = 0.0;
	void draw() {
		float xPos = x * 30;
		float yPos = 600 - (y * 30) - 30;
		glColor3f(1, 1, 0);
		glPushMatrix();
		glBegin(GL_POLYGON);
			glVertex3f(xPos, yPos, 0.0);
			glVertex3f(xPos + size, yPos, 0.0);
			glVertex3f(xPos + size, yPos + size, 0.0);
			glVertex3f(xPos, yPos + size, 0.0);
		glEnd();
		glPopMatrix();
	}
	void setPosition(char c) {
		if (c == 'a')
			x -= 30;
		if (c == 'w')
			y += 30;
		if (c == 'd')
			x += 30;
		if (c == 's')
			y -= 30;
	}
	void MapCollision(char key)
	{
		if (key == 'a')
		{
			if(map[y][x-1] == 1)
				x -= 1;
		}
		if (key == 'w')
		{
			if (map[y - 1][x] == 1)
				y -= 1;
		}
		if (key == 'd')
		{
			if (map[y][x+1] == 1)
				x += 1;
		}
		if (key == 's')
		{
			if (map[y + 1][x] == 1)
				y += 1;
		}
	}

}player[2];


GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void drawMap();
void MapCollision(char key);


void main(int argc, char *argv[])
{
	//초기화 함수들

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치지정
	glutInitWindowSize(WINWIDTH, WINHIEGHT); // 윈도우의 크기 지정
	glutCreateWindow("얼음땡"); // 윈도우 생성 (윈도우 이름)
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);


	glutMainLoop();
}
// 윈도우 출력 함수
GLvoid drawScene(GLvoid)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	drawMap();
	player[0].draw();

	glFlush(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glOrtho(0, WINWIDTH, 0.0, WINHIEGHT, -1.0, 1.0);
}

void Keyboard(unsigned char key, int x, int y) {
	if (key == 'a' || key == 'A')
		player[0].MapCollision('a');
		//player[0].setPosition('a');
	if (key == 'w' || key == 'W')
		player[0].MapCollision('w');
	if (key == 'd' || key == 'D')
		player[0].MapCollision('d');
	if (key == 's' || key == 'S')
		player[0].MapCollision('s');
	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y) {
	
	glutPostRedisplay();
}


void drawMap() {
	int i, j;
	for (i = 0; i<21; i++)
		for (j = 0; j<21; j++)
		{
			if (map[i][j] == 0)
				drawRect( j * 30,  (600 - i * 30)-30);
		
		}
}


