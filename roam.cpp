#if 0


#include<stdlib.h>
#include <GLUT/glut.h>
#include<stdio.h>
#include<iostream>
#include <math.h>

GLfloat ambientLight[]={0.7f,0.7f,0.7f,1.0f};//全局环境光等于加个偏移量
GLfloat specular[]={0.1f,0.1f,0.1f,0.1f};//全局白色镜面光
GLfloat diffuse[]={0.1f,0.1f,0.1f,1.00};//全局散射光
GLfloat emission[]={0.15f,0.15f,0.15f,1.00};//全局辐射
GLfloat null[]={0,0.0f,0.0f,0.0f};        

GLfloat lightPos0[4]={30.0f,100.0f,0.0f,1.0f};    //聚光灯0位置// 最后一位设为0,则为平行光// Playing时的位置
GLfloat lightPos1[4]={-5.0f,50.0f,-10.0f,1.0f};    //SELECT_MODE 的光位置

GLfloat fAspect;//窗口高宽比, 见鼠标右键和reshape

GLfloat matrixcamera[]={1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,-100.0,1.0};//漫游用存储矩阵   
GLfloat matrixreverse[]={1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,100.0,1.0};// matrixcamera 的逆矩阵  //应用:碰撞检测  12=Tx  13=Ty  14=Tz

char *str;//显示字用

#define MapWidth 200
#define MapDepth 200

#define SM_CXSCREEN 680
#define SM_CYSCREEN 480


struct POINT{
    int x;
    int y;
};

GLfloat xrot=0;        //抬头角度  见 display 和  mouse_move，本来也可以嵌入到matrixcamera里面.不过这样简单.
#define PI            3.141592653
POINT curpos;    //鼠标当前位置 .x  .y 坐标


void DrawGround(void)//黑白格子地板
{
    static GLfloat fExtent=200    ;    //地图长和宽的一半，见DrawGround
    static GLfloat fStep  =20 ;      //网格间隔，见DrawGround
    static GLfloat y      =-10;    //地板下移, 见DrawGround
    GLint j=0;
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_QUADS);
        for(GLint x=-fExtent;x<=fExtent;x+=fStep)
        {    
            for(GLint z=-fExtent;z<=fExtent;z+=fStep)
            {
            glVertex3f(x,y,z);                //左下
            glVertex3f(x+fStep,y,z);        //右下
            glVertex3f(x+fStep,y,z-fStep);    //右上
            glVertex3f(x,y,z-fStep);        //左上
            j++;
            if(j%2==1)
                glColor3f(1.0f,1.0f,1.0f);
            else
                glColor3f(0.0f,0.0f,0.0f);
        }}
    glEnd();
}

void display(void)
{        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(0.0f,0.0f,-30.0f);//全局前移,让camera看到玩家;如果不前移则为第一人称;前移了是第三人称.
        glRotatef(xrot,1,0,0);//控制抬头
        glPushMatrix();// 画玩家
            glTranslatef(0,-10+4.5,0);//-10为地板下移量(见DrawGround,可设为全局量）,加偏移量让玩家在地面上
            glColor3f(0.5f,0.5f,0.0f);
            glutSolidSphere(5.0f,100,100);//player 5为半径
        glPopMatrix();
        
        glMultMatrixf(matrixcamera);
        glLightfv(GL_LIGHT0,GL_POSITION,lightPos0);/* Light *///更新聚光灯0的位置其实是不动的，不过第一人称动了
        glPushMatrix();    /* 画地图 */
            DrawGround();
        glPopMatrix();
    glutSwapBuffers(); //前后缓冲区交换
}
void TimerFunction(int value)
{    
    glutPostRedisplay();

//    GetCursorPos(&curpos);//获取鼠标当前位置, 防止鼠标到达边界
//    if(curpos.x<( SM_CXSCREEN )*0.01 ||
//        curpos.x>( SM_CXSCREEN )*0.99 ||
//            curpos.y<( SM_CYSCREEN )*0.01 ||
//                curpos.y>( SM_CYSCREEN )*0.99)
//    SetCursorPos(( SM_CXSCREEN )/2,( SM_CYSCREEN )/2);//设置鼠标的位置//

    glutTimerFunc(1000/60,TimerFunction,value);
}
void IdleFunc(void){}
void mouse_press(int button,int state,int x,int y){}
void mouse_move(int x, int y)
{
    static GLint x_buffer=0, y_buffer=0; // i.e. 让现在坐标和之前坐标做比较
    if(x<x_buffer)
    {    glPushMatrix();
            glLoadIdentity();
            glLoadMatrixf(matrixreverse);
            glRotatef(1.2,0,1,0);
            glGetFloatv(GL_MODELVIEW_MATRIX,matrixreverse);

            glLoadIdentity();
            glRotatef(-1.2,0,1,0);
            glMultMatrixf(matrixcamera);
            glGetFloatv(GL_MODELVIEW_MATRIX,matrixcamera);
        glPopMatrix();}
    else if(x>x_buffer)
    {    glPushMatrix();
            glLoadIdentity();
            glLoadMatrixf(matrixreverse);
            glRotatef(-1.2,0,1,0);
            glGetFloatv(GL_MODELVIEW_MATRIX,matrixreverse);

            glLoadIdentity();
            glRotatef(1.2,0,1,0);
            glMultMatrixf(matrixcamera);
            glGetFloatv(GL_MODELVIEW_MATRIX,matrixcamera);
        glPopMatrix();}

    if(y<y_buffer)    //抬头
    {    if(xrot>-10)
            xrot-=0.5;}
    else if(y>y_buffer)//低头
    {    if(xrot<90)
            xrot+=0.5;}

    x_buffer=x;    //    存储起来，下次做比较用
    y_buffer=y;

}


void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
      case 'w'://前
      case 'W':
              glPushMatrix();
                glLoadIdentity();
                glLoadMatrixf(matrixreverse);
                glTranslatef(0.0f,0.0f,-1.0f);
                glGetFloatv(GL_MODELVIEW_MATRIX,matrixreverse);

                glLoadIdentity();
                glTranslatef(0.0f,0.0f,1.0f);
                glMultMatrixf(matrixcamera);
                glGetFloatv(GL_MODELVIEW_MATRIX,matrixcamera);
            glPopMatrix();
            break;
      case 's'://后
      case 'S':
              glPushMatrix();
                glLoadIdentity();
                glLoadMatrixf(matrixreverse);
                glTranslatef(0.0f,0.0f,1.0f);
                glGetFloatv(GL_MODELVIEW_MATRIX,matrixreverse);

                glLoadIdentity();
                glTranslatef(0.0f,0.0f,-1.0f);
                glMultMatrixf(matrixcamera);
                glGetFloatv(GL_MODELVIEW_MATRIX,matrixcamera);
            glPopMatrix();
            break;
      case 'a'://左转
      case 'A':
              glPushMatrix();
                glLoadIdentity();
                glLoadMatrixf(matrixreverse);
                glRotatef(3,0,1,0);
                glGetFloatv(GL_MODELVIEW_MATRIX,matrixreverse);

                glLoadIdentity();
                glRotatef(-3,0,1,0);
                glMultMatrixf(matrixcamera);
                glGetFloatv(GL_MODELVIEW_MATRIX,matrixcamera);
            glPopMatrix();
            break;
      case 'd'://右转
      case 'D':
              glPushMatrix();
                glLoadIdentity();
                glLoadMatrixf(matrixreverse);
                glRotatef(-3,0,1,0);
                glGetFloatv(GL_MODELVIEW_MATRIX,matrixreverse);

                glLoadIdentity();
                glRotatef(3,0,1,0);
                glMultMatrixf(matrixcamera);
                glGetFloatv(GL_MODELVIEW_MATRIX,matrixcamera);
            glPopMatrix();
            break;
      default:
            break;
    
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////
}
void init(void)
{    
    //sndPlaySound("sound/Start.wav",SND_ASYNC|SND_NODEFAULT);
    glShadeModel(GL_SMOOTH);//FLAT填充颜色取决于最后一个点//SMOOTH渐变(Default)
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);//剔除

    /* 光 */
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);//颜色追踪根据glColor设置材料属性
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);//环境光和散射光根据物体本身颜色 ie 根据glColor

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);//设置环境光

    glEnable(GL_LIGHT0);//使聚光灯0有效，其位置在display func中实时更新

    glMaterialfv(GL_FRONT,GL_SPECULAR,specular);//镜面光设置
    glMateriali(GL_FRONT,GL_SHININESS,128);        //镜面指数 1大--128小
    glMaterialfv(GL_FRONT, GL_DIFFUSE,diffuse); //散射设置
    glMaterialfv(GL_FRONT, GL_EMISSION,emission);//辐射设置

    /* 雾 */
    glEnable(GL_FOG);
    GLfloat fogColor[4]= {0.1f, 0.1f, 0.1f, 1.0f};  //雾色  
    glFogfv(GL_FOG_COLOR,fogColor);//设置雾色
    glFogf(GL_FOG_START,50.0f);//雾开始位置离原点距离
    glFogf(GL_FOG_END,1000.0f);    //雾停止位置离原点距离
    glFogi(GL_FOG_MODE,GL_LINEAR);//雾方程
    glHint(GL_FOG_HINT,GL_NICEST);//最好效果,一般可用FASTEST,好用NICEST,不理用DONT_CARE
    glFogf(GL_FOG_DENSITY, 1);

    /* 混合 */ //Superbible第四版 152 页 画透明物体用
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);//画透明用
        //抗锯齿
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

    //控制开关颜色通道
    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    glutSetCursor(GLUT_CURSOR_NONE);//隐藏鼠标

    //建立字符输出显示列表
    GLuint base=glGenLists(256);
    glListBase(base);
    for(int i=0;i<256;i++)
    {    
        glNewList(base + i,GL_COMPILE);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, i);
        glEndList();
    }
}
void reshape(GLsizei w,GLsizei h)
{
    glViewport(0,0,w,h);
    fAspect=(GLfloat)w/(GLfloat)h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80.0f,fAspect,1.0,2000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
};

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
    //glutInitWindowSize (GetSystemMetrics( SM_CXSCREEN ),GetSystemMetrics( SM_CYSCREEN ));//自动获取屏幕大小
    glutInitWindowSize(SM_CXSCREEN, SM_CYSCREEN);
    glutInitWindowPosition (0,0);
    glutCreateWindow("DH");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(IdleFunc);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse_press);
    glutPassiveMotionFunc(mouse_move);//当鼠标在窗口中移动,并按下键时调用
    glutMotionFunc(mouse_move);//当鼠标在窗口中移动,并没有按下键时调用
    glutTimerFunc(100,TimerFunction,1);
    glutMainLoop();
    return 0;
}



#endif