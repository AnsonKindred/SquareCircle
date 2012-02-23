#include "Controller.h"

/**
 *
 * @author Anson Kindred
 */
void Controller::init()
{
    srand(time(0));
    _initializeWindow();
    GLEngine::init();
    glutIdleFunc(Controller::tick);

    cout<<"Welcome to whatever this is\n\n";
    SquareCircle::init(10, 1);

	glutMainLoop();
}

void Controller::_initializeWindow()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); 
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH)-20, glutGet(GLUT_SCREEN_HEIGHT)-96); 
	glutInitWindowPosition(10, 35); 
	glutCreateWindow("SquareCircle");
}

void Controller::tick()
{
    
    // .. Do whatever here

    glutPostRedisplay();
}

void Controller::dispose()
{
    GLEngine::dispose();
	#ifdef OS_WINDOWS
    _CrtDumpMemoryLeaks();
	#endif
}
