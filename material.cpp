
#define MATERIAL 0


#if MATERIAL
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

const double PI = 3.1415926;

float fTranslate;
float fRotate = 0.f;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};
float light_pos[] = {0.9, 0.9 , 1, 1};
GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 }; // the color of the diffusion light
GLfloat spot_angle = 0.f;   // the angle of the light cone of the spotlight
GLfloat spot_orientation[] = {0.f, -1.0f, 0.f};    // in which direction the spotlight is pointing

float mat_specular_tea[] = {0.f, 0.f, 0.f, 0.f};



void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}


void Draw_Table() // This function draws a triangle with RGB colors
{
	glPushMatrix();
	glTranslatef(0, 0, 4+1);
	glRotatef(90, 1, 0, 0);
    float mat_diffuse_tea[] = {0.85f, 0.65f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_tea);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_tea);
	glutSolidTeapot(1);
    {   // set the speculat to the default value
        float def_specular[] = {0.f, 0.f, 0.f, 0.f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, def_specular);
    }
	glPopMatrix();
    
	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
    float mat_diffuse_sur[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_sur);
	glutSolidCube(1.0);
	glPopMatrix();
    
	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
    float mat_diffuse_leg1[] = {0.f, 1.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_leg1);
	Draw_Leg();
	glPopMatrix();
    
	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
    float mat_diffuse_leg2[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_leg2);
	Draw_Leg();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
    float mat_diffuse_leg3[] = {0.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_leg3);
	Draw_Leg();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
    float mat_diffuse_leg4[] = {0.0f, 0.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_leg4);
	Draw_Leg();
	glPopMatrix();
    
}



void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport
    
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
    
	float whRatio = (GLfloat)width/(GLfloat)height;
	if (bPersp) {
		gluPerspective(45.0f, whRatio,0.1f,100.0f);
	    //glFrustum(-3, 3, -3, 3, 3,100);
	} else {
	    glOrtho(-3 ,3, -3, 3,-100,100);
	}
    
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


void key(unsigned char k, int x, int y)
{
	switch(k)
	{
        case 27:
        case 'q': {exit(0); break; }
        case 'p': {bPersp = !bPersp; break; }
            
        case ' ': {bAnim = !bAnim; break;}
        case 'o': {bWire = !bWire; break;}
            
            // move all to left
        case 'a': {
            eye[0] += 0.2f;
            center[0] += 0.2f;
            break;
        }
            // move all to right
        case 'd': {
            eye[0] -= 0.2f;
            center[0] -= 0.2f;
            break;
        }
            // move all up
        case 'w': {
            eye[1] -= 0.2f;
            center[1] -= 0.2f;
            break;
        }
            // move all down
        case 's': {
            eye[1] += 0.2f;
            center[1] += 0.2f;
            break;
        }
            // zoom in
        case 'z': {
            eye[2] -= 0.2f;
            center[2] -= 0.2f;
            break;
        }
            // zoom out
        case 'c': {
            eye[2] += 0.2f;
            center[2] += 0.2f;
            break;
        }
            // move diffusion light to left
        case 'j':{
            light_pos[0]-=0.2f;
            break;
        }
            // move spotlight direction to left
        case 'J':{
            spot_orientation[0] -= 0.1f;
            break;
        }
            // move diffusion light to right
        case 'l':{
            light_pos[0]+=0.2f;
            break;
        }
            // move spotlight direction to right
        case 'L':{
            spot_orientation[0] += 0.1f;
            break;
        }
            // move diffusion light back
        case 'i':{
            light_pos[1]+=0.2f;
            break;
        }
            // move spotlight direction back
        case 'I':{
            spot_orientation[2] -= 0.5f;
            break;
        }
            // move diffusion light forward
        case 'k':{
            light_pos[1]-=0.2f;
            break;
        }
            // move spotlight direction forward
        case 'K':{
            spot_orientation[2] += 0.5f;
            break;
        }
            // move diffusion light up
        case 'u':{
            light_pos[2]-=0.2f;
            break;
        }
            // move diffusion light down
        case 'n':{
            light_pos[2]+=0.2f;
            break;
        }
            // decrease red channel of the diffusion light
        case 'r':{
            light_color[0] -= 0.05f;
            break;
        }
            // increase red channel of the diffusion light
        case 'R':{
            light_color[0] += 0.05f;
            break;
        }
            // decrease green channel of the diffusion light
        case 'g':{
            light_color[1] -= 0.05f;
            break;
        }
            // increase red channel of the diffusion light
        case 'G':{
            light_color[1] += 0.05f;
            break;
        }
            // decrease blue channel of the diffusion light
        case 'b':{
            light_color[2] -= 0.05f;
            break;
        }
            // increase red channel of the diffusion light
        case 'B':{
            light_color[2] += 0.05f;
            break;
        }
            // increase the specular coefficient of the teaport
        case 't':{
            mat_specular_tea[0] += 0.1f;
            mat_specular_tea[1] += 0.1f;
            mat_specular_tea[2] += 0.1f;
            break;
        }
            // decrease the specular coefficient of the teaport
        case 'T':{
            mat_specular_tea[0] -= 0.1f;
            mat_specular_tea[1] -= 0.1f;
            mat_specular_tea[2] -= 0.1f;
            break;
        }
            // increase the angle of spotlight cone
        case 'y':{
            spot_angle += 0.4f;
            break;
        }
            // decrease the angle of spotlight cone
        case 'Y':{
            spot_angle -= 0.4f;
            break;
        }
        default:{
            break;
        }
	}
    
	updateView(wHeight, wWidth);
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
    
    // diffusion light settings
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    glLightfv(GL_LIGHT0, GL_EMISSION, light_color);
	glEnable(GL_LIGHT0);
    // draw the diffusion light
    glPushMatrix();
    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
    glMaterialfv(GL_FRONT, GL_EMISSION, light_color);
    glutSolidSphere(0.1, 300, 300);
    {
        float def_emi[] = {0.f, 0.f, 0.f, 1.f};
        glMaterialfv(GL_FRONT, GL_EMISSION, def_emi);
    }
    glPopMatrix();
    
    
    
    // spot light settings
    GLfloat spot_pos[] = {0.f, 1.5f, 0.0f, 0.5f};
    GLfloat spot_ambient[]= { 0.2, 0.2, 0.2, 0.3 };
    GLfloat spot_diffuse[]= { 0.9, 0.6, 0.0, 0.3 };
    GLfloat spot_specular[] = { 1.0, 0.6, 0.6, 1.0 };
    
    glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, spot_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, spot_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR,spot_specular);
    
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.2f);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, spot_angle);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_orientation);
    glEnable(GL_LIGHT1);
    
    
    // draw the spotlight
    glPushMatrix();
    glEnable(GL_BLEND); // make it transparent
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, spot_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, spot_diffuse);
    
    // position the spotlight to a correct place
    glTranslatef(spot_pos[0], spot_pos[1]+0.7, spot_pos[2]);
    glRotatef(atan(spot_orientation[0]/spot_pos[1])*180/PI, 0, 0, 1);
    glRotatef(atan(-spot_orientation[2]/spot_pos[1])*180/PI, 1, 0, 0);
    glTranslatef(0, -0.7, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(2*0.7*tan(spot_angle/180*PI), 0.7, 300, 300);
    {
        float def_emi[] = {0.f, 0.f, 0.f, 1.f};
        glMaterialfv(GL_FRONT, GL_EMISSION, def_emi);
    }
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glPopMatrix();
    
    
    //	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Table();						// Draw triangle
    
	if (bAnim) fRotate    += 0.5f;
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
