#include "Controller.h"

/**
 *
 * @author Anson Kindred
 */
void Controller::init()
{
    srand(time(0));
    cout<<"Welcome to whatever this is\n\n";
    _initializeWindow();
    GLEngine::init();
    
    SquareCircle::init(100, .2);
    glutIdleFunc(Controller::tick);

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
