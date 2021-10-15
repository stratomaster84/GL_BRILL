#include <iostream>
using namespace std;

//----------------------------------------------
#define _USE_MATH_DEFINES
#include <math.h>

#include <GL/Bible/OpenGLSB.h>	// System and OpenGL Stuff
#include <GL/Bible/GLTools.h>   // GLTools
#include <GL/Bible/loadtga.c>
#include "brilliant.h"

//using namespace std

// Brilliant Material
GLfloat mat_specular[] = {1,1,1,1};
GLfloat mat_amb_dif_red[] = {0,0.7,0.7,0.7};
GLfloat mat_amb_dif_white[] = {1,1,1,0.3};
GLfloat mat_shininess = 100;
// Light
GLfloat light_specular[] = {1,1,1,1};
GLfloat light_diffuse[] = {1,1,1,1};
GLfloat light_ambient[] = {0,0,0,1};
GLfloat glob_amb[] = {0.2,0.2,0.2,1};
//Light Position
GLfloat light_rad = 20;
GLfloat light_pos[] = {0,20,0,1};
// Textures
GLuint theTextures[2];
// Temporary images
GLbyte *pBytes;

PFNGLACTIVETEXTUREPROC   glActiveTexture = NULL;


// Rotation amounts

static GLfloat theta[] = {0,0};
static GLfloat theta2 = 0.0f;
static GLfloat zRot = 3.0f;

unsigned int _W= 1024,_H = 768;
int X00,Y00;

GLfloat   MaxZRot;
GLfloat   MinZRot;

bool Is_Red = true;
bool Is_Transparent = false;
bool Is_Background = false;
bool Is_Light = true;

GLint iWidth, iHeight, iComponents;
GLenum eFormat;

_Bril_Model my_brilliant;

void Draw_Background();

void ChangeSize(int w, int h)
    {
    GLfloat fAspect;
    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w/(GLfloat)h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90.0f, fAspect, 1.0, 40.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }


void SetupRC()
    {
        glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");

                light_pos[0] = light_rad*sin(theta2*M_PI/180);
                light_pos[2] = light_rad*cos(theta2*M_PI/180);
                glLightfv(GL_LIGHT0,GL_POSITION,light_pos);

        glLightModelfv(GL_LIGHT_MODEL_AMBIENT,glob_amb);
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
        glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
        glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);

        glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
        glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);

        glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

        glEnable(GL_LIGHT0);
        glEnable(GL_DEPTH_TEST);

        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        //glDisable(GL_CULL_FACE);
        glClearColor(0.0f, 0.0f, 0.0f, 0.3f );

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glGenTextures(1,theTextures);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        /*glBindTexture(GL_TEXTURE_2D,theTextures[0]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);
        pBytes = gltLoadTGA(EDGE_PIC, &iWidth, &iHeight, &iComponents, &eFormat);
        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

    glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,theTextures[1]);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);
        pBytes = gltLoadTGA(BACK_PIC, &iWidth, &iHeight, &iComponents, &eFormat);
        glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBytes);
        free(pBytes);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);


        my_brilliant.MadeGLList(9,1.5,1.0,0.5,1,1.0);
        MinZRot = 5.0f;
        MaxZRot = 2.0f;

    }

// Respond to arrow keys
void SpecialKeys(int key, int x, int y)
    {

    if(key == GLUT_KEY_F1)
                Is_Red = !Is_Red;

    else if(key == GLUT_KEY_F4)
        Is_Transparent = !Is_Transparent;
    else if(key == GLUT_KEY_F2)
        Is_Background = !Is_Background;
    else if(key == GLUT_KEY_F3)
        Is_Light = !Is_Light;
    else if(key == GLUT_KEY_LEFT){
                theta2 -= 5.0f;
                theta2 = (GLfloat)((const int)theta2 % 360);
                light_pos[0] = light_rad*sin(theta2*M_PI/180);
                light_pos[2] = light_rad*cos(theta2*M_PI/180);
                glLightfv(GL_LIGHT0,GL_POSITION,light_pos);

        }
    else if(key == GLUT_KEY_RIGHT){
                theta2 += 5.0f;
                theta2 = (GLfloat)((const int)theta2 % 360);
                light_pos[0] = light_rad*sin(theta2*M_PI/180);
                light_pos[2] = light_rad*cos(theta2*M_PI/180);
                glLightfv(GL_LIGHT0,GL_POSITION,light_pos);

        }
    else if(key == GLUT_KEY_F12)
        exit(0);

    glutPostRedisplay();
}
void KeyFunc(unsigned char key, int x, int y)
    {

    if(key == '+' && zRot>MaxZRot)
        zRot -= 0.1f;
    else if(key == '-' && zRot<MinZRot)
        zRot += 0.1f;

    glutPostRedisplay();
}

void mouse(int a,int b,int x,int y){
        if (a == GLUT_LEFT_BUTTON && b == GLUT_DOWN){
                X00 = x;
                Y00 = y;
        }
}
void mot(int x,int y){

        theta[1] += 360*(double)(x-X00)/(double)_W;
        theta[0] += 360*(double)(y-Y00)/(double)_H;

        theta[0] = (GLfloat)((const int)theta[0] % 360);
        theta[1] = (GLfloat)((const int)theta[1] % 360);

        X00 = x;
        Y00 = y;

        glutPostRedisplay();

}

// Called to draw scene
void RenderScene(void)
    {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(Is_Background)
        Draw_Background();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,theTextures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,theTextures[1]);
        if(!Is_Transparent)
                glDisable(GL_TEXTURE_2D);
        else
                glEnable(GL_TEXTURE_2D);
        if(Is_Red)
                glColor4fv(mat_amb_dif_red);
        else
                glColor4fv(mat_amb_dif_white);
        if(Is_Light)
                glEnable(GL_LIGHTING);
        else
                glDisable(GL_LIGHTING);


       glPushMatrix();
        glTranslatef(0.0f, 0.0f, -zRot);
        glRotatef(theta[0],1,0,0);
        glRotatef(theta[1],0,1,0);

        my_brilliant.Draw_Brill();

    glPopMatrix();
    glutSwapBuffers();
}

int main(int argc, char *argv[])
    {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(_W, _H);
    glutCreateWindow("My Brilliant");
    glutFullScreen();
    glutReshapeFunc(ChangeSize);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(KeyFunc);
    glutDisplayFunc(RenderScene);
    glutMouseFunc(mouse);
    glutMotionFunc(mot);
    SetupRC();
    glutMainLoop();

    return 0;
    }


void Draw_Background(){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0f, 1.0f, 0.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_LIGHTING);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,theTextures[1]);
    glActiveTexture(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);

    // No depth buffer writes for background
    glDepthMask(GL_FALSE);
    glColor4fv(mat_amb_dif_white);
    // Background image
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(1.0f, 0.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(1.0f, 1.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(0.0f, 1.0f);
    glEnd();
    glDepthMask(GL_TRUE);

    // Back to 3D land
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
