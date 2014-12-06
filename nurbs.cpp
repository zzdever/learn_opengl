
#define NURBS 0


# if NURBS
#include <iostream>
#include <GLUT/GLUT.h>
#include <math.h>

#define PI 3.14

GLfloat spin_x = 0;/*整个图形绕x轴旋转角度*/
GLfloat spin_y = 0;/*整个图形绕y轴旋转角度*/
GLfloat spin_z = 0;/*整个图形绕z轴旋转角度*/

GLUnurbsObj *pNurb_base = NULL;/*NURBS对象指针*/
GLUnurbsObj *pNurb_pipe = NULL;
GLUnurbsObj *pNurb_lamp = NULL;

float eye[] = {0, 0, 15.0f};
float center[] = {0, 0, 0};

GLint drawlist;

//台灯底座的控制点
GLfloat ctrlPoints_base[4][4][3]={
    {{ -3.0f, -3.0f, -3.0f },
        { -4.5f, -2.0f, -3.0f },
        { -4.5f, 2.0f, -3.0f },
        { -3.0f, 3.0f, -3.0f }},
    
    {{ -1.0f, -4.5f, -3.0f },
        { -1.0f, -1.0f, 1.0f },
        { -1.0f, 1.0f, 1.0f },
        { -1.0f, 4.5f, -3.0f }},
    
    {{ 1.0f, -4.5f, -3.0f },
        { 1.0f, -1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 4.5f, -3.0f }},
    
    {{ 3.0f, -3.0f, -3.0f },
        { 4.5f, -2.0f, -3.0f },
        { 4.5f, 2.0f, -3.0f },
        { 3.0f, 3.0f, -3.0f }}};

//台灯弯曲管的控制点
GLfloat ctrlPoints_pipe[5][5][3] = {
    {{-4.5f, 0.0f, -1.0f},
        {-4.5f, 0.0f, 3.0f},
        {-3.0f, 0.0f, 6.0f},
        {1.0f, 0.0f, 9.0f},
        {6.0f, 0.0f, 11.0f}},
    
    {{-3.5f, -2.0f, -1.0f},
        {-3.5f, -2.0f, 4.0f},
        {-2.0f, -2.0f, 7.0f},
        {1.5f, -2.0f, 8.0f},
        {6.0f, -2.0f, 10.0f}},
    
    {{-2.5f, 0.0f, -1.0f},
        {-2.5f, 0.0f, 4.0f},
        {-1.0f, 0.0f, 6.0f},
        {2.0f, 0.0f, 7.0f},
        {6.0f, 0.0f, 9.0f}},
    
    {{-3.5f, 2.0f, -1.0f},
        {-3.5f, 2.0f, 4.0f},
        {-2.0f, 2.0f, 7.0f},
        {1.5f, 2.0f, 8.0f},
        {6.0f, 2.0f, 10.0f}},
    
    {{-4.5f, 0.0f, -1.0f},
        {-4.5f, 0.0f, 3.0f},
        {-3.0f, 0.0f, 6.0f},
        {1.0f, 0.0f, 9.0f},
        {6.0f, 0.0f, 11.0f}},
};

//灯罩的控制点
GLfloat ctrlPoints_lamp[4][6][3] = {
    {{8.0f, -5.0f, 9.0f},
        {8.0f, -3.0f, 9.0f},
        {8.0f, -1.0f, 9.0f},
        {8.0f, 1.0f, 9.0f},
        {8.0f, 3.0f, 9.0f},
        {8.0f, 5.0f, 9.0f}},
    
    {{9.0f, -7.0f, 9.0f},
        {9.0f, -6.0f, 11.0f},
        {9.05f, -5.0f, 13.0f},
        {9.0f, 5.0f, 13.0f},
        {9.0f, 6.0f, 11.0f},
        {9.0f, 7.0f, 9.0f}},
    
    {{10.0f, -7.0f, 9.0f},
        {10.0f, -6.0f, 11.0f},
        {10.0f, -5.0f, 13.0f},
        {10.0f, 5.0f, 13.0f},
        {10.0f, 6.0f, 11.0f},
        {10.0f, 7.0f, 9.0f}},
    
    {{11.0f, -5.0f, 9.0f},
        {11.0f, -3.0f, 9.0f},
        {11.0f, -1.0f, 9.0f},
        {11.0f, 1.0f, 9.0f},
        {11.0f, 3.0f, 9.0f},
        {11.0f, 5.0f, 9.0f}}
};

/*u方向的结点序列，u_knot数=u方向控制网格数+u方向阶数*/
GLfloat u_knots_base[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat u_knots_pipe[10] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat u_knots_lamp[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
/*v方向的结点序列，v_knot数=v方向控制网格数+v方向阶数*/
GLfloat v_knots_base[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat v_knots_pipe[10] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
GLfloat v_knots_lamp[12] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};


void Draw_base()
{
    glScaled(1, 1, 2);
    glutSolidCube(1.0);
}

/*显示NURB控制点*/
void DrawPoints(void)
{
	int i,j;
    
	glPointSize(5.0f);
	glColor3ub(255,0,0);/*红色*/
    
	glBegin(GL_POINTS);
    for(i = 0; i < 5; i++)
        for(j = 0; j < 7; j++)
            glVertex3fv(ctrlPoints_lamp[i][j]);
	glEnd();
}

/*整个图形的旋转*/
void spin()
{
	glutPostRedisplay();
}

/*
 void updateView(int width, int height)
 {
 glViewport(0,0,width,height);						// Reset The Current Viewport
 
 glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
 glLoadIdentity();									// Reset The Projection Matrix
 
 float whRatio = (GLfloat)width/(GLfloat)height;
 gluPerspective(45.0f, whRatio,0.1f,100.0f);
 
 glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
 }
 */

/*特殊键响应*/
void spec_keyboard(int key, int x, int y)
{
	switch(key)
	{
        case GLUT_KEY_LEFT:/*左箭头*/
            spin_y += 2.0;
            break;
        case GLUT_KEY_RIGHT:/*右箭头*/
            spin_y -= 2.0;
            break;
        case GLUT_KEY_UP:/*上箭头*/
            spin_x += 2.0;
            break;
        case GLUT_KEY_DOWN:/*下箭头*/
            spin_x -= 2.0;
            break;
        case '-':
            spin_z += 2.0;
            break;
        case '=':
            spin_z -= 2.0;
            break;
        case 'j':
            eye[0] -= 0.2f;
            center[0] -= 0.2f;
            break;
        case 'l':
            eye[0] += 0.2f;
            center[0] += 0.2f;
            break;
        case 'i':
            eye[1] -= 0.2f;
            center[1] -= 0.2f;
            break;
        case 'k':
            eye[1] += 0.2f;
            center[1] += 0.2f;
            break;
        case 'n':
            eye[2] -= 0.2f;
            center[2] -= 0.2f;
            break;
        case 'm':
            eye[2] += 0.2f;
            center[2] += 0.2f;
            break;
        case 'q':
            exit(0);
            break;
        default:
            break;
	}
}

/*NURBS错误回调函数*/
void CALLBACK NurbsErrorHandler(GLenum nErrorCode)
{
	char cMessage[64];
    
	/*提取错误信息*/
	strcpy(cMessage,"NURBS error occured: ");
	strcat(cMessage, (char *)gluErrorString(nErrorCode));
    
	/*显示错误信息*/
	glutSetWindowTitle(cMessage);
}

GLint RenderScene(void);

void redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    glRotatef(spin_x, 1.0, 0, 0);
    
    RenderScene();
    //glCallList(drawlist);
    
    spin_x += 0.5f;
    
    glutSwapBuffers();/*显示图像*/
}

//用于绘制台灯
GLint RenderScene(void)
{

    GLint lid = 0;
//    lid = glGenLists(1);
//	glNewList(lid, GL_COMPILE);
    
	/*保存模型视图矩阵*/
	glMatrixMode(GL_MODELVIEW);
    
    
	glPushMatrix();
    //    glScalef(0.5, 0.5, 0.5);
    
	/*特殊键控制网格旋转*/
	glRotatef(spin_x, 1.0, 0.0, 0.0);//绕x轴旋转
	glRotatef(spin_y, 0.0, 1.0, 0.0);//绕y轴旋转
	glRotatef(spin_z, 0.0, 0.0, 1.0);//绕z轴旋转
    gluLookAt(eye[0], eye[1], eye[2],center[0], center[1], center[2], 0, 1, 0);
    
	gluBeginSurface(pNurb_base);
    float mat_diffuse_base[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_specular_base[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_base);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_base);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
    glColor3ub(0,255,20);
    gluNurbsSurface(pNurb_base, 8, u_knots_base, 8, v_knots_base, 4 * 3, 3, &ctrlPoints_base[0][0][0], 4, 4, GL_MAP2_VERTEX_3);//[4][4][3]
	gluEndSurface(pNurb_base);
    glPopMatrix();
    
    gluBeginSurface(pNurb_lamp);
    float mat_diffuse_lamp[] = {0.0f, 1.0f, 0.2f, 1.0f};
    float mat_specular_lamp[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_lamp);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_lamp);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
    glColor3ub(0,255,20);
    gluNurbsSurface(pNurb_lamp, 8, u_knots_lamp, 12, v_knots_lamp, 6 * 3, 3, &ctrlPoints_lamp[0][0][0], 4, 6, GL_MAP2_VERTEX_3);//[4][6][3]
	gluEndSurface(pNurb_lamp);
    glPopMatrix();
    
    gluBeginSurface(pNurb_pipe);
    float mat_diffuse_pipe[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_specular_pipe[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_pipe);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_pipe);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
    glColor3ub(192,192,192);
    gluNurbsSurface(pNurb_pipe, 10, u_knots_pipe, 10, v_knots_pipe, 5 * 3, 3, &ctrlPoints_pipe[0][0][0], 5, 5, GL_MAP2_VERTEX_3);//[5][5][3]
    gluEndSurface(pNurb_pipe);
    glPopMatrix();
    
    float mat_diffuse_cube[] = {0.0f, 1.0f, 0.2f, 1.0f};
    float mat_specular_cube[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_cube);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_cube);
	glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
    glTranslatef(-4, 0, -2);
    glRotatef(90, 0, 0, 1);
    glColor3ub(0,255,20);
    Draw_base();
    glPopMatrix();
    
    float R = 4.2;
    double n = 50;
    glPushMatrix();
    glColor3ub(0,255,20);
    glTranslatef(0, 0, -3);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, 0.0);
    for (int i = 0; i < 50; i++) {
        glVertex2f(R * cos(2 * PI / n * i), R * sin(2 * PI / n * i));
    }
    glEnd();
    glPopMatrix();
    
    R = 1;
    glPushMatrix();
    glColor3ub(0,255,20);
    glTranslatef(6, 0, 10);
    glRotatef(90, 0, 1, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, 0.0);
    for (int i = 0; i < 50; i++) {
        glVertex2f(R * cos(2 * PI / n * i), R * sin(2 * PI / n * i));
    }
    glEnd();
    glPopMatrix();
    
    GLUquadricObj *cylinder;
    cylinder = gluNewQuadric();
    glPushMatrix();
    glTranslatef(6, 0, 10);
    glRotatef(90, 0, 1, 0);
    gluCylinder(cylinder, 1.0f, 1.0f, 2.6f, 32, 32);
    glPopMatrix();
    
    //    DrawPoints();
	glPopMatrix();/*重置模型视图矩阵*/
    
    //glEndList();
    return lid;
}


/*设置场景*/
void SetupRC()
{
    //	glClearColor(1.0f, 1.0f, 1.0f, 1.0f );/*白色背景*/
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );/*黑色背景*/
    
	/*开启光照*/
	glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
	/*开启颜色跟踪*/
	glEnable(GL_COLOR_MATERIAL);
    
	glEnable(GL_AUTO_NORMAL);/*自动生成法线*/
    
	pNurb_base = gluNewNurbsRenderer();/*设置Nurbs对象*/
    pNurb_pipe = gluNewNurbsRenderer();
    pNurb_lamp = gluNewNurbsRenderer();
    
    
	/*设置NURBS的属性*/
	gluNurbsProperty(pNurb_base, GLU_SAMPLING_TOLERANCE, 25.0f);/*设置采样容差，定义线框的精细程度*/
    gluNurbsProperty(pNurb_pipe, GLU_SAMPLING_TOLERANCE, 25.0f);/*设置采样容差，定义线框的精细程度*/
    gluNurbsProperty(pNurb_lamp, GLU_SAMPLING_TOLERANCE, 25.0f);/*设置采样容差，定义线框的精细程度*/
    
    
    drawlist = RenderScene();
}

/*窗口大小变化*/
void ChangeSize(int w, int h)
{
	/*防止被除数为0*/
	if(h == 0)
		h = 1;
    
	/*设置视口*/
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    
	/*透视投影*/
	gluPerspective (45.0f, (GLdouble)w/(GLdouble)h, 1.0, 40.0f);
    
	/*重置模型视图矩阵*/
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
	/*方便观察，平移*/
	glTranslatef (0.0f, 0.0f, -20.0f);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("NURBS Surface");
    glutDisplayFunc(redraw);
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(spec_keyboard);
    glutIdleFunc(idle);
	SetupRC();
	glutMainLoop();
    
	return 0;
}

#endif