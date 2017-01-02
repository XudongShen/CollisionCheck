// glutEx1.cpp : �������̨Ӧ�ó������ڵ㡣  
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
//����  
float whRatio;
float whAngle=45.0f;
float fRotate = 0.0f;
float vRotate = 0.01f; //����������ٶ�
int bAnim = 0;         //��������з���
int wHeight = 0;
int wWidth = 0;


int WindowWidth =800;
int WindowHeight =800;


//�ӵ�  
float center[] = { 0, 0, 35 };
float eye[] = { 0, 0, 40 };

int signal[4] = {0,0,0,0};


void grab(void)  
{  
    FILE*    pDummyFile;  //ָ����һbmp�ļ������ڸ��������ļ�ͷ����Ϣͷ����  
    FILE*    pWritingFile;  //ָ��Ҫ�����ͼ��bmp�ļ�  
    GLubyte* pPixelData;    //ָ���µĿյ��ڴ棬���ڱ����ͼbmp�ļ�����  
    GLubyte  BMP_Header[BMP_Header_Length];  
    GLint    i, j;  
    GLint    PixelDataLength;   //BMP�ļ������ܳ���  
	
    // �����������ݵ�ʵ�ʳ���  
    i = WindowWidth * 3;   // �õ�ÿһ�е��������ݳ���  
    while( i%4 != 0 )      // �������ݣ�ֱ��i�ǵı���  
        ++i;                
    PixelDataLength = i * WindowHeight;  //��������λ��  
  
    // �����ڴ�ʹ��ļ�  
    pPixelData = (GLubyte*)malloc(PixelDataLength);  
    if( pPixelData == 0 )  
        exit(0);  
  
    pDummyFile = fopen("bitmap1.bmp", "rb");//ֻ����ʽ��  
    if( pDummyFile == 0 )  
        exit(0);  
  
    pWritingFile = fopen("grab.bmp", "wb"); //ֻд��ʽ��  
    if( pWritingFile == 0 )  
        exit(0);  
  
    //�Ѷ����bmp�ļ����ļ�ͷ����Ϣͷ���ݸ��ƣ����޸Ŀ������  
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);  //��ȡ�ļ�ͷ����Ϣͷ��ռ��54�ֽ�  
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);  
    fseek(pWritingFile, 0x0012, SEEK_SET); //�ƶ���0X0012����ָ��ͼ���������ڴ�  
    i = WindowWidth;  
    j = WindowHeight;  
    fwrite(&i, sizeof(i), 1, pWritingFile);  
    fwrite(&j, sizeof(j), 1, pWritingFile);  
  
    // ��ȡ��ǰ������ͼ�����������  
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);  //����4λ���뷽ʽ  
    glReadPixels(0, 0, WindowWidth, WindowHeight,  
        GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);      
  
    // д����������  
    fseek(pWritingFile, 0, SEEK_END);  
    //��������BMP�ļ�����д��pWritingFile  
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);   
  
    // �ͷ��ڴ�͹ر��ļ�  
    fclose(pDummyFile);  
    fclose(pWritingFile);  
    free(pPixelData);  
}  

void drawScene()
{
	//�ذ�  
	glPushMatrix();
	glTranslatef(0.0f, -1.0f*roomSizeY / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);

	glScalef(roomSizeX, roomSizeZ, 1);
	drawRect(texture[0]);
	glPopMatrix();
/*
	//�컨��     
	glPushMatrix();
	glTranslatef(0.0f, 1.0f*roomSizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);

	glScalef(roomSizeX, roomSizeZ, 1);
	drawRect(texture[0]);
	glPopMatrix();

	//ǽ�ڣ�ǰ��  
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.0f*roomSizeZ / 2.0);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);

	glScalef(roomSizeX, roomSizeY, 1);
	drawRect(texture[1]);
	glPopMatrix();

	//ǽ�ڣ���  
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f*roomSizeZ / 2.0f);
	glScalef(roomSizeX, roomSizeY, 1);

	drawRect(texture[1]);
	glPopMatrix();

	//ǽ�ڣ���  
	glPushMatrix();
	glTranslatef(-1.0f*roomSizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);

	glScalef(roomSizeZ, roomSizeY, 1);
	drawRect(texture[1]);
	glPopMatrix();

	//ǽ�ڣ��ң�  
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

	//����  
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
	glMatrixMode(GL_PROJECTION);//���þ���ģʽΪͶӰ       
	glLoadIdentity();   //��ʼ������Ϊ��λ����          
	whRatio = (GLfloat)width / (GLfloat)height;  //������ʾ����     
	gluPerspective(whAngle, whRatio, 1.0f, 500.0f); //͸��ͶӰ        
												  //glFrustum(-3, 3, -3, 3, 3,100);      
	glMatrixMode(GL_MODELVIEW);  //���þ���ģʽΪģ��    
}

void reshape(int width, int height)
{
	if (height == 0)      //����߶�Ϊ0      
	{
		height = 1;   //�ø߶�Ϊ1��������ַ�ĸΪ0������      
	}

	wHeight = height;
	wWidth = width;

	updateView(wHeight, wWidth); //�����ӽ�      
}


void idle()
{
	glutPostRedisplay();
}



void init()
{
	glEnable(GL_DEPTH_TEST);//������Ȳ���        
	glEnable(GL_LIGHTING);  //��������ģʽ   
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
	case 'q': {exit(0); break; } //�˳�  

	case 'a': { //����  
		move(eye, center, left);
		break;
	}
	case 'd': { //����  
		move(eye, center, right);
		break;
	}
	case 'w': { //ǰ��  
		move(eye, center, front);
		break;
	}
	case 's': {  //����  
		move(eye, center, back);
		break;
	}
	
	case 'j': { //���������  
		if (bAnim==0) bAnim=1;
		else if (bAnim==1) bAnim=0;
        break;
	}
	case 'l': {  //���������  
		if (bAnim==0) bAnim=2;
		else if (bAnim==2) bAnim=0;
		break;
	}
    case 'z': { //�Ŵ�  
//		whAngle -= 2.0f;
//		if (whAngle < 5.0f){whAngle=5.0f;}
		zoomIn(eye, center, 0.6);
		break;
	}
	case 'c': {  //��С  
//		whAngle += 2.0f;
//		if (whAngle > 70.0f){whAngle=70.0f;}
		zoomOut(eye, center, 0.6);
		break;
	}
	case 'v': {  //��ͼ  
		grab();
		break;
	}
    case 'n': {  //Ĭ���ӽ�  
		center[0] =0;
		center[1] =0;
		center[2] =35;
        eye[0] = 0;
		eye[1] = 0;
		eye[2] = 40;
		whAngle = 45.0f;
		break;
	}

	

/*	case 'j': {//�ӽ�����  
		rotate(eye, center, left);
		break;
	}
	case 'l': {//�ӽ�����  
		rotate(eye, center, right);
		break;
	}
	case 'i': {//�ӽ�����  
		rotate(eye, center, up);
		//center[1] += 0.2f;
		break;
	}
	case 'k': {//�ӽ�����  
		rotate(eye, center, down);
		//center[1] -= 0.2f;
		break;
	}
*/
	}

	updateView(wHeight, wWidth); //�����ӽ�  
}

void redraw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�����ɫ����Ȼ���    
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   //��ʼ������Ϊ��λ����      
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);                // ������0��0��0�����ӵ����� (0,5,50)��Y������  
	glPolygonMode(GL_FRONT, GL_FILL);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	// ���ù��ռ���  
	glEnable(GL_LIGHTING);
	// ָ��������ǿ�ȣ�RGBA��  
	GLfloat ambientLight[] = { 2.0f, 2.0f, 2.0f, 1.0f };

	// ���ù���ģ�ͣ���ambientLight��ָ����RGBAǿ��ֵӦ�õ�������  
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	// ������ɫ׷��  
	glEnable(GL_COLOR_MATERIAL);
	// ���ö��������Ļ������ɢ���������ԣ�׷��glColor  
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glRotatef(fRotate, 0, 1.0f, 0);
	if (bAnim==1) fRotate += vRotate;
	else if(bAnim==2) fRotate -= vRotate;
	drawScene();//���Ƴ���   
	glutSwapBuffers();//����������  
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);//��glut�ĳ�ʼ��         
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//��ʼ����ʾģʽ:RGB��ɫģ�ͣ���Ȳ��ԣ�˫����           
	glutInitWindowSize(800, 800);//���ô��ڴ�С   
	glutInitWindowPosition(200, 200);
	int windowHandle = glutCreateWindow("Simple GLUT App");//���ô��ڱ���   
	glutDisplayFunc(redraw); //ע����ƻص�����         
	glutReshapeFunc(reshape);   //ע���ػ�ص�����         
	glutKeyboardFunc(key); //ע�ᰴ���ص�����   
	glutPassiveMotionFunc(mouse);//ע������ƶ�����
	glutIdleFunc(idle);//ע��ȫ�ֻص�����������ʱ����       

	init();
	glutMainLoop();  // glut�¼�����ѭ��       
	return 0;
}