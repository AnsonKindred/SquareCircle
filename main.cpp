#include "Base.h"
#include "Controller.h"

void main(int argc, char **argv)
{
    atexit(Controller::dispose);
    Controller::init();
}
