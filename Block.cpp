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

    // center point
    glColor3f(1, 1, 1);
    /*glPointSize(2);
    glBegin(GL_POINTS);
    glVertex2d(center.first, center.second);
    glEnd();*/

    _drawNeighborLines();
}

void Block::_drawNeighborLines()
{
    vector<Block*>::iterator it = neighbors.begin();
    glBegin(GL_LINES);
    for(;it != neighbors.end(); ++it)
    {
        glVertex2d(center.first, center.second);
        glVertex2d((*it)->center.first, (*it)->center.second);
    }
    glEnd();
}

void Block::finalize()
{
    _calculateCenter(&center);
    vector<Block*>::iterator it = neighbors.begin();
    for(;it != neighbors.end(); ++it) cout << (*it) << "\n";
    cout <<"\n";
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
    //if(neighbors.empty() || neighbors.back() != n)
    //{
        neighbors.push_back(n);
    //} 

    //vector<Block*>::iterator it = find (neighbors.begin(), neighbors.end(), n);

    //if(it == neighbors.end()) neighbors.push_back(n);
}