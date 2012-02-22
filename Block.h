#ifndef BLOCK_HEADER
#define BLOCK_HEADER

#include "VectorUtil.h"
#include <GL/gl.h>

class Block
{
public:

    Block();
    Block(int layer, int slice);
    ~Block();

    triple<float> color;
    pair<double, double>** points[2];
    int row_lengths[2];
    pair<double, double> center;
    double area;
    int layer, slice;
    bool isLastBlockInRow;
    bool isFirstBlockInRow;
    bool isOnLastLayer;

    vector<Block*> neighbors;
    void finalize();

    void draw();

    void addPoint(int layer, pair<double, double>* point);
    void addNeighbor(Block* point);

    void setCirclePosition(int layer, int slice);

private:
    void _calculateCenter(pair<double, double>* result);
    float _calculateArea();
    void _drawNeighborLines();

};

#endif
