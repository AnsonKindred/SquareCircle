#ifndef CONTROLLER
#define CONTROLLER

#include "Base.h"
#include "GLEngine.h"

/**
 *
 * @author Anson Kindred
 */
class Controller
{

public:

    static void init();
    static void dispose();

    static void tick();

private:
    
    static void _initializeWindow();

    /*static void keyUp(unsigned char key, int x, int y);
    static void keyDown(unsigned char key, int x, int y);

    static void specialKeyUp(int key, int x, int y);
    static void specialKeyDown(int key, int x, int y);

    static void mouseDown(int button, int x, int y);
    static void mouseUp(int button, int x, int y);
    static void mouseDragged(int x, int y);
    static void handleMouseClick(int button, int state, int x, int y);*/
};

#endif
