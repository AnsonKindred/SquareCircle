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

    static Block** blocks;

    static pair<double, double>** secondaries, **last_secondaries;
    static pair<double, double>** primaries, **last_primaries;
    static pair<double, double>** temp;

    static int last_num_slices, num_slices, next_num_slices;
    static int primary_c, secondary_c, last_primary_c, last_secondary_c;
    static int layer_i, slice_i, block_layer, block_slice;
    static float layer_radius;
    static double last_slice_f;

public:

    static void init(int numLayers, float BlockSize);
    static void draw();

    static void dispose();

private:
    static void _constructCircle();
    static void _constructNextLayer();
    static void _constructNextSlice();
    static int _getNumSlices(int layer);

    static void _drawBoxes();
};

#endif
