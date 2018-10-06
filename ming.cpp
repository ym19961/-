// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <GL/glew.h> // glew.h Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include<windows.h>
#include<stack>
#include<fstream>
using namespace std;
typedef float Color[3];
void Bresenhanmline(int x0, int y0, int x1,int y1) {
	int x, y, dx, dy;
	glPointSize(1);
	x = x0;
	y = y0;
	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	int s1 = x1>x0 ? 1 : -1;
	int s2 = y1>y0 ? 1 : -1;
	bool interchange = false;
	if (dy>dx) {
		float temp = dx;
		dx = dy;
		dy = temp;
		interchange = true;
	}
	float p = 2 * dy - dx;
	for (int i = 0; i<dx; i++) {
		glVertex2i(x, y);
		if (p >= 0) {
			if (!interchange)
				y += s2;
			else
				x += s1;
			p -= 2 * dx;
		}
		if (!interchange)
			x += s1;
		else
			y += s2;
		p += 2 * dy;
	}
}
struct Point {
	int x;
	int y;
};
bool arrayEquals(Color a1, Color a2) {
	if (a1[0] == a2[0] && a1[1] == a2[1] && a1[2]== a2[2])
		return true;
	else
		return false;
}
void glPoint(int x, int y, Color color) {
	glColor3f(color[0],color[1],color[2]);
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}
void fill1(int x, int y,Color border,Color color) {
	Color pixels;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
	if (!arrayEquals(pixels, border) && !arrayEquals(pixels, color)) {
		glPoint(x, y, color);
		fill1(x-1,y,border,color);
		fill1(x,y+1,border,color);
		fill1(x+1,y,border,color);
		fill1(x,y-1,border,color);
	}
}
void fill2(int xseed, int yseed,Color border,Color color) {
	//237,28,36  red
	stack<Point> PointStack;
	Color pixels;

	while (!PointStack.empty()) {
		PointStack.pop();
	}
	Point point = { xseed,yseed };
	PointStack.push(point);
	int x, y,x1,y1,x2,y2;
	while (!PointStack.empty()) {
		Point tempPoint = PointStack.top();
		PointStack.pop();
		x = tempPoint.x;
		y = tempPoint.y;
		glPoint(x, y, color);
		x1 = x - 1;
		glReadPixels(x1, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
		if (!arrayEquals(pixels, border) && !arrayEquals(pixels, color)) {
			Point p1 = { x1,y };
			PointStack.push(p1);
		}
		y1 = y + 1;
		glReadPixels(x, y1, 1, 1, GL_RGB, GL_FLOAT, &pixels);
		if (!arrayEquals(pixels, border) && !arrayEquals(pixels, color)) {
			Point p2 = { x,y1 };
			PointStack.push(p2);
		}
		x2 = x + 1;
		glReadPixels(x2, y, 1, 1, GL_RGB, GL_FLOAT, &pixels);
		//cout << (int)pixels[0] << endl;
		if (!arrayEquals(pixels, border) && !arrayEquals(pixels, color)) {
			Point p3 = { x2,y };
			PointStack.push(p3);
		}
		y2 = y - 1;
		glReadPixels(x, y2, 1, 1, GL_RGB, GL_FLOAT, &pixels);
		if (!arrayEquals(pixels, border) && !arrayEquals(pixels, color)) {
			Point p4 = { x,y2 };
			PointStack.push(p4);
		}
	}
}
void fill(int xseed, int yseed, Color border, Color color) {
	//237,28,36  red
	stack<Point> PointStack;
	Color pixels;
	Point point = { xseed,yseed };
	PointStack.push(point);
	int savePoint;
	int xL, xR, x, y;
	while (!PointStack.empty()) {
		Point tempPoint = PointStack.top();
		PointStack.pop();
		savePoint = tempPoint.x;
		x = tempPoint.x;
		y = tempPoint.y;
		glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
		while (!arrayEquals(pixels, border)) {
			glPoint(x, y, color);
			++x;
			glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
		}
		xR = x - 1;
		x = savePoint - 1;
		glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
		while (!arrayEquals(pixels, border)) {
			glPoint(x, y, color);
			--x;
			glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
		}
		xL = x + 1;
		x = xR;
		++y;
		while (x >= xL) {
			glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
			if (!arrayEquals(pixels, border) && !arrayEquals(pixels, color)) {
				Point p = { x,y };
				PointStack.push(p);
				break;
			}
			else {
				--x;
				glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
			}
		}
		y = y - 2;
		x = xR;
		while (x >xL) {
			glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
			if (!arrayEquals(pixels, border) && !arrayEquals(pixels, color)) {
				Point p = { x,y };
				PointStack.push(p);
				break;
			}
			else {
				--x;
				glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, pixels);
			}
		}
	}
}
void display()
{
	glShadeModel(GL_FLAT);
	glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glViewport(0, 0, 500, 500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Color border = {0.0,0.0,1.0};
	Color color = { 0.0,1.0,1.0 };
	glColor3f(border[0], border[1], border[2]);
	glPointSize(1);
	Bresenhanmline(150, 100, 150, 190);
	Bresenhanmline(150, 210, 150, 300);
	Bresenhanmline(135, 85, 135, 315);
	Bresenhanmline(250, 100, 250, 190);
	Bresenhanmline(250, 210, 250, 300);
	Bresenhanmline(265, 85, 265, 315);
	Bresenhanmline(150, 300, 250, 300);
	Bresenhanmline(135, 315, 265, 315);
	Bresenhanmline(150, 100, 250, 100);
	Bresenhanmline(135, 85, 265, 85);
	Bresenhanmline(150, 210, 250, 210);
	Bresenhanmline(150, 190, 250, 190);
	//Bresenhanmline(220, 210, 210, 190);
	Bresenhanmline(300, 20, 300, 350);
	Bresenhanmline(315, 20, 315, 155);//内线
	Bresenhanmline(315, 250, 315, 175);
	Bresenhanmline(315, 270, 315, 335);
	Bresenhanmline(430, 20, 430, 350);
	Bresenhanmline(415, 20, 415, 155);
	Bresenhanmline(415, 175, 415, 250);
	Bresenhanmline(415, 270, 415, 335);
	Bresenhanmline(300, 350, 430, 350);
	Bresenhanmline(315, 335, 415, 335);
	Bresenhanmline(300, 20, 315, 20);
	Bresenhanmline(415, 20, 430, 20);
	Bresenhanmline(315, 270, 415, 270);
	Bresenhanmline(315, 250, 415, 250);
	//Bresenhanmline(395, 270, 385, 250);
	Bresenhanmline(315, 175, 415, 175);
	Bresenhanmline(315, 155, 415, 155);
	//Bresenhanmline(385, 175, 375, 155);
	glEnd();
	glFlush();//刷新绘图命令
	fill(264, 314,border,color);
	fill2(429, 349,border,color);
}
void Reshape(int w, int h)
{
	glViewport(0, 0, w, h); //设置窗口值这三个函数调整绘图坐标，使左下角位置为(0,0),右上角位置为(w,h)
	glMatrixMode(GL_PROJECTION); //对投影矩阵应用随后的矩阵操作
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h); //设置坐标系值, 这个要放在Reshape中，否则窗口右上角的坐标一直都是初始的w和h
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("明");
	glutDisplayFunc(display);
	glutReshapeFunc(Reshape); //窗口初次创建是被调用
	glutMainLoop();
	return 0;
}
