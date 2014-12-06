//
//  main.cpp
//  gl
//
//  Created by ying on 10/8/14.
//  Copyright (c) 2014 ying. All rights reserved.
//




/*

#include <GLUT/glut.h>

void myinit(void);
void display(void);


void myinit(void)
{

    
    //glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    GLfloat mat_ambient[]={0.0,0.1,0.8,1.0};
    GLfloat mat_diffuse[]={0.0,0.3,0.6,1.0};
    GLfloat mat_specular[]={1.0,0.0,1.0,1.0};
    GLfloat mat_shininess[]={15.0};
    GLfloat position[]={5.0,5.0,5.0,0.0};
    GLfloat fogColor[4]={0.6,0.6,0.0,1.0};
    
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
    glLightfv(GL_LIGHT0,GL_POSITION,position);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glFrontFace(GL_CW);
    // glEnable(GL_POLYGON_SMOOTH);
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    
   
    //启用雾化处理
    glEnable(GL_FOG);
    {
        //采用线性变化的雾化效果
        glFogi(GL_FOG_MODE,GL_LINEAR);
        //指定雾化颜色（黄色）
        glFogfv(GL_FOG_COLOR,fogColor);
        //指定按线性变化时计算公式的参量
        glFogf(GL_FOG_START,3.0);
        glFogf(GL_FOG_END,15.0);
        //规定雾化效果的质量
        glHint(GL_FOG_HINT,GL_DONT_CARE);
    }
 
     
    // glShadeModel(GL_FLAT);
}

void reshape(GLsizei w,GLsizei h)
{
    
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if(w<=h*3)
        glOrtho(-6.0,6.0,-2.0*(GLfloat)h*3/(GLfloat)w,
                2.0*(GLfloat)h*3/(GLfloat)w,0.0,10.0);
    else
        glOrtho(-6.0*(GLfloat)h/(GLfloat)w,
                6.0*(GLfloat)h/(GLfloat)w,-2.0,2.0,0.0,10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void display(void)
{
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    //在不同远近（Z方向）绘制同样大小、颜色的环，显示雾化的效果
    glPushMatrix();
    glTranslatef(-3.0,-1.5,-3.0);
    //auxSolidTorus(0.6,1.5);
    glutSolidTorus(0.6, 1.5, 100, 100);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(-0.5,-0.5,-6.0);
    //auxSolidTorus(0.6,1.5);
    glutSolidTorus(0.6, 1.5, 100, 100);

    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(2.0,0.5,-8.0);
    //auxSolidTorus(0.6,1.5);
    glutSolidTorus(0.6, 1.5, 100, 100);
    


    glPopMatrix();
    
    glFlush();
    
    glutSwapBuffers();

    
    
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(640, 480);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
    
    
    glutCreateWindow("Glut Demo");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    //glutIdleFunc(&mIdle);
    //glutKeyboardFunc(key);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    myinit();
    
    glutMainLoop();
    
}
 
*/






















#define TEST 0

#if TEST

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GLUT/GLUT.h>

#include "stanford_bunny.h"

#define WindowWidth 600
#define WindowHeight 600

#define BMP_Header_Length 54

void grab(void)
{
    glReadBuffer(GL_FRONT);
    
    FILE* pDummyFile;
    FILE* pWritingFile ;
    GLubyte* pPixelData;
    GLubyte BMP_Header[BMP_Header_Length];
    GLint i, j;
    GLint PixelDataLength;
    
    i = WindowWidth * 3;
    
    while( i%4 != 0 )
        ++i;
    
    PixelDataLength = i * WindowHeight;
    
    pPixelData = (GLubyte*)malloc(PixelDataLength);
    if( pPixelData == 0 ){
        printf("pPixelData failed");
        exit(0);
    }
    pDummyFile = fopen("/Users/ying/o.bmp", "rb");
    if( pDummyFile == 0 ){
        printf("pDummyFile failed");
        exit(0);
    }
    
    pWritingFile = fopen("/Users/ying/grab.bmp", "wb");
    if( pWritingFile == 0 ){
        printf("pWritingFile failed");
        exit(0);
    }
    
    
    // 读取像素
    glReadBuffer(GL_FRONT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, WindowWidth, WindowHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);
    
    // 把 dummy.bmp 的文件头复制为新文件的文件头
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile); fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
    i = WindowWidth;
    j = WindowHeight;
    
    fwrite(&i, sizeof(i), 1, pWritingFile);
    fwrite(&j, sizeof(j), 1, pWritingFile);
    
    
    // 写入像素数据
    fseek(pWritingFile, 0, SEEK_END);
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);
    
    // 释放内存和关闭文件
    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);
}


static GLint ImageWidth;
static GLint ImageHeight;
static GLint PixelLength;
static GLubyte* PixelData;
static bool displayPic = false;

void read(void){
    // 打开文件
    FILE* pFile = fopen("/Users/ying/o.bmp", "rb");
    if( pFile == 0 )
        exit(0);
    // 读取图象的大小信息
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
    fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);
    // 计算像素数据长度
    PixelLength = ImageWidth * 3;
    while( PixelLength % 4 != 0 )
        ++PixelLength;
    PixelLength *= ImageHeight;
    // 读取像素数据
    PixelData = (GLubyte*)malloc(PixelLength);
    if( PixelData == 0 )
        exit(0);
    fseek(pFile, 54, SEEK_SET);
    fread(PixelData, PixelLength, 1, pFile);
    // 关闭文件
    fclose(pFile);
    
}


// baseX, baseY: position
// ratio: size
// spin: rotate
void drawStart(float baseX, float baseY, float ratio, float spin){
    glPushMatrix();  // save framework
    
    glTranslatef(baseX,baseY,0);  // move to appropriate position
    glRotatef(spin, 0, 0, 1.0 );  // rotate an angle
    
    glBegin(GL_POLYGON);
    glColor3f(1, 1, 0);
    
    // three triangles consist of the star
    // first triangles
    glVertex2f(-0.08*ratio, +0*ratio);
    glVertex2f(-0.207*ratio, -0.4*ratio);
    glVertex2f(+0.327*ratio, +0*ratio);
    // second triangles
    glVertex2f(+0.08*ratio, +0*ratio);
    glVertex2f(+0.207*ratio, -0.4*ratio);
    glVertex2f(-0.327*ratio, +0*ratio);
    // third triangles
    glVertex2f(-0.08*ratio, +0*ratio);
    glVertex2f(+0.08*ratio, +0*ratio);
    glVertex2f(+0*ratio, +0.235*ratio);
    
    glEnd();
    
    glPopMatrix();  // revert framework
}


int power_of_two(int n) {
    if( n <= 0 )
        return 0;
    
    return (n & (n-1)) == 0;
}


/* 函数 load_texture
 * 读取一个 BMP 文件作为纹理
 * 如果失败,返回 0,如果成功,返回纹理编号
 */


GLuint load_texture(const char* file_name) {
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLint last_texture_ID;
    GLuint texture_ID = 0;
    
    // 打开文件,如果失败,返回
    FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 )
        return 0;
    
    // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);
    
    // 计算每行像素所占字节数,并根据此数据计算总像素字节数
    {
        GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
            ++line_bytes;
        total_bytes = line_bytes * height;
    }
    
    // 根据总像素字节数分配内存
    pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 )
    {
        fclose(pFile);
        return 0;
    }
    
    // 读取像素数据
    if( fread(pixels, total_bytes, 1, pFile) <= 0 ) {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    
    
    
    // 在旧版本的 OpenGL 中
    // 如果图象的宽度和高度不是的整数次方,则需要进行缩放
    // 这里并没有检查 OpenGL 版本,出于对版本兼容性的考虑,按旧版本处理
    // 另外,无论是旧版本还是新版本,
    // 当图象的宽度和高度超过当前 OpenGL 实现所支持的最大值时,也要进行缩放
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width) || !power_of_two(height) || width > max || height > max )
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;
            
            // 计算每行需要的字节数和总字节数
            new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;
            // 分配内存
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 )
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }
            
            // 进行像素缩放
            gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels,
                          new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);
            // 释放原来的像素数据,把 pixels 指向新的像素数据,并重新设置 width 和 height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }
    
    
    
    // 分配一个新的纹理编号
    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 ) {
        free(pixels);
        fclose(pFile);
        return 0;
    }
    
    
    // 绑定新的纹理,载入纹理并设置纹理参数
    // 在绑定前,先获得原来绑定的纹理编号,以便在最后进行恢复
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, last_texture_ID);
    
    // 之前为 pixels 分配的内存可在使用 glTexImage2D 以后释放
    // 因为此时像素数据已经被 OpenGL 另行保存了一份(可能被保存到专门的图形硬件中)
    free(pixels);
    return texture_ID;
}


void draw_sphere()
{
    // 设置光源
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    {
        GLfloat
        pos[] = {5.0f, 5.0f, 0.0f, 1.0f},
        ambient[] = {0.0f, 0.0f, 1.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    }
    
    // 绘制一个球体 glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(0, 0, 2);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}






void setLight(void)
{
    static const GLfloat light_position[] = {1.0f, 1.0f, -1.0f, 1.0f};
    static const GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    static const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    static const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}


void setMatirial(const GLfloat mat_diffuse[4], GLfloat mat_shininess)
{
    static const GLfloat mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    static const GLfloat mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialf (GL_FRONT, GL_SHININESS, mat_shininess);
}

static float eye[]={0, 0, 1};
static float center[]={0, 0, -1};

static float di = 0.1;

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
        case 27:
        case 'q': {
            exit(0);
            break;
        }
        case 'p': { break; }
            
        case ' ': { break;}
        case 'o': { break;}
            
        case 'a': {
            eye[0]=eye[0]-di;
            break;
        }
        case 'd': {
            eye[0]=eye[0]+di;
            break;
        }
        case 'w': {
            eye[1]=eye[1]+di;
            break;
        }
        case 's': {
            eye[1]=eye[1]-di;
            break;
        }
        case 'e': {
            eye[2]=eye[2]-di;
            break;
        }
        case 'c': {
            eye[2]=eye[2]+di;
            break;
        }
            
        case 'j':{
            center[0]=center[0]-di;
            break;
        }
        case 'l':{
            center[0]=center[0]+di;
            break;
        }
        case 'i':{
            center[0]=center[0]+di;
            break;
        }
        case 'k':{
            center[0]=center[0]-di;
            break;
        }
        case 'u':{
            center[0]=center[0]-di;
            break;
        }
        case 'n':{
            center[0]=center[0]+di;
            break;
        }
        case 'g':{
            grab();
            break;
        }
        case 'h':{
            displayPic = !displayPic;
            read();
            break;
        }
	}
}




float angle = 0.0;


GLuint texGround;
GLuint texWall;
GLuint texSky;

//float mat_specular[] = {0.85f, 0.65f, 0.2f, 1.0f};
//float mat_ambient[] = {0.85f, 0.65f, 0.2f, 1.0f};
//float mat_shininess[] = {0.85f, 0.65f, 0.2f, 1.0f};
//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
//glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
//glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


void display()
{
    /*
     // 清除屏幕
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     // 设置观察点
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(60, 1, 5, 25);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     gluLookAt(5, 0, 6.5, 0, 0, 0, 0, 1, 0);
     
     glEnable(GL_DEPTH_TEST);
     // 绘制球体
     glDisable(GL_STENCIL_TEST );
     draw_sphere();
     
     // 绘制一个平面镜。在绘制的同时注意设置模板缓冲。
     // 另外,为了保证平面镜之后的镜像能够正确绘制,在绘制平面镜时需要将深度缓冲区设置为只读的。
     // 在绘制时暂时关闭光照效果
     glClearStencil(0);
     glClear(GL_STENCIL_BUFFER_BIT);
     
     glStencilFunc(GL_ALWAYS, 1, 0xFF);
     glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
     
     glEnable(GL_STENCIL_TEST);
     glDisable(GL_LIGHTING);
     
     glColor3f(0.5f, 0.5f, 0.5f);
     glDepthMask(GL_FALSE);
     glStencilMask(GL_TRUE);
     glRectf(-1.5f, -1.5f, 1.5f, 1.5f);
     glDepthMask(GL_TRUE);
     
     // 绘制一个与先前球体关于平面镜对称的球体,注意光源的位置也要发生对称改变
     // 因为平面镜是在 X 轴和 Y 轴所确定的平面,所以只要 Z 坐标取反即可实现对称
     // 为了保证球体的绘制范围被限制在平面镜内部,使用模板测试
     glStencilFunc(GL_EQUAL, 1, 0xFF);
     //glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
     glScalef(1.0f, 1.0f, -1.0f);
     draw_sphere();
     
     // 交换缓冲
     glutSwapBuffers();
     
     return;
     
     */
    
    
    
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 设置视角
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, 1, 1, 21);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1, 5, 5, 0, 0, 0, 0, 0, 1);
    
    
    
//    GLUquadricObj *q;
//    q = gluNewQuadric(); // 绘制球体
//    gluQuadricNormals(q, GL_SMOOTH); // 产生光滑
//    gluQuadricTexture(q, GL_TRUE); // 激活曲面纹理坐标参照
//    
//    glBindTexture(GL_TEXTURE_2D, texSky); // 设置纹理
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    gluSphere(q, 10.0f, 32, 16); // 绘制球体
    

    
//    //void DrawObject() // 绘制气球球体
//    {
//        glTranslatef(0.0f,1.5f,0.0f);
//        glColor3f(1.0f, 1.0f, 1.0f); // 设置成白色
//        //glBindTexture(GL_TEXTURE_2D, texture1[0]); // Select Texture 0 (1)
//        gluSphere(q, 1.0f, 32, 16); // 绘制球体
//        glBindTexture(GL_TEXTURE_2D, texSky); // 设置纹理
//        glColor4f(1.0f, 1.0f, 1.0f, 0.4f); // 设置成白色，40%透明效果（融合）
//        glEnable(GL_BLEND); // 启动融合操作
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE); // 指定混合的属性
//        glEnable(GL_TEXTURE_GEN_S); // 起用球体纹理影射
//        glEnable(GL_TEXTURE_GEN_T); // 起用球体纹理影射
//        gluSphere(q, 1.0f, 32, 16); // 用新的纹理来绘制第二个球体球体
//        // 将产生多维纹理效果
//        glDisable(GL_TEXTURE_GEN_S); // 取消球体纹理影射
//        glDisable(GL_TEXTURE_GEN_T); // 取消球体纹理影射
//        glDisable(GL_BLEND); // 取消混合
//    }
    
    
    

    
gluCylinder
    
    
    glutSwapBuffers();

    
    
    
    return;

    
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 设置视角
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, 1, 1, 21);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(1, 5, 5, 0, 0, 0, 0, 0, 1);
    
    // 使用“地”纹理绘制土地
    glBindTexture(GL_TEXTURE_2D, texGround);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-8.0f, -8.0f, 0.0f);
    glTexCoord2f(0.0f, 5.0f);
    glVertex3f(-8.0f, 8.0f, 0.0f);
    glTexCoord2f(5.0f, 5.0f);
    glVertex3f(8.0f, 8.0f, 0.0f);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(8.0f, -8.0f, 0.0f);
    glEnd();
    
    // 使用“墙”纹理绘制栅栏
    glBindTexture(GL_TEXTURE_2D, texWall);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.0f, -3.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-6.0f, -3.0f, 1.5f);
    glTexCoord2f(5.0f, 1.0f);
    glVertex3f(6.0f, -3.0f, 1.5f);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(6.0f, -3.0f, 0.0f);
    glEnd();
    
    // 旋转后再绘制一个
    glRotatef(-90, 0, 0, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-6.0f, -3.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-6.0f, -3.0f, 1.5f);
    glTexCoord2f(5.0f, 1.0f);
    glVertex3f(6.0f, -3.0f, 1.5f);
    glTexCoord2f(5.0f, 0.0f);
    glVertex3f(6.0f, -3.0f, 0.0f);
    glEnd();
    
    // 交换缓冲区,并保存像素数据到文件
    glutSwapBuffers();
    
    return;
    
    
    
    
    
    
    
    // 定义一些材质颜色
    const static GLfloat red_color[] = {1.0f, 0.0f, 0.0f, 1.0f};
    const static GLfloat green_color[] = {0.0f, 1.0f, 0.0f, 0.2f};
    const static GLfloat blue_color[] = {0.0f, 0.0f, 1.0f, 0.3f};
    
    
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 启动混合并设置混合因子
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 0.1f, 10.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /*
     gluLookAt(eye[0], eye[1], eye[2],
     center[0], center[1], center[2],
     0, 1, 0);
     */
    //gluLookAt(0, 1, 0, 0, 0, 0, 0, 0, 1);
    gluLookAt(0, 0, 1, 0, 0, 0, 0, 1, 0);
    
    // 设置光源
    setLight();
    
    // 以(0, 0, 0.5)为中心,绘制一个半径为.3 的不透明红色球体(离观察者最远)
    setMatirial(red_color, 30.0);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -1.0f);
    glutSolidSphere(0.3, 300, 300);
    glPopMatrix();
    
    
    // 下面将绘制半透明物体了,因此将深度缓冲设置为只读
    glDepthMask(GL_FALSE);
    
    
    
    // 以(0.1, 0, 0)为中心,绘制一个半径为.15 的半透明绿色球体(在前两个球体之间)
    setMatirial(green_color, 30.0);
    glPushMatrix();
    glTranslatef(0.1, 0, -0.5);
    glutSolidSphere(0.15, 300, 300);
    glPopMatrix();
    
    
    // 以(0.2, 0, -0.5)为中心,绘制一个半径为.2 的半透明蓝色球体(离观察者最近)
    setMatirial(blue_color, 30.0);
    glPushMatrix();
    glTranslatef(0.2f, 0.0f, -0.1f);
    glutSolidSphere(0.2, 300, 300);
    glPopMatrix();
    
    
    
    
    
    
    
    // 完成半透明物体的绘制,将深度缓冲区恢复为可读可写的形式
    glDepthMask(GL_TRUE);
    
    if(displayPic){
        glPixelZoom(1.0f,1.0f);
        glRasterPos2f(0,0);
        glDrawPixels(ImageWidth, ImageHeight, GL_BGR, GL_UNSIGNED_BYTE, PixelData);
        
    }
    
    /*
     glPixelZoom(-0.5f,-0.5f);
     glRasterPos2f(0,0);
     glCopyPixels(WindowWidth/2, WindowHeight/2, WindowWidth/2, WindowHeight/2, GL_COLOR);
     */
    
    
    glutSwapBuffers();
    
    
    return;
    
    
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 创建透视效果视图
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, 1.0f, 1.0f, 20.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 5.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    
    
    {
        GLfloat global_light_position[] = {0.9f, 0.0f, 0.9f, 1.0f};
        GLfloat global_light_ambient[] = {0.8f, 0.8f, 0.8f, 1.0f};
        GLfloat global_light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat global_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glLightfv(GL_LIGHT1, GL_POSITION, global_light_position);
        glLightfv(GL_LIGHT1, GL_AMBIENT, global_light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, global_light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, global_light_specular);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }
    
    
    
    // 定义太阳光源,它是一种白色的光源
    {
        GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
        GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
        glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
        glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHTING);
        glEnable(GL_DEPTH_TEST);
    }
    
    
    // 定义太阳的材质并绘制太阳
    {
        GLfloat sun_mat_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_diffuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_emission[] = {0.5f, 0.0f, 0.0f, 1.0f};
        GLfloat sun_mat_shininess = 0.0f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
        glMaterialf (GL_FRONT, GL_SHININESS, sun_mat_shininess);
        glutSolidSphere(2.0, 40, 32);
    }
    
    // 定义地球的材质并绘制地球
    {
        GLfloat earth_mat_ambient[] = {0.0f, 0.0f, 0.5f, 1.0f};
        GLfloat earth_mat_diffuse[] = {0.0f, 0.0f, 0.5f, 1.0f};
        GLfloat earth_mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
        GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat earth_mat_shininess = 30.0f;
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
        glMaterialf (GL_FRONT, GL_SHININESS, earth_mat_shininess);
        glRotatef(angle, 0.0f, -1.0f, 0.0f);
        glTranslatef(5.0f, 0.0f, 0.0f);
        glutSolidSphere(2.0, 40, 32);
    }
    
    
    float x=-0.8;
    float y=0.1;
    float z=0.0;
    glPushMatrix();
    
    glTranslatef(x, y, z);
    glRotatef(90, 1, 0, 0);
    glScalef(3,3,3);
    glColor3f(0.5,0.5,0.5);
    
    DrawBunny();
    glPopMatrix();
    
    
    glutSwapBuffers();
}



/*
 
 glEnable(GL_DEPTH_TEST);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(75, 1, 1, 400000000);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 //    gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);
 gluLookAt(-2, 5, 10, 0, 0, 0, 0, 1, 0);
 
 
 
 
 // 绘制红色的“太阳”
 glColor3f(1.0f, 0.0f, 0.0f);
 glutSolidSphere(0.69600000, 20, 20);
 // 绘制蓝色的“地球”
 glColor3f(0.0f, 0.0f, 1.0f);
 glRotatef(day/360.0*360.0, 0.0f, 0.0f, -1.0f);
 glTranslatef(0.150000000, 0.0f, 0.0f);
 glutSolidSphere(0.15945000, 20, 20);
 // 绘制黄色的“月亮”
 glColor3f(1.0f, 1.0f, 0.0f);
 glRotatef(day/30.0*360.0 - day/360.0*360.0, 0.0f, 0.0f, -1.0f);
 glTranslatef(0.38000000, 0.0f, 0.0f);
 
 glutSolidSphere(0.4345000, 20, 20);
 
 glutSolidSphere(0.5,20,20);
 
 
 glutSwapBuffers();
 
 
 }
 */

void mIdle(void)
{
    angle += 1.0f;
    if( angle >= 360.0f )
        angle = 0.0f;
    
    display();
}

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(WindowWidth, WindowHeight);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_STENCIL);
    
    
    glutCreateWindow("Glut Demo");
    glutDisplayFunc(display);
    glutIdleFunc(&mIdle);
    glutKeyboardFunc(key);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    texGround = load_texture("/Users/ying/ground.bmp");
    texWall = load_texture("/Users/ying/wall.bmp");
    texSky = load_texture("/Users/ying/nightskya.bmp");
    
    
    glutMainLoop();
    

}

#endif


