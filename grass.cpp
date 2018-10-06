  // ConsoleApplication1.cpp: �������̨Ӧ�ó������ڵ㡣
// Tree_1.cpp: �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <string>
using namespace std;
class Point2
{
public:
	Point2();
	Point2(float, float);
public:
	float x;
	float y;
};
Point2::Point2() :x(0), y(0){}
Point2::Point2(float xx, float yy) : x(xx), y(yy){}
class  Node
{
public:
	Point2 point;
	float direction;
	Node() {}
};

GLfloat PI = 3.14;
string way[4]; //�ṩ�������ɹ��� 
string rule, temprule;

float len; //������
float Alpha;

int n;  ////��������
Point2 pStart(0, -10); //start point
Node   stack[150];
int    stackpointer;


void LSystemRule()
{
	//��ʼ��
	way[0] = "[+F-F][-F+F]";
	way[1] = "FF[+F+F-[F-F++F]]F[-F-F+[F-F++F]]";
	len = 0.5;
	Alpha = 22;
	n = 4;

	stackpointer = 0;
	for (int i = 0; i <150; i++)
	{
		stack[i].point.x = 0;
		stack[i].direction = 0;
	}
	rule = way[0];
	for (int i = 1; i <= n; i++)
	{
		int curlen = temprule.length();
		int pos = 0, j = 0;
		while (j < curlen)
		{
			if (temprule[j] == 'F')
			{
				rule += way[1];
				j++;
				pos = rule.length() - 1;
			}
			else
			{
				rule += temprule[j];
				pos++;
				j++;
			}
		}
		temprule = rule;
		//rule.empty();
		rule.clear();
	}
	rule = temprule;
}

void drawGrass()
{
	Node  Nextnode, Curnode;
	Curnode.point.x = pStart.x;
	Curnode.point.y = pStart.y;
	Curnode.direction = 85;
	int length = rule.length();
	int  i = 0;
	glColor3ub(154, 205, 50);
	//glColor3ub(211, 177, 125);
	//cout << rule << endl;
	glRotatef(180, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 30.0, 0.0);
	glPushMatrix();
	while (i < length)
	{
		switch (rule[i])
		{
		case 'F':
			Nextnode.point.x = Curnode.point.x + len * cos(Curnode.direction * PI / 180);
			Nextnode.point.y = Curnode.point.y - len * sin(Curnode.direction * PI / 180);
			Nextnode.direction = Curnode.direction;
			glLineWidth(2.0f);
			glBegin(GL_LINES);
			glVertex2f(Curnode.point.x, Curnode.point.y);
			glVertex2f(Nextnode.point.x, Nextnode.point.y);
			glEnd();
			Curnode = Nextnode;
			break;
		case '[':
			stack[stackpointer] = Curnode;
			stackpointer++;
			break;
		case ']':
			Curnode = stack[stackpointer - 1];
			stackpointer--;
			break;
		case '+':
			Curnode.direction = Curnode.direction + Alpha;
			break;
		case '-':
			Curnode.direction = Curnode.direction - Alpha;
			break;
		default:
			;
		}
		i++;
	}
	glPopMatrix();
}
void  init()
{
	// Set the clear color to black
	glClearColor(0.0, 0.0, 0.0, 0.0);
	LSystemRule();
}
void  display()
{
	// Clear the color and depth buffers.  
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGrass();

	glFlush();
	glutSwapBuffers();
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	GLfloat f = 20.0f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-f, f, -f * (GLfloat)h / (GLfloat)w,
			f*(GLfloat)h / (GLfloat)w, -f, f);
	else
		glOrtho(-f * (GLfloat)w / (GLfloat)h,
			f*(GLfloat)w / (GLfloat)h, -f, f, -f, f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
int  main(int  argc, char  **argv)
{
	glutInit(&argc, argv);    // Initialize GLUT function callings
							  //sets the initial display mode
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	// Set window position, from the left and top corner of the screen, in numbers of pixels 
	glutInitWindowPosition(200, 100);

	// Set window size (width, height) in number of pixels     
	glutInitWindowSize(600, 600);

	// Specify a window creation event 
	glutCreateWindow("L_System Grass");
	//A window is reshaped
	glutReshapeFunc(reshape);

	// Specify the drawing function that is called when the window 
	glutDisplayFunc(display);         // is created or re-drew

	init();	    // Invoke this function for initialization	

	glutMainLoop();    // Enter the event processing loop

	return 0;          // Indicate normal termination 		
}
