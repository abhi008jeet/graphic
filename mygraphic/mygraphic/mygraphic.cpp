// mygraphic.cpp : Defines the entry point for the console application.
//

#define _CRT_SECURE_NO_WARNINGS
#define GLEW_STATIC
#pragma comment (lib, "glew32s.lib")
#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// angle of rotation for the camera direction
float angle = 0.0f, yAngle = 0.0f;

// actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f, ly = 0.0f;

// XZ position of the camera
float x = 0.0f, z = 5.0f;

// the key states. These variables will be zero
//when no key is being presses
float deltaAngle = 0.0f, deltayAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1, yOrigin = 0, lockBall = 0, lockBall1 = 0, lockBall2 = 0, lockP = 0, lockQ = 0, lockR = 0, countP = 0, countR = 0, countQ = 0;
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];

static GLuint texName;

void makeCheckImage(void)
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		checkImage);
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}



void computePos(float deltaMove) {

	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}


/*GLuint LoadTexture(const char * filename)
{
GLuint texture;
int width, height;
unsigned char * data;

FILE * file;
file = fopen(filename, "rb");

if (file == NULL) return 0;
width = 1024;
height = 512;
data = (unsigned char *)malloc(width * height * 3);
//int size = fseek(file,);
fread(data, width * height * 3, 1, file);
fclose(file);

for (int i = 0; i < width * height; ++i)
{
int index = i * 3;
unsigned char B, R;
B = data[index];
R = data[index + 2];

data[index] = R;
data[index + 2] = B;
}

glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
free(data);

return texture;
}*/


void makeRoom() {

	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(40.0f, 25.0f, -20.0f);
	glVertex3f(40.0f, 25.0f, 20.0f);
	glVertex3f(-40.0f, 25.0f, 20.0f);
	glVertex3f(-40.0f, 25.0f, -20.0f);
	glEnd();

	glColor3f(0.5f, 0.5f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(40.0f, 40.0f, -20.0f);
	glVertex3f(40.0f, 40.0f, 20.0f);
	glVertex3f(40.0f, 0.0f, 20.0f);
	glVertex3f(40.0f, 0.0f, -20.0f);
	glEnd();

	glColor3f(0.5f, 0.5f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-40.0f, 40.0f, -20.0f);
	glVertex3f(-40.0f, 40.0f, 20.0f);
	glVertex3f(-40.0f, 15.0f, 20.0f);
	glVertex3f(-40.0f, 15.0f, -20.0f);
	glEnd();

	glColor3f(0.5f, 0.5f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-40.0f, 15.0f, -20.0f);
	glVertex3f(-40.0f, 0.0f, -20.0f);
	glVertex3f(-40.0f, 0.0f, -8.0f);
	glVertex3f(-40.0f, 15.0f, -8.0f);
	glEnd();

	glColor3f(0.5f, 0.5f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-40.0f, 15.0f, 20.0f);
	glVertex3f(-40.0f, 0.0f, 20.0f);
	glVertex3f(-40.0f, 0.0f, 8.0f);
	glVertex3f(-40.0f, 15.0f, 8.0f);
	glEnd();

	glPushMatrix();
	glTranslatef(-40.0f, 0.0f, 0.0f);
	if (countP == 4 && countR == 3 && countQ == 7)
		glRotatef(88.0f, 0.0f, 0.0f, 1.0f);
	glColor3f(0.542f, 0.269f, 0.074f);
	glBegin(GL_QUADS);
	glVertex3f(0.0f, 15.0f, -8.0f);
	glVertex3f(0.0f, 15.0f, 8.0f);
	glVertex3f(0.0f, 0.0f, 8.0f);
	glVertex3f(0.0f, 0.0f, -8.0f);
	glEnd();
	glPopMatrix();

	glColor3f(0.5f, 0.5f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(40.0f, 40.0f, -20.0f);
	glVertex3f(-40.0f, 40.0f, -20.0f);
	glVertex3f(-40.0f, 0.0f, -20.0f);
	glVertex3f(40.0f, 0.0f, -20.0f);
	glEnd();

	glColor3f(0.5f, 0.5f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(40.0f, 40.0f, 20.0f);
	glVertex3f(-40.0f, 40.0f, 20.0f);
	glVertex3f(-40.0f, 0.0f, 20.0f);
	glVertex3f(40.0f, 0.0f, 20.0f);
	glEnd();
}

void boundaryRoom() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 25.0f, -20.0f);
	glVertex3f(40.0f, 24.9f, -20.0f);
	glVertex3f(40.0f, 24.9f, 20.0f);
	glVertex3f(39.9f, 25.0f, 20.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 0.0f, -20.0f);
	glVertex3f(40.0f, 0.1f, -20.0f);
	glVertex3f(40.0f, 0.1f, 20.0f);
	glVertex3f(39.9f, 0.0f, 20.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-39.9f, 25.0f, -20.0f);
	glVertex3f(-40.0f, 24.9f, -20.0f);
	glVertex3f(-40.0f, 25.0f, 20.0f);
	glVertex3f(-39.9f, 24.9f, 20.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-39.9f, 0.0f, -20.0f);
	glVertex3f(-40.0f, 0.1f, -20.0f);
	glVertex3f(-40.0f, 0.1f, 20.0f);
	glVertex3f(-39.9f, 0.0f, 20.0f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(40.0f, 25.0f, -19.9f);
	glVertex3f(40.0f, 24.9f, -20.0f);
	glVertex3f(-40.0f, 24.9f, -20.0f);
	glVertex3f(-40.0f, 25.0f, -19.9f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(40.0f, 0.0f, -19.9f);
	glVertex3f(40.0f, 0.1f, -20.0f);
	glVertex3f(-40.0f, 0.1f, -20.0f);
	glVertex3f(-40.0f, 0.0f, -19.9f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(40.0f, 25.0f, 19.9f);
	glVertex3f(40.0f, 24.9f, 20.0f);
	glVertex3f(-40.0f, 24.9f, 20.0f);
	glVertex3f(-40.0f, 25.0f, 19.9f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(40.0f, 0.0f, 19.9f);
	glVertex3f(40.0f, 0.1f, 20.0f);
	glVertex3f(-40.0f, 0.1f, 20.0f);
	glVertex3f(-40.0f, 0.0f, 19.9f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 40.0f, -20.0f);
	glVertex3f(39.9f, 0.0f, -20.0f);
	glVertex3f(40.0f, 0.0f, -19.9f);
	glVertex3f(40.0f, 40.0f, -19.9f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 40.0f, 20.0f);
	glVertex3f(39.9f, 0.0f, 20.0f);
	glVertex3f(40.0f, 0.0f, 19.9f);
	glVertex3f(40.0f, 40.0f, 19.9f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-39.9f, 40.0f, -20.0f);
	glVertex3f(-39.9f, 0.0f, -20.0f);
	glVertex3f(-40.0f, 0.0f, -19.9f);
	glVertex3f(-40.0f, 40.0f, -19.9f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(-39.9f, 40.0f, 20.0f);
	glVertex3f(-39.9f, 0.0f, 20.0f);
	glVertex3f(-40.0f, 0.0f, 19.9f);
	glVertex3f(-40.0f, 40.0f, 19.9f);
	glEnd();
}

void makeFan() {

	glPushMatrix();
	GLUquadricObj *fanPole;
	fanPole = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 5.0f);
	gluCylinder(fanPole, 0.1f, 0.1f, 1.0f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	GLUquadricObj *fanCone;
	fanCone = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 6.0f);
	gluCylinder(fanCone, 0.1f, 0.6f, 1.0f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	GLUquadricObj *fanDrum;
	fanDrum = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 4.5f);
	gluCylinder(fanDrum, 1.0f, 1.0f, 0.5f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	GLUquadricObj *fanDisc;
	fanDisc = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 4.5f);
	gluDisk(fanDisc, 0.0f, 1.0f, 32, 32);
	glPopMatrix();

	//fanWings
	glPushMatrix();
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 4.75f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(8.0f, 0.0f, 0.4f);
	glVertex3f(8.0f, 0.0f, -0.4f);
	glVertex3f(1.0f, 0.0f, -0.4f);
	glVertex3f(1.0f, 0.0f, 0.4f);
	glEnd();
	glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(8.0f, 0.0f, 0.4f);
	glVertex3f(8.0f, 0.0f, -0.4f);
	glVertex3f(1.0f, 0.0f, -0.4f);
	glVertex3f(1.0f, 0.0f, 0.4f);
	glEnd();
	glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(8.0f, 0.0f, 0.4f);
	glVertex3f(8.0f, 0.0f, -0.4f);
	glVertex3f(1.0f, 0.0f, -0.4f);
	glVertex3f(1.0f, 0.0f, 0.4f);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

//frame
void makeFrame() {

	glPushMatrix();
	glRotatef(0.0f, 0.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 17.0f, -4.0f);
	glVertex3f(39.9f, 17.0f, 4.0f);
	glVertex3f(39.9f, 7.0f, 4.0f);
	glVertex3f(39.9f, 7.0f, -4.0f);
	glEnd();

	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.8f, 17.0f, -4.0f);
	glVertex3f(39.8f, 17.0f, 4.0f);
	glVertex3f(40.0f, 17.0f, 4.0f);
	glVertex3f(40.0f, 17.0f, -4.0f);
	glEnd();

	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.8f, 7.0f, -4.0f);
	glVertex3f(39.8f, 7.0f, 4.0f);
	glVertex3f(40.0f, 7.0f, 4.0f);
	glVertex3f(40.0f, 7.0f, -4.0f);
	glEnd();

	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.8f, 17.0f, 4.0f);
	glVertex3f(39.8f, 7.0f, 4.0f);
	glVertex3f(40.0f, 7.0f, 4.0f);
	glVertex3f(40.0f, 17.0f, 4.0f);
	glEnd();

	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.8f, 17.0f, -4.0f);
	glVertex3f(39.8f, 7.0f, -4.0f);
	glVertex3f(40.0f, 7.0f, -4.0f);
	glVertex3f(40.0f, 17.0f, -4.0f);
	glEnd();

	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 17.0f, 3.9f);
	glVertex3f(39.9f, 7.0f, 3.9f);
	glVertex3f(39.8f, 7.0f, 4.0f);
	glVertex3f(39.8f, 17.0f, 4.0f);
	glEnd();

	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 17.0f, -3.9f);
	glVertex3f(39.9f, 7.0f, -3.9f);
	glVertex3f(39.8f, 7.0f, -4.0f);
	glVertex3f(39.8f, 17.0f, -4.0f);
	glEnd();

	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 16.9f, -4.0f);
	glVertex3f(39.9f, 16.9f, 4.0f);
	glVertex3f(39.8f, 17.0f, 4.0f);
	glVertex3f(39.8f, 17.0f, -4.0f);
	glEnd();

	glColor3f(0.3984f, 0.1998f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(39.9f, 7.1f, -4.0f);
	glVertex3f(39.9f, 7.1f, 4.0f);
	glVertex3f(39.8f, 7.0f, 4.0f);
	glVertex3f(39.8f, 7.0f, -4.0f);
	glEnd();

	glPopMatrix();

}

void makeLever() {
	glPushMatrix();
	glTranslatef(0.0f, 7.1f, -17.0f);
	if (lockBall == 1) {
		glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
	}
	else {
		glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
	}
	glPushMatrix();
	GLUquadricObj * leverBase;
	glColor3f(0.3f, 0.3f, 0.3f);
	leverBase = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	gluCylinder(leverBase, 0.05, 0.05, 1.5f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 0.0f,0.0f);
	GLUquadricObj * leverBall;
	leverBall = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 1.6f);
	glutSolidSphere(0.2f, 64, 64);
	glPopMatrix();

	glPopMatrix();
}
void makeTable() {
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -18.0f);
	glPushMatrix();
	glColor3f(0.3984f, 0.1998f, 0.0f);
	GLUquadricObj *tableBase;
	tableBase = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	gluCylinder(tableBase, 1.2f, 0.7f, 1.0f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3984f, 0.1998f, 0.0f);
	GLUquadricObj *tableSup;
	tableSup = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 6.0f);
	gluCylinder(tableSup, 0.7f, 1.2f, 1.0f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3984f, 0.1998f, 0.0f);
	GLUquadricObj *tablePole;
	tablePole = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	gluCylinder(tablePole, 0.7f, 0.7f, 7.0f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3984f, 0.1998f, 0.0f);
	GLUquadricObj *tableTop;
	tableTop = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 7.1f);
	gluCylinder(tableTop, 4.0f, 4.0f, 0.2f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.3984f, 0.1998f, 0.0f);
	GLUquadricObj *tableTopDisc;
	tableTopDisc = gluNewQuadric();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 7.3f);
	gluDisk(tableTopDisc, 0.0f, 4.0f, 32, 32);
	glPopMatrix();

	glPopMatrix();
}

void makeRad() {

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	GLUquadricObj *radDrum;
	radDrum = gluNewQuadric();
	glTranslatef(0.0f, 10.0f, 0.0f);
	gluCylinder(radDrum, 1.4f, 1.4f, 0.2f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1.0f, 1.0f, 0.0f);
	GLUquadricObj *radFrontY;
	radFrontY = gluNewQuadric();
	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 10.0f, 0.0f);
	gluDisk(radFrontY, 0.0f, 1.4f, 32, 32);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	GLUquadricObj *radFrontB;
	radFrontB = gluNewQuadric();
	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 10.0f, -0.05f);
	gluDisk(radFrontB, 0.0f, 0.5f, 32, 32);
	glPopMatrix();
	glPopMatrix();
}

void makeRadFan() {
	glPushMatrix();
	glRotatef(0.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.4f, 0.15f, 0.0f);
	glVertex3f(0.4f, -0.15f, 0.0f);
	glVertex3f(1.4f, -0.15f, 0.0f);
	glVertex3f(1.4f, 0.15f, 0.0f);
	glEnd();

	glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.4f, 0.15f, 0.0f);
	glVertex3f(0.4f, -0.15f, 0.0f);
	glVertex3f(1.4f, -0.15f, 0.0f);
	glVertex3f(1.4f, 0.15f, 0.0f);
	glEnd();

	glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	glVertex3f(0.4f, 0.15f, 0.0f);
	glVertex3f(0.4f, -0.15f, 0.0f);
	glVertex3f(1.4f, -0.15f, 0.0f);
	glVertex3f(1.4f, 0.15f, 0.0f);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}
void makeSOS() {	
		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 23.0f, -17.0f);
		glVertex3f(39.9f, 23.0f, -17.0f - 1.5f);
		glVertex3f(39.9f, 22.6f, -17.0f - 1.5f);
		glVertex3f(39.9f, 22.6f, -17.0);
		glEnd();


		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 23.0f, 17.0f);
		glVertex3f(39.9f, 23.0f, 17.0f + 1.5f);
		glVertex3f(39.9f, 22.6f, 17.0f + 1.5f);
		glVertex3f(39.9f, 22.6f, 17.0);
		glEnd();

		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 23.0f, -13.5f);
		glVertex3f(39.9f, 23.0f, -13.5f - 1.5f);
		glVertex3f(39.9f, 22.6f, -13.5f - 1.5f);
		glVertex3f(39.9f, 22.6f, -13.5);
		glEnd();


		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 23.0f, 13.5f);
		glVertex3f(39.9f, 23.0f, 13.5f + 1.5f);
		glVertex3f(39.9f, 22.6f, 13.5f + 1.5f);
		glVertex3f(39.9f, 22.6f, 13.5);
		glEnd();

		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 23.0f, -10.0f);
		glVertex3f(39.9f, 23.0f, -10.0f - 1.5f);
		glVertex3f(39.9f, 22.6f, -10.0f - 1.5f);
		glVertex3f(39.9f, 22.6f, -10.0);
		glEnd();


		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 23.0f, 10.0f);
		glVertex3f(39.9f, 23.0f, 10.0f + 1.5f);
		glVertex3f(39.9f, 22.6f, 10.0f + 1.5f);
		glVertex3f(39.9f, 22.6f, 10.0f);
		glEnd();

		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 20.0f, -2.0f);
		glVertex3f(39.9f, 20.0f, -2.0f - 1.5f);
		glVertex3f(39.9f, 19.6f, -2.0f - 1.5f);
		glVertex3f(39.9f, 19.6f, -2.0f);
		glEnd();


		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 20.0f, 2.0f);
		glVertex3f(39.9f, 20.0f, 2.0f + 1.5f);
		glVertex3f(39.9f, 19.6f, 2.0f + 1.5f);
		glVertex3f(39.9f, 19.6f, 2.0f);
		glEnd();

		glColor3f(0.7f, 0.0f, 0.0f);
		glBegin(GL_QUADS);
		glVertex3f(39.9f, 20.0f, -0.75f);
		glVertex3f(39.9f, 20.0f, -0.75f + 1.5f);
		glVertex3f(39.9f, 19.6f, -0.75f + 1.5f);
		glVertex3f(39.9f, 19.6f, -0.75f);
		glEnd();


}
void renderScene(void) {

	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, 10, z, x + lx, 10 + ly, z + lz, 0, 10, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(100.0f, 0.0f, -100.0f);
	glTexCoord2f(0.0, 1.0); glVertex3f(100.0f, 0.0f, 100.0f);
	glTexCoord2f(1.0, 1.0); glVertex3f(-100.0f, 0.0f, 100.0f);
	glTexCoord2f(1.0, 0.0); glVertex3f(-100.0f, 0.0f, -100.0f);

	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Draw ground
	//glBegin(GL_QUADS);
	//glVertex3f(-100.0f, 0.0f, -100.0f);
	//glVertex3f(-100.0f, 0.0f, 100.0f);
	//glVertex3f(100.0f, 0.0f, 100.0f);
	//glVertex3f(100.0f, 0.0f, -100.0f);
	//glEnd();

	makeRoom();
	boundaryRoom();
	glPushMatrix();
	glTranslatef(-25.0f, 18.0f, 0.0f);
	glRotatef(-67.0, 0.0f, 1.0f, 0.0f);
	makeFan();

	glPushMatrix();
	glTranslatef(0.0f, 18.0f, 0.0f);
	glRotatef(-45.0, 0.0f, 1.0f, 0.0f);
	makeFan();

	glPushMatrix();
	glTranslatef(25.0f, 18.0f, 0.0f);
	glRotatef(-60.0, 0.0f, 1.0f, 0.0f);
	makeFan();
	makeFrame();
	makeSOS();
	makeTable();
	makeLever();
	//making radDrums
	if (lockBall == 1) {
		glPushMatrix();
		glTranslatef(-25.0f, 0.0f, 19.9f);
		makeRad();

		glPushMatrix();
		glTranslatef(0.0f, 0.0f, 19.9f);
		makeRad();

		glPushMatrix();
		glTranslatef(25.0f, 0.0f, 19.9f);
		makeRad();

		glPushMatrix();
		glTranslatef(0.0f, 10.0f, 19.899f);
		//if (lockQ == 1) {
		glRotatef(countQ*15.0f, 0.0f, 0.0f, 1.0f);
		//lockQ = 0;
		//}
		makeRadFan();

		glPushMatrix();
		glTranslatef(-25.0f, 10.0f, 19.899f);
		//if (lockR == 1) {
		glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(countR*15.0f, 0.0f, 0.0f, 1.0f);
		//lockR = 0;
		//}
		makeRadFan();

		glPushMatrix();
		glTranslatef(25.0f, 10.0f, 19.899f);
		//if (lockP == 1) {
		glRotatef(-60.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(-1 * countP*15.0f, 0.0f, 0.0f, 1.0f);
		//lockP = 0;
		//}
		makeRadFan();
		
	}


	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy) {

	if (key == 27)
		exit(0);
	else if (key == 's') {
		if (lockBall1 == 0) {
			lockBall1 = 1;
		}
		else if (lockBall1 == 1 && lockBall2 == 1) {
			lockBall = 1;
		}
	}
	else if (key == 'o') {
		if (lockBall1 == 1) {
			lockBall2 = 1;
		}
	}
	else if (key == 'p') {
		countP++;
		countP = countP % 8;
	}
	else if (key == 'q') {
		countQ++;
		countQ = countQ % 8;
	}
	else if (key == 'r') {
		countR++;
		countR = countR % 8;
	}
	else {
		lockBall1 = 0;
		lockBall2 = 0;
	}
}

void pressKey(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_UP: deltaMove = 0.5f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
	}
}

void releaseKey(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN: deltaMove = 0; break;
	}
}

void mouseMove(int x, int y) {

	// this will only be true when the left button is down
	if (xOrigin >= 0) {

		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;
		deltayAngle = (y - yOrigin) * 0.002f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
		ly = sin(yAngle + deltayAngle);
	}
}

void mouseButton(int button, int state, int x, int y) {


	if (button == GLUT_LEFT_BUTTON) {


		if (state == GLUT_UP) {
			angle += deltaAngle;
			yAngle += deltayAngle;
			xOrigin = -1;
			yOrigin = 0;
		}
		else {
			xOrigin = x;
			yOrigin = y;
		}
	}
}


int main(int argc, char **argv) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1320, 720);
	glutCreateWindow("Escape House");
	init();
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	//functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 1;
}
