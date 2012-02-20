#ifndef BLOCK_HEADER
#define BLOCK_HEADER

#include "VectorUtil.h"
#include <gl/GL.h>

class Block
{
public:

    Block();
    ~Block();

    triple<float> color;
    pair<double, double>** points[2];
    int row_lengths[2];
    pair<double, double> center;
    double area;

    vector<Block*> neighbors;
    void finalize();

    void draw();

    void addPoint(int layer, pair<double, double>* point);
    void addNeighbor(Block* point);

private:
    void _calculateCenter(pair<double, double>* result);
    float _calculateArea();

    void _drawNeighborLines();

};

#endif