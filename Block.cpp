#define CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>
 
// The most important line
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#include "Block.h"

Block::Block() : color(0, 0, 0)
{
    color.x = (rand()%256)/256.f;
    color.y = (rand()%256)/256.f;
    color.z = (rand()%256)/256.f;
    points[0] = NULL;
    points[1] = NULL;
    row_lengths[0] = 0;
    row_lengths[1] = 0;
    area = 0;
}

Block::~Block()
{
    delete[] points[0];
    delete[] points[1];
}

void Block::draw()
{
    glColor3f(color.x, color.y, color.z);
    glBegin(GL_LINE_LOOP);
    int c = 0;
    glLineWidth(2);
    for(; c < 2; c++)
    {
        if(c % 2 == 0) {
            for(int p = 0; p < row_lengths[c]; p++)
            {
                glVertex2d(points[c][p]->first, points[c][p]->second);
            }
        }
        else {
            for(int p =row_lengths[c]-1; p >= 0; p--)
            {
                glVertex2d(points[c][p]->first, points[c][p]->second);
            }
        }
    }
    glEnd();

    _drawNeighborLines();
}

void Block::_drawNeighborLines()
{
    glColor3f(color.x-.2, color.y-.2, color.z-.2);
    glLineWidth(1);

    float offset_x = 0;(rand()%100)/500.0;
    float offset_y = 0;(rand()%100)/500.0;
    vector<Block*>::iterator it = neighbors.begin();
    glBegin(GL_LINES);
    for(;it != neighbors.end(); ++it)
    {
        glVertex2d(center.first+offset_x, center.second+offset_y);
        glVertex2d((*it)->center.first+offset_x, (*it)->center.second+offset_y);
    }
    glEnd();
}

void Block::finalize()
{
    _calculateCenter(&center);
}

void Block::_calculateCenter(pair<double, double>* result)
{
    if(area == 0) _calculateArea();

    result->first = 0;
    result->second = 0;

    for(int i = 0; i < row_lengths[0]-1; i++) {
        result->first  += (points[0][i]->first  + points[0][i+1]->first)  * (points[0][i]->first*points[0][i+1]->second - points[0][i+1]->first*points[0][i]->second);
        result->second += (points[0][i]->second + points[0][i+1]->second) * (points[0][i]->first*points[0][i+1]->second - points[0][i+1]->first*points[0][i]->second);
    }
    result->first  += (points[0][row_lengths[0]-1]->first  + points[1][row_lengths[1]-1]->first)  * (points[0][row_lengths[0]-1]->first*points[1][row_lengths[1]-1]->second - points[1][row_lengths[1]-1]->first*points[0][row_lengths[0]-1]->second);
    result->second += (points[0][row_lengths[0]-1]->second + points[1][row_lengths[1]-1]->second) * (points[0][row_lengths[0]-1]->first*points[1][row_lengths[1]-1]->second - points[1][row_lengths[1]-1]->first*points[0][row_lengths[0]-1]->second);
    for(int i = row_lengths[1]-1; i > 0; i--) {
        result->first  += (points[1][i]->first  + points[1][i-1]->first)  * (points[1][i]->first*points[1][i-1]->second - points[1][i-1]->first*points[1][i]->second);
        result->second += (points[1][i]->second + points[1][i-1]->second) * (points[1][i]->first*points[1][i-1]->second - points[1][i-1]->first*points[1][i]->second);
    }
    
    result->first /= 6*area;
    result->second /= 6*area;
}

float Block::_calculateArea()
{
    double sum = 0;
    for(int i = 0; i < row_lengths[0]-1; i++) {
        sum += points[0][i]->first*points[0][i+1]->second - points[0][i+1]->first*points[0][i]->second;
    }
    sum += points[0][row_lengths[0]-1]->first*points[1][row_lengths[1]-1]->second - points[1][row_lengths[1]-1]->first*points[0][row_lengths[0]-1]->second;
    for(int i = row_lengths[1]-1; i > 0; i--) {
        sum += points[1][i]->first*points[1][i-1]->second - points[1][i-1]->first*points[1][i]->second;
    }
    area = .5 * sum;
    return area;
}

void Block::addPoint(int layer, pair<double, double>* point)
{
    if(row_lengths[layer] == 0) {
        points[layer][row_lengths[layer]] = point;
        row_lengths[layer]++;
    }
    else if(points[layer][row_lengths[layer]-1]->first != point->first || 
        points[layer][row_lengths[layer]-1]->second != point->second) 
    {
        points[layer][row_lengths[layer]] = point;
        row_lengths[layer]++;
    }
}

void Block::addNeighbor(Block* n)
{
    neighbors.push_back(n);
}

void Block::setCirclePosition(int layer_i, int slice_i)
{
    this->layer_i = layer_i;
    this->slice_i = slice_i;
}