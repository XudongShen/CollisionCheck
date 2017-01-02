#pragma once
#define GLUT_DISABLE_ATEXIT_HACK 
#include <stdio.h>
#include "GL/GLUT.H" 
 

void loadTex(int i, char *filename, GLuint* texture);
typedef enum { left, right, front, back, up ,down }direction;
float getDistance(float x1, float y1, float x2, float y2);//获得二维的点的距离
GLint generateNewBoundingBox(GLfloat kernelX, GLfloat kernelY, GLfloat lengthX, GLfloat lengthY);//申请并生成一个边界盒子的编号，千万别重复申请，参数为中心坐标的x,z值，和x方向的长度，z方向的宽度
void moveBoundingBox(GLint number, GLfloat moveX, GLfloat moveY);//移动边界盒子，参数为边界盒子的编号，x方向移动的距离和y方向的距离
void scalerBoundingBox(GLint number, GLfloat scaler);//缩放边界盒子，参数为边界盒子的编号，缩放比例
void move(float* eye, float* center, direction dir);//水平移动观察者，使用方式见main函数
void rotate(float* eye, float* center, direction dir);//观察者视角的旋转，在main中处理为鼠标操作，见mouse函数
void zoomIn(float* eye, float* center, float delta);//放大整个场景，实际作用是拉进观察者和场景原点的距离
void zoomOut(float* eye, float* center, float delta);//缩小场景
void drawRect(GLuint texture);
void drawCube(GLuint texture);