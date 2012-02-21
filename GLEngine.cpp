#ifdef OS_WINDOWS
#define CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>
 
// The most important line
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "GLEngine.h"

long GLEngine::startTime = 0;

/**
 * GLEngine
 *
 * Handles standard gl initialization stuff, kicks off the display loop
 *
 * @modified 5/28/2011
 */
void GLEngine::init()
{
    cout<<"Initializing GLEngine\n";

	startTime = clock();

    _initializeEventHandlers();
    _initializeGLEnvironment();

    //SquareCircle::init(100, .2);
}

void GLEngine::dispose()
{
    SquareCircle::dispose();
}

void GLEngine::_initializeGLEnvironment()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    //glutSetCursor(GLUT_CURSOR_NONE);
}

void GLEngine::_initializeEventHandlers()
{
    glutDisplayFunc(GLEngine::display);
	glutReshapeFunc(GLEngine::reshape);
}

void GLEngine::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    SquareCircle::draw();

    glutSwapBuffers(); 
	glFlush();
}

void GLEngine::reshape(int width, int height)
{
    float aspect = (float) width / height;
        
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
        
    //gluPerspective(45, aspect, .1, 1000.0);
    glOrtho(-10.0*aspect, 10.0*aspect, -10.0, 10.0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}
