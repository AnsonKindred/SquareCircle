#ifndef GL_ENGINE
#define GL_ENGINE

#include "Base.h"
#include <GL/glut.h>
#include "SquareCircle.h"

/**
 * Main: GLEngine
 *
 * Initializes the window and starts the draw loop
 *
 * @modified 5/28/2011
 */
class GLEngine
{
    static long startTime;

public:

    static void display();
    static void init();
    static void reshape(int width, int height);
    
    static void dispose();

private:
    static void _initializeEventHandlers();
    static void _initializeGLEnvironment();
};

#endif
