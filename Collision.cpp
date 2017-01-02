#include <math.h>
#include <stdlib.h>
#include "MyHeader.h"
typedef struct boundingBoxStruct boundingBox;
struct boundingBoxStruct {
	GLfloat kernelX;
	GLfloat kernelY;
	GLfloat lengthX;
	GLfloat lengthY;
};

const float PI = 3.1415926535;

float myabs(float x) {
	return x < 0 ? -x : x;
}

float min(float x, float y)
{
	return x < y ? x : y;
}

float max(float x, float y)
{
	return x > y ? x : y;
}

typedef struct node *nodeptr;
typedef nodeptr memForBox;

struct node {
	GLint number;
	boundingBox box;
	GLfloat far;
	memForBox next;
};

struct dot {
	float x;
	float y;
	dot(float _x, float _y) :x(_x), y(_y) { }
};

float Direction(dot pi, dot pj, dot pk) {
	return (pk.x - pi.x)*(pj.y - pi.y) - (pj.x - pi.x)*(pk.y - pi.y);
}

bool OnSegment(dot pi, dot pj, dot pk) {
	if ((min(pi.x, pj.x)-0.1 <= pk.x) && (pk.x <= max(pi.x, pj.x)+0.1)
		&& (min(pi.y, pj.y)-0.1 <= pk.y) && (pk.y <= max(pi.y, pj.y)+0.1))
		return true;
	else return false;
}

bool SegmentIntersect(dot p1, dot p2, dot p3, dot p4)
{
	float d1, d2, d3, d4;
	d1 = Direction(p3, p4, p1);
	d2 = Direction(p3, p4, p2);
	d3 = Direction(p1, p2, p3);
	d4 = Direction(p1, p2, p4);
	printf("d1 = %f\nd2 = %f\nd3 = %f\nd4 = %f\n\n", d1, d2, d3, d4);
	if (((d1>0 && d2<0) || (d1<0 && d2>0)) && ((d3>0 && d4<0) || (d3<0 && d4>0)))
		return true;
	else if (d1==0 && OnSegment(p3, p4, p1))
		return true;
	else if (d2==0 && OnSegment(p3, p4, p2))
		return true;
	else if (d3==0 && OnSegment(p1, p2, p3))
		return true;
	else if (d4==0 && OnSegment(p1, p2, p4))
		return true;
	else return false;
}

memForBox header = NULL;
GLint maxNumber = 1;

GLint generateNewBoundingBox(GLfloat kernelX, GLfloat kernelY, GLfloat lengthX, GLfloat lengthY) {
	memForBox temp,ptr;
	temp = (memForBox)malloc(sizeof(node));
	temp->number = maxNumber++;
	temp->box.kernelX = kernelX;
	temp->box.kernelY = kernelY;
	temp->box.lengthX = lengthX + 2;
	temp->box.lengthY = lengthY + 2;
	temp->far = getDistance(kernelX, kernelY, kernelX + lengthX/2, kernelY + lengthY/2);
	temp->next = NULL;
	if (header == NULL)
		header = temp;
	else {
		ptr = header;
		while (ptr->next != NULL) {
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
	return temp->number;
}

void moveBoundingBox(GLint number, GLfloat moveX, GLfloat moveY) {
	memForBox ptr;
	ptr = header;
	while (ptr != NULL) {
		if (ptr->number == number) {
			ptr->box.kernelX += moveX;
			ptr->box.kernelY += moveY;
			return;
		}
		ptr = ptr->next;
	}
}

void scalerBoundingBox(GLint number, GLfloat scaler) {
	memForBox ptr;
	ptr = header;
	while (ptr != NULL) {
		if (ptr->number == number) {
			ptr->box.lengthX *= scaler;
			ptr->box.lengthY *= scaler;
			ptr->far *= scaler;
			return;
		}
		ptr = ptr->next;
	}
}

float getDistance(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}


void move(float* eye, float* center, direction dir) {
	memForBox ptr = header;
	float offset = 0.8f;
	float deltax = 0;
	float deltay = 0;
	float x0 = eye[0];
	float y0 = eye[2];
	float nx = 0;
	float ny = 0;
	float d = getDistance(eye[0], eye[2], center[0], center[2]);
	switch (dir) {
	case front:
		deltax = (center[0] - eye[0])*offset / d;
		deltay = (center[2] - eye[2])*offset / d;
		break;
	case back:
		deltax = -(center[0] - eye[0])*offset / d;
		deltay = -(center[2] - eye[2])*offset / d;
		break;
	case left:
		deltax = (center[2] - eye[2])*offset / d;
		deltay = -(center[0] - eye[0])*offset / d;
		break;
	case right:
		deltax = -(center[2] - eye[2])*offset / d;
		deltay = (center[0] - eye[0])*offset / d;
		break;
	default:
		break;
	}

	while (ptr != NULL) {
		d = getDistance(eye[0], eye[2], ptr->box.kernelX, ptr->box.kernelY);
		if ((ptr->far+2)*1.2 < d){
			ptr = ptr->next;
			continue;
		}
		nx = x0 + deltax;
		ny = y0 + deltay;
		float kx = ptr->box.kernelX;
		float ky = ptr->box.kernelY;
		float lx = ptr->box.lengthX / 2;
		float ly = ptr->box.lengthY / 2;
		dot dots[4] = { dot(kx + lx,ky + ly),dot(kx - lx,ky + ly),dot(kx - lx,ky - ly),dot(kx + lx,ky - ly) };
		dot dote = dot(x0, y0);
		dot dotn = dot(nx, ny);
		bool signal = false;
		int crack = -1;
		for (int i = 0; i < 4; i++) {
			if (SegmentIntersect(dote, dotn, dots[i], dots[(i + 1) % 4])) {
				signal = !signal;
				crack = i;
			}
		}
		printf("**************************************\n");
		if (signal) {
			if (crack % 2 == 0)
				deltay = 0;
			else
				deltax = 0;
		}
		ptr = ptr->next;
	}

	eye[0] += deltax;
	eye[2] += deltay;
	center[0] += deltax;
	center[2] += deltay;

}

void rotate(float* eye, float* center, direction dir) {
	float x1 = eye[0];
	float y1 = eye[2];
	float x2 = center[0];
	float y2 = center[2];
	float alpha = 0.8f / (10 * PI);
	float cosa = cos(alpha);
	float sina = sin(alpha);
	float tempx = x2 - x1;
	float tempy = y2 - y1;
	float r = sqrtf((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (eye[1] - center[1])*(eye[1] - center[1]));
	float s = asinf((center[1] - eye[1]) / r);
	float scaler = 1;
	float nd = getDistance(x1, y1, x2, y2);
	switch (dir) {
	case left:
		center[0] = x1 + tempx*cosa + tempy*sina;
		center[2] = y1 + tempy*cosa - tempx*sina;
		break;
	case right:
		center[0] = x1 + tempx*cosa - tempy*sina;
		center[2] = y1 + tempy*cosa + tempx*sina;
		break;
	case up:
		if (s + alpha > PI / 2)
			break;
		center[1] = eye[1] + r*sinf(s + alpha);
		scaler = r * cosf(s + alpha) / nd;
		center[0] = x1+(x2-x1)*scaler;
		center[2] = y1 + (y2 - y1)*scaler;
		break;
	case down:
		if (s - alpha < -PI / 2)
			break;
		center[1] = eye[1] + r*sinf(s - alpha);
		scaler = r * cosf(s - alpha) / nd;
		center[0] = x1 + (x2 - x1)*scaler;
		center[2] = y1 + (y2 - y1)*scaler;
		break;
	default:
		break;
	}
}

void zoomOut(float* eye, float* center, float delta) {
	float x0 = eye[0];
	float y0 = eye[2];
	float x1 = center[0];
	float y1 = center[2];
	float d = getDistance(x0, y0, x1, y1);
	float scaler = (d + delta) / d;
	eye[0] = x0*scaler;
	eye[2] = y0*scaler;
	center[0] = x1 + eye[0] - x0;
	center[2] = y1 + eye[2] - y0;
}

void zoomIn(float* eye, float* center, float delta) {
	float x0 = eye[0];
	float y0 = eye[2];
	float x1 = center[0];
	float y1 = center[2];
	float d = getDistance(x0, y0, x1, y1);
	float scaler = (d - delta) / d;
	if (scaler < 0)
		return;
	eye[0] = x0*scaler;
	eye[2] = y0*scaler;
	center[0] = x1 + eye[0] - x0;
	center[2] = y1 + eye[2] - y0;
}

void drawRect(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]   
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x1,y2 },{ x2,y2 },{ x2,y1 } };
	int dir[4][2] = { { 1,1 },{ 1,0 },{ 0,0 },{ 0,1 } };
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void drawCube(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	int i, j;
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat z1 = -0.5, z2 = 0.5;

	//指定六个面的四个顶点，每个顶点用3个坐标值表示     
	//前 后 上 下 左 右  

	GLfloat point[6][4][3] = {
		{ { x1,y1,z1 },{ x1,y2,z1 },{ x2,y2,z1 },{ x2,y1,z1 } },
		{ { x1,y1,z2 },{ x2,y1,z2 },{ x2,y2,z2 },{ x1,y2,z2 } },
		{ { x1,y2,z1 },{ x1,y2,z2 },{ x2,y2,z2 },{ x2,y2,z1 } },
		{ { x1,y1,z1 },{ x2,y1,z1 },{ x2,y1,z2 },{ x1,y1,z2 } },
		{ { x2,y1,z1 },{ x2,y2,z1 },{ x2,y2,z2 },{ x2,y1,z2 } },
		{ { x1,y1,z1 },{ x1,y1,z2 },{ x1,y2,z2 },{ x1,y2,z1 } },
	};

	int dir[6][4][2] = {
		{ { 0,0 },{ 0,1 },{ 1,1 },{ 1,0 } },
		{ { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } },
		{ { 0,1 },{ 0,0 },{ 1,0 },{ 1,1 } },
		{ { 1,1 },{ 0,1 },{ 0,0 },{ 1,0 } },
		{ { 1,0 },{ 1,1 },{ 0,1 },{ 0,0 } },
		{ { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } },
	};


	for (i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glBegin(GL_QUADS);
		for (j = 0; j < 4; j++) {
			glTexCoord2iv(dir[i][j]);
			glVertex3fv(point[i][j]);
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}