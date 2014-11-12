#define TABLE 0



#if TABLE

#include <GLUT/GLUT.h>
#include <stdlib.h>

float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f


void Draw_Cylinder()
{
    // initialize a cylinder object
    GLUquadricObj *cylinder_obj;
    cylinder_obj=gluNewQuadric();
    
    // double loop to get [-1,-1] [-1,1] [1,-1] and [1,1]
    for (int i=-1; i<2; i+=2) {
        for (int j=-1; j<2; j+=2) {
            glPushMatrix();
            glTranslatef(i*0.25f, -0.05f, j*0.25f);             //move close to the corner
            glRotatef(90, 1.0f, 0, 0);                  // draw vertical to x-z plane
            gluCylinder(cylinder_obj, 0.05, 0.05, 0.5, 30, 5);          // draw a cylinder
            glPopMatrix();
        }
    }
    
}


void Draw_Table() // This function draws a triangle with RGB colors
{
    // loop for 6 times to draw the table surface and the 5 little cuboids
    for(int i=0;i<6;i++){
        glPushMatrix();
        glScalef(2*(6-i)/6.0,0.2,2*(6-i)/6.0);          // different sizes
        glTranslatef(0, 0.5*i, 0);              // move up along the y axis
        glutWireCube(0.5);              // draw a cuboid
        glPopMatrix();
    }
    
    Draw_Cylinder();
    
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
    
	glViewport(0,0,width,height);						// Reset The Current Viewport
    
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
    
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
    
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void idle()
{
	glutPostRedisplay();
}

void redraw()
{
    // If want display in wireframe mode
    //	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix
	
	glPushMatrix();
    glTranslatef(-2.0f, 0.0f,-6.0f);		// Place the triangle Left
    glTranslatef(0.0f, fTranslate, 0.0f);	// Translate in Y direction
    Draw_Table();						// Draw triangle
	glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
    glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
    Draw_Table();						// Draw triangle
	glPopMatrix();
    
    
	glPushMatrix();
    glTranslatef(2.0f,0.0f,-6.0f);			// Place the triangle Right
    glScalef(fScale, fScale, fScale);		// Scale with the same value in x,y,z direction
    Draw_Table();						// Draw triangle
	glPopMatrix();
    
    
	fTranslate += 0.005f;
	fRotate    += 0.5f;
	fScale     -= 0.005f;
    
	if(fTranslate > 0.5f) fTranslate = 0.0f;
	if(fScale < 0.5f)     fScale     = 1.0f;
	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(640,480);
    int windowHandle = glutCreateWindow("Simple GLUT App");
    
    glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
    
    glutMainLoop();
    
    return 0;
}

#endif

