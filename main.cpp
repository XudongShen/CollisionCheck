// glutEx1.cpp : 定义控制台应用程序的入口点。  
//  
#define _CRT_SECURE_NO_WARNINGS  
#include <Windows.h>
#include <stdio.h>  
#include <string.h>  
#include"MyHeader.h"  

#include <io.h>  
#include <math.h>       /* for cos(), sin(), and sqrt() */    


#define roomSizeX 100  
#define roomSizeY 15  
#define roomSizeZ 100
#define BMP_Header_Length 54


GLuint texture[3];
//视区  
float whRatio;
float whAngle=45.0f;
float fRotate = 0.0f;
float vRotate = 0.01f; //环轨道绕行速度
int bAnim = 0;         //环轨道绕行方向
int wHeight = 0;
int wWidth = 0;


int WindowWidth =800;
int WindowHeight =800;


//视点  
float center[] = { 0, 0, 35 };
float eye[] = { 0, 0, 40 };

int signal[4] = {0,0,0,0};


void grab(void)  
{  
    FILE*    pDummyFile;  //指向另一bmp文件，用于复制它的文件头和信息头数据  
    FILE*    pWritingFile;  //指向要保存截图的bmp文件  
    GLubyte* pPixelData;    //指向新的空的内存，用于保存截图bmp文件数据  
    GLubyte  BMP_Header[BMP_Header_Length];  
    GLint    i, j;  
    GLint    PixelDataLength;   //BMP文件数据总长度  
	
    // 计算像素数据的实际长度  
    i = WindowWidth * 3;   // 得到每一行的像素数据长度  
    while( i%4 != 0 )      // 补充数据，直到i是的倍数  
        ++i;                
    PixelDataLength = i * WindowHeight;  //补齐后的总位数  
  
    // 分配内存和打开文件  
    pPixelData = (GLubyte*)malloc(PixelDataLength);  
    if( pPixelData == 0 )  
        exit(0);  
  
    pDummyFile = fopen("bitmap1.bmp", "rb");//只读形式打开  
    if( pDummyFile == 0 )  
        exit(0);  
  
    pWritingFile = fopen("grab.bmp", "wb"); //只写形式打开  
    if( pWritingFile == 0 )  
        exit(0);  
  
    //把读入的bmp文件的文件头和信息头数据复制，并修改宽高数据  
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //读取文件头和信息头，占据54字节  
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);  
    fseek(pWritingFile, 0x0012, SEEK_SET); //移动到0X0012处，指向图像宽度所在内存  
    i = WindowWidth;  
    j = WindowHeight;  
    fwrite(&i, sizeof(i), 1, pWritingFile);  
    fwrite(&j, sizeof(j), 1, pWritingFile);  
  
    // 读取当前画板上图像的像素数据  
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //设置4位对齐方式  
    glReadPixels(0, 0, WindowWidth, WindowHeight,  
        GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);      
  
    // 写入像素数据  
    fseek(pWritingFile, 0, SEEK_END);  
    //把完整的BMP文件数据写入pWritingFile  
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);   
  
    // 释放内存和关闭文件  
    fclose(pDummyFile);  
    fclose(pWritingFile);  
    free(pPixelData);  
}  

void drawScene()
{
	//地板  
	glPushMatrix();
	glTranslatef(0.0f, -1.0f*roomSizeY / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);

	glScalef(roomSizeX, roomSizeZ, 1);
	drawRect(texture[0]);
	glPopMatrix();
/*
	//天花板     
	glPushMatrix();
	glTranslatef(0.0f, 1.0f*roomSizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);

	glScalef(roomSizeX, roomSizeZ, 1);
	drawRect(texture[0]);
	glPopMatrix();

	//墙壁（前）  
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.0f*roomSizeZ / 2.0);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);

	glScalef(roomSizeX, roomSizeY, 1);
	drawRect(texture[1]);
	glPopMatrix();

	//墙壁（后）  
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f*roomSizeZ / 2.0f);
	glScalef(roomSizeX, roomSizeY, 1);

	drawRect(texture[1]);
	glPopMatrix();

	//墙壁（左）  
	glPushMatrix();
	glTranslatef(-1.0f*roomSizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);

	glScalef(roomSizeZ, roomSizeY, 1);
	drawRect(texture[1]);
	glPopMatrix();

	//墙壁（右）  
	glPushMatrix();
	glTranslatef(1.0f*roomSizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);

	glScalef(roomSizeZ, roomSizeY, 1);
	drawRect(texture[1]);
	glPopMatrix();

*/
	glPushMatrix();
	glScalef(50, 15, 50);
	if (signal[3] == 0) {
		signal[3] = generateNewBoundingBox(0, 0, 50, 50);
	}
	drawCube(texture[1]);
	glPopMatrix();

	//箱子  
	glPushMatrix();
	glTranslatef(-1.0f*roomSizeX / 2.0f + 2.5f, -1.0f*roomSizeY / 2.0f + 5.0f, -1.0f*roomSizeZ / 2.0f + 5.0f);
	glScalef(5, 10, 5);
	if (signal[0] == 0) {
		signal[0] = generateNewBoundingBox(-1.0f*roomSizeX / 2.0f + 2.5f, -1.0f*roomSizeZ / 2.0f + 5.0f, 5.0f, 5.0f);
	}
	drawCube(texture[0]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f*roomSizeX / 2.0f + 2.5f, -1.0f*roomSizeY / 2.0f + 15.0f, -1.0f*roomSizeZ / 2.0f + 5.0f);
	glScalef(5, 10, 5);
	drawCube(texture[0]);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.0f*roomSizeX / 2.0f + 15.0f, -1.0f*roomSizeY / 2.0f + 5.0f, -1.0f*roomSizeZ / 2.0f + 5.0f);
	glScalef(20, 10, 5);
	if (signal[1] == 0) {
		signal[1] = generateNewBoundingBox(-1.0f*roomSizeX / 2.0f + 15.0f, -1.0f*roomSizeZ / 2.0f + 5.0f, 20.0f, 5.0f);
	}
	drawCube(texture[0]);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-1.0f*roomSizeX / 2.0f + 2.5f, -1.0f*roomSizeY / 2.0f + 5.0f, -1.0f*roomSizeZ / 2.0f + 17.5f);
	glScalef(5, 10, 20);
	if (signal[2] == 0) {
		signal[2] = generateNewBoundingBox(-1.0f*roomSizeX / 2.0f + 2.5f, -1.0f*roomSizeZ / 2.0f + 17.5f, 5.0f, 20.0f);
	}
	drawCube(texture[0]);
	glPopMatrix();
}

void updateView(int height, int width)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影       
	glLoadIdentity();   //初始化矩阵为单位矩阵          
	whRatio = (GLfloat)width / (GLfloat)height;  //设置显示比例     
	gluPerspective(whAngle, whRatio, 1.0f, 500.0f); //透视投影        
												  //glFrustum(-3, 3, -3, 3, 3,100);      
	glMatrixMode(GL_MODELVIEW);  //设置矩阵模式为模型    
}

void reshape(int width, int height)
{
	if (height == 0)      //如果高度为0      
	{
		height = 1;   //让高度为1（避免出现分母为0的现象）      
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth); //更新视角      
}


void idle()
{
	glutPostRedisplay();
}



void init()
{
	glEnable(GL_DEPTH_TEST);//开启深度测试        
	glEnable(GL_LIGHTING);  //开启光照模式   
	glGenTextures(3, texture);
	loadTex(0, "Crack.bmp", texture);
	loadTex(1, "Monet.bmp", texture);
	loadTex(2, "Spot.bmp", texture);
	glutSetCursor(GLUT_CURSOR_NONE);
	SetCursorPos(500, 500);
}

int postx = -1;
int posty = -1;
const int RANGE_VIEW_ROTATE = 2;
void mouse(int x, int y) {
	bool signal = false;
	if (postx == -1) {
		postx = x;
		posty = y;
	}
	if (x - postx > RANGE_VIEW_ROTATE) {
		rotate(eye, center, right);
		signal = true;
	}
	if (x - postx < -RANGE_VIEW_ROTATE) {
		rotate(eye, center, left);
		signal = true;
	}
	if (y - posty > RANGE_VIEW_ROTATE) {
		rotate(eye, center, down);
		signal = true;
	}
	if (y - posty < -RANGE_VIEW_ROTATE) {
		rotate(eye, center, up);
		signal = true;
	}
	if (signal) {
		SetCursorPos(500, 500);
	}
}

void key(unsigned char k, int x, int y)
{
	switch (k)
	{
	case 27:
	case 'q': {exit(0); break; } //退出  

	case 'a': { //左移  
		move(eye, center, left);
		break;
	}
	case 'd': { //右移  
		move(eye, center, right);
		break;
	}
	case 'w': { //前移  
		move(eye, center, front);
		break;
	}
	case 's': {  //后移  
		move(eye, center, back);
		break;
	}
	
	case 'j': { //环轨道左移  
		if (bAnim==0) bAnim=1;
		else if (bAnim==1) bAnim=0;
        break;
	}
	case 'l': {  //环轨道右移  
		if (bAnim==0) bAnim=2;
		else if (bAnim==2) bAnim=0;
		break;
	}
    case 'z': { //放大  
//		whAngle -= 2.0f;
//		if (whAngle < 5.0f){whAngle=5.0f;}
		zoomIn(eye, center, 0.6);
		break;
	}
	case 'c': {  //缩小  
//		whAngle += 2.0f;
//		if (whAngle > 70.0f){whAngle=70.0f;}
		zoomOut(eye, center, 0.6);
		break;
	}
	case 'v': {  //截图  
		grab();
		break;
	}
    case 'n': {  //默认视角  
		center[0] =0;
		center[1] =0;
		center[2] =35;
        eye[0] = 0;
		eye[1] = 0;
		eye[2] = 40;
		whAngle = 45.0f;
		break;
	}

	

/*	case 'j': {//视角左移  
		rotate(eye, center, left);
		break;
	}
	case 'l': {//视角右移  
		rotate(eye, center, right);
		break;
	}
	case 'i': {//视角上移  
		rotate(eye, center, up);
		//center[1] += 0.2f;
		break;
	}
	case 'k': {//视角上移  
		rotate(eye, center, down);
		//center[1] -= 0.2f;
		break;
	}
*/
	}

	updateView(wHeight, wWidth); //更新视角  
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度缓存    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   //初始化矩阵为单位矩阵      
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);                // 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上  
	glPolygonMode(GL_FRONT, GL_FILL);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	// 启用光照计算  
	glEnable(GL_LIGHTING);
	// 指定环境光强度（RGBA）  
	GLfloat ambientLight[] = { 2.0f, 2.0f, 2.0f, 1.0f };

	// 设置光照模型，将ambientLight所指定的RGBA强度值应用到环境光  
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// 启用颜色追踪  
	glEnable(GL_COLOR_MATERIAL);
	// 设置多边形正面的环境光和散射光材料属性，追踪glColor  
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glRotatef(fRotate, 0, 1.0f, 0);
	if (bAnim==1) fRotate += vRotate;
	else if(bAnim==2) fRotate -= vRotate;
	drawScene();//绘制场景   
	glutSwapBuffers();//交换缓冲区  
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//对glut的初始化         
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//初始化显示模式:RGB颜色模型，深度测试，双缓冲           
	glutInitWindowSize(800, 800);//设置窗口大小   
	glutInitWindowPosition(200, 200);
	int windowHandle = glutCreateWindow("Simple GLUT App");//设置窗口标题   
	glutDisplayFunc(redraw); //注册绘制回调函数         
	glutReshapeFunc(reshape);   //注册重绘回调函数         
	glutKeyboardFunc(key); //注册按键回调函数   
	glutPassiveMotionFunc(mouse);//注册鼠标移动函数
	glutIdleFunc(idle);//注册全局回调函数：空闲时调用       

	init();
	glutMainLoop();  // glut事件处理循环       
	return 0;
}