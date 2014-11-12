#define TEA 0


#if TEA

#include <stdlib.h>
#include <stdio.h>
#include <GLUT/glut.h>

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

bool teaAnim = false;   // if the teaport rotates
float fteaportRotate = 0;   // rotation angle of the teaport

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

float teaport[] = {0, 0, 1};


void Draw_Scene()
{
	glPushMatrix();
	glTranslatef(0+teaport[0], 0+teaport[1], 4+teaport[2]); // move the teaport to a position
	glRotatef(90, 1, 0, 0);
    glRotatef(fteaportRotate, 0, 1, 0); // rotate the teaport
	glutSolidTeapot(1);
	glPopMatrix();
    
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();
    
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();
    
	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();
    
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport
    
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
    
	float whRatio = (GLfloat)width/(GLfloat)height;
	
	if (bPersp){
        gluPerspective(45.0f,whRatio,0.1f,100.0f);
	}
	else
	    glOrtho(-3 ,3, -3, 3,-100,100);
    
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
    
	wHeight = height;
	wWidth = width;
    
	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
        case 27: case 'q': {exit(0); break; }
        case 'p': {bPersp = !bPersp; updateView(wHeight, wWidth);break; }
            
        case ' ': {bAnim = !bAnim; break;}
        case 'o': {bWire = !bWire; break;}
            
        case 'a': { // move left
            eye[0] += 0.2f;
            center[0] += 0.2f;
            break;
        }
        case 'd': { // move right
            eye[0] -= 0.2f;
            center[0] -= 0.2f;
            break;
        }
        case 'w': { // move up
            eye[1] -= 0.2f;
            center[1] -= 0.2f;
            break;
        }
        case 's': { // move down
            eye[1] += 0.2f;
            center[1] += 0.2f;
            break;
        }
        case 'z': { // move forward
            eye[2] -= 0.2f;
            center[2] -= 0.2f;
            break;
        }
        case 'c': { // move backward
            eye[2] += 0.2f;
            center[2] += 0.2f;
            break;
        }
            
            //teaport
        case 'l': { // right
            teaport[0] += 0.2f;
            if (teaport[0] >= 2.f) teaport[0] = 2.f;
            break;
        }
        case 'j': { // move left
            teaport[0] -= 0.2f;
            if (teaport[0] <= -2.f) teaport[0] = -2.f;
            break;
        }
        case 'i': { // move backward
            teaport[1] -= 0.2f;
            if (teaport[1] <= -2.f) teaport[1] = -2.f;
            break;
        }
        case 'k': { // move forward
            teaport[1] += 0.2f;
            if (teaport[1] >= 1.f) teaport[1] = 1.f;
            break;
        }
        case 'e': { // rotate
            teaAnim = !teaAnim;
            break;
        }
	}
}


void redraw()
{
    
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix
    
	gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上
    
	if (bWire) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
    
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};
    
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);
    
    //	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();						// Draw Scene
    
	if (bAnim) fRotate    += 0.5f;
    if (teaAnim) fteaportRotate += 1.f; // change the rotation angle
    
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple GLUT App");
    
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key);
	glutIdleFunc(idle);
    
	glutMainLoop();
	return 0;
}




#endif
