#pragma once
#define GLUT_DISABLE_ATEXIT_HACK 
#include <stdio.h>
#include "GL/GLUT.H" 
 

void loadTex(int i, char *filename, GLuint* texture);
typedef enum { left, right, front, back, up ,down }direction;
float getDistance(float x1, float y1, float x2, float y2);//��ö�ά�ĵ�ľ���
GLint generateNewBoundingBox(GLfloat kernelX, GLfloat kernelY, GLfloat lengthX, GLfloat lengthY);//���벢����һ���߽���ӵı�ţ�ǧ����ظ����룬����Ϊ���������x,zֵ����x����ĳ��ȣ�z����Ŀ��
void moveBoundingBox(GLint number, GLfloat moveX, GLfloat moveY);//�ƶ��߽���ӣ�����Ϊ�߽���ӵı�ţ�x�����ƶ��ľ����y����ľ���
void scalerBoundingBox(GLint number, GLfloat scaler);//���ű߽���ӣ�����Ϊ�߽���ӵı�ţ����ű���
void move(float* eye, float* center, direction dir);//ˮƽ�ƶ��۲��ߣ�ʹ�÷�ʽ��main����
void rotate(float* eye, float* center, direction dir);//�۲����ӽǵ���ת����main�д���Ϊ����������mouse����
void zoomIn(float* eye, float* center, float delta);//�Ŵ�����������ʵ�������������۲��ߺͳ���ԭ��ľ���
void zoomOut(float* eye, float* center, float delta);//��С����
void drawRect(GLuint texture);
void drawCube(GLuint texture);