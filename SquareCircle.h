#ifndef SQUARE_CIRCLE
#define SQUARE_CIRCLE

#include "Base.h"
#include "Block.h"
#include <GL/gl.h>

class SquareCircle
{

private:
    static int myNumLayers;
    static int myBoxesID;
    static float myBlockSize;
    static const int MAP_RES = 1000;

    static pair<double, double>** points;
    static Block** blocks;

    static pair<double, double>** secondaries, **last_secondaries;
    static pair<double, double>** primaries, **last_primaries;
    static pair<double, double>** temp;

public:

    static void init(int numLayers, float BlockSize);
    static void draw();

    static void dispose();

private:
    static void _constructCircle();
    static int _getNumSlices(int layer);

    static void _drawBoxes();
};

#endif
