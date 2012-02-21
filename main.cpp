#include "Base.h"
#include "Controller.h"

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
    atexit(Controller::dispose);
    Controller::init();
	return 1;
}
