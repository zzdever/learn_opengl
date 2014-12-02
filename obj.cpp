
#define OBJ 0

#if OBJ

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <GLUT/GLUT.h>

using namespace std;

float fTranslate;
float fRotate;
float fScale = 1.0f;
bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

float eye[] = {0, 0, 8};
float center[] = {0, 0, 0};
GLfloat light_pos[] = {1, 1 , 1, 1};


int scale = 100;
int v_num=0, vn_num=0, f_num=0;
vector <vector <float>> v;
vector <vector <float>> vn;
vector<vector<int>> fv;
vector<vector<int>> fn;
vector <float> point;
vector <int> index1;
vector <int> index2;

void readFile() {
    GLfloat x, y, z;
    string type;
    string buffer;
    
    // file path
    ifstream fin("/Users/ying/Desktop/ReadOBJ/head.obj");
    if (!fin)
        exit(0);
    string newLine;
    while (getline(fin, newLine)) {
        if (newLine[0] == 'v') {
            if (newLine[1] == 'n') {        // vn x y z
                istringstream stream(newLine);
                stream >> type >> x >> y >> z;
                point.push_back(x);
                point.push_back(y);
                point.push_back(z);
                vn.push_back(point);
                //                cout << "vn[" << vn_num+1 << "] = (" << vn.at(vn_num).at(0) << ", "
                //                     <<  vn.at(vn_num).at(1) << ", " << vn.at(vn_num).at(2) << ")" << endl;
                vn_num++;
                for (int i=0; i<3; i++) {
                    point.pop_back();
                }
            }
            else {
                if (newLine[1] != 't') {    // v x y z, ignore vt
                    istringstream stream(newLine);
                    stream >> type >> x >> y >> z;
                    point.push_back(x);
                    point.push_back(y);
                    point.push_back(z);
                    v.push_back(point);
                    //                    cout << "v[" << v_num+1 << "] = (" << v.at(v_num).at(0) << ", "
                    //                         <<  v.at(v_num).at(1) << ", " << v.at(v_num).at(2) << ")" << endl;
                    v_num++;
                    for (int i=0; i<3; i++) {
                        point.pop_back();
                    }
                }
            }
        }
        else {
            if (newLine[0] == 'f') {        // f
                istringstream stream(newLine);
                stream >> type;             // eliminate f
                for (int i=0; i<3; i++) {
                    stream >> buffer;
                    int k;
                    int num1=0, num2=0;
                    for (k=0; buffer[k]!='/'; k++)
                        num1 = num1*10 + buffer[k] - 48;
                    index1.push_back(num1);
                    for (k++; buffer[k]!='/'; k++)  {
                        ;                   // ignore ft
                    }
                    for (k++; k<buffer.size(); k++)
                        num2 = num2*10 + buffer[k] - 48;
                    index2.push_back(num2);
                }
                fv.push_back(index1);
                fn.push_back(index2);
                //                cout << "fv[" << f_num+1 << "] = (" << fv.at(f_num).at(0) << " "
                //                <<  fv.at(f_num).at(1) << " " << fv.at(f_num).at(2) << ")" << " ";
                //                cout << "fn[" << f_num+1 << "] = (" << fn.at(f_num).at(0) << " "
                //                <<  fn.at(f_num).at(1) << " " << fn.at(f_num).at(2) << ")" << endl;
                f_num++;
                for (int i=0; i<3; i++) {
                    index1.pop_back();
                    index2.pop_back();
                }
            }
            else {                          // # || g || s || use...
                ;
                //                cout << newLine << endl;
            }
        }
    }
    fin.close();
    cout << "Program end" << endl;
}

void updateView(int width, int height)
{
    glViewport(0, 0, width, height);					// Reset The Current Viewport
    
    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix
    
    float whRatio = (GLfloat)width/(GLfloat)height;
    if (bPersp)
        gluPerspective(45.0f, whRatio, 0.1f, 100.0f);
    else
        glOrtho(-3 ,3, -3, 3, -100, 100);
    
    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
    if (height == 0)									// Prevent A Divide By Zero By
        height = 1;										// Making Height Equal One
    
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
        case 'q': {exit(0); break; }
        case 'p': {bPersp = !bPersp; break; }
        case ' ': {bAnim = !bAnim; break;}
        case 'o': {bWire = !bWire; break;}
            
        case 'a': {
            eye[0] += 0.1f;
            center[0] += 0.1f;
            break;
        }
        case 'd': {
            eye[0] -= 0.1f;
            center[0] -= 0.1f;
            break;
        }
        case 'w': {
            eye[1] -= 0.1f;
            center[1] -= 0.1f;
            break;
        }
        case 's': {
            eye[1] += 0.1f;
            center[1] += 0.1f;
            break;
        }
        case 'z': {
            eye[2] -= 1.0f;
            center[2] -= 0.1f;
            break;
        }
        case 'c': {
            eye[2] += 1.0f;
            center[2] += 0.1f;
            break;
        }
            // move diffusion light to left
        case 'j':{
            light_pos[0]-=0.2f;
            break;
        }
            // move diffusion light to right
        case 'l':{
            light_pos[0]+=0.2f;
            break;
        }
            // move diffusion light back
        case 'i':{
            light_pos[1]+=0.2f;
            break;
        }
            // move diffusion light forward
        case 'k':{
            light_pos[1]-=0.2f;
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
            // 缩小
        case 'g':{
            scale+=10;
            break;
        }
            // 放大
        case 'h':{
            scale-=10;
            break;
        }
    }
    
    updateView(wHeight, wWidth);
}

void redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();				// Reset The Current Modelview Matrix
    gluLookAt(eye[0], eye[1], eye[2],
              center[0], center[1], center[2],
              0, 1, 0);				// 场景（0，0，0）的视点中心 (0, 5, 50)，Y轴向上
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
    
    // diffusion light settings
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_EMISSION, white);
    glEnable(GL_LIGHT0);
    
    // draw the diffusion light
    glPushMatrix();
    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
    glMaterialfv(GL_FRONT, GL_EMISSION, white);
    glutSolidSphere(0.1, 300, 300);
    {
        float def_emi[] = {0.f, 0.f, 0.f, 1.f};
        glMaterialfv(GL_FRONT, GL_EMISSION, def_emi);
    }
    glPopMatrix();
    
    
    // draw model
    glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
    for (int i=0; i<f_num; i++) {
        glBegin(GL_TRIANGLES);
        glColor3f(0, 0, 0);
        glNormal3f(vn.at(fn.at(i).at(0) -1).at(0), vn.at(fn.at(i).at(0) -1).at(1), vn.at(fn.at(i).at(0) -1).at(2));
        glVertex3f(v.at(fv.at(i).at(0) -1).at(0)/scale, v.at(fv.at(i).at(0) -1).at(1)/scale, v.at(fv.at(i).at(0) -1).at(2)/scale);
        
        glNormal3f(vn.at(fn.at(i).at(1) -1).at(0), vn.at(fn.at(i).at(1) -1).at(1), vn.at(fn.at(i).at(1) -1).at(2));
        glVertex3f(v.at(fv.at(i).at(1) -1).at(0)/scale, v.at(fv.at(i).at(1) -1).at(1)/scale, v.at(fv.at(i).at(1) -1).at(2)/scale);
        
        glNormal3f(vn.at(fn.at(i).at(2) -1).at(0), vn.at(fn.at(i).at(2) -1).at(1), vn.at(fn.at(i).at(2) -1).at(2));
        glVertex3f(v.at(fv.at(i).at(2) -1).at(0)/scale, v.at(fv.at(i).at(2) -1).at(1)/scale, v.at(fv.at(i).at(2) -1).at(2)/scale);
        glEnd();
    }
    
    if (bAnim)
        fRotate += 3.0f;
    
    glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(640, 640);
    int windowHandle = glutCreateWindow("Simple GLUT App");
    
    // read obj file and save data to v, vn, fv, fn
    readFile();
    glutDisplayFunc(redraw);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    
    glutMainLoop();
    return 0;
}


#endif