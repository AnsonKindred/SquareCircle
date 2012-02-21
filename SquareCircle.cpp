#ifdef OS_WINDOWS
#define CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>
 
// The most important line
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "SquareCircle.h"

int SquareCircle::myNumLayers = 0;
float SquareCircle::myBlockSize = 0;
int SquareCircle::myBoxesID = 0;
pair<double, double>** SquareCircle::points;
Block** SquareCircle::blocks;
    
pair<double, double>** SquareCircle::secondaries, **SquareCircle::last_secondaries;
pair<double, double>** SquareCircle::primaries, **SquareCircle::last_primaries;
pair<double, double>** SquareCircle::temp;

void SquareCircle::dispose()
{
    for(int l = 0; l < myNumLayers; l++)
    {
        delete[] points[l];
    }
    delete[] points;
    
    for(int l = 0; l < myNumLayers-1; l++)
    {
        delete[] blocks[l];
    }
    delete[] blocks;

    glDeleteLists(myBoxesID, 1);
}

void SquareCircle::init(int numLayers, float blockSize)
{
    myNumLayers = numLayers;
    myBlockSize = blockSize;

    clock_t start_time = clock();
    cout << "Compiling geometry...\n";

    _constructCircle();
    
    cout << "done: "<<(clock()-start_time)<<"ms\n";
    //exit(0);
    myBoxesID = glGenLists(1);
    glNewList(myBoxesID, GL_COMPILE);
    _drawBoxes();
    glEndList();
}

void SquareCircle::_constructCircle()
{
    int last_num_slices = 1;
    int num_slices = _getNumSlices(0);
    int next_num_slices = _getNumSlices(1);

    int primary_c = 0;
    int secondary_c = 0;
    int last_primary_c = 0;
    int last_secondary_c = 0;

    // Our goal is to fill this array
    blocks = new Block*[myNumLayers-1];

    // Points holds exactly one pointer to each point so the memory can be freed later. Other than that it's actually not even necessary...
    // I think if I could free the memory from the blocks themselves correctly it would be unnecessary
    points = new pair<double, double>*[myNumLayers];

    // First layer, just a single origin point
    points[0] = new pair<double, double>[1];

    // The all important arrays that allow this magic to work. 
    // Primaries store only the primary points for each layer. Secondaries store points calculated at the current layer but with 
    // the next layer's number of slices. This essentually projects the primaries from the next layer back onto the current layer 
    // resulting in the desired box shape.
    last_primaries = new pair<double, double>*[num_slices+1];
    last_secondaries = new pair<double, double>*[next_num_slices+1];

    // First layer, edge case, every point is at the origin
    last_primaries[last_primary_c++] = &points[0][0];
    // secondary points for the next layer, also all at the origin, but they need to be in this list for referencing on the next layer
    for(int i = 0; i <= next_num_slices; i++) last_secondaries[last_secondary_c++] = &points[0][0];
    // last primary point
    last_primaries[last_primary_c++] = &points[0][0];

    last_num_slices = num_slices;
    num_slices = next_num_slices;
    
    // The rest of the layers
    blocks[0] = new Block[num_slices/4];
    for(int layer = 1; layer < myNumLayers; layer++)
    {
        int block_layer = layer-1;
        next_num_slices = _getNumSlices(layer+1);
        
        secondaries = new pair<double, double>*[next_num_slices/4+1];
        primaries = new pair<double, double>*[num_slices/4+1];

        float radius = layer*myBlockSize;

        last_primary_c = 0;
        last_secondary_c = 0;
        primary_c = 0;
        secondary_c = 0;
        int point_c = 0;

        // Start a new layer of points and blocks
        points[layer] = new pair<double, double>[num_slices/4+next_num_slices/4+2];
        if(layer < myNumLayers -1) {
            // Gotta allocate one block layer ahead so that they can be linked to as neighbors
            blocks[block_layer+1] = new Block[next_num_slices/4];
        }

        // First slice of each layer, edge case
        points[layer][point_c].first = 0.f;
        points[layer][point_c].second = radius;

        // It's also always a primary and secondary
        primaries[primary_c] = &points[layer][point_c];
        secondaries[secondary_c] = &points[layer][point_c];
        point_c++; primary_c++; secondary_c++;

        double last_slice_f = 0;
        for(int slice = 1; slice <= num_slices/4; slice++)
        {
            double slice_f = (double)slice/num_slices;
            int block_slice = slice-1;

            // Let's build a block
            Block* block = &blocks[block_layer][block_slice];
            block->setCirclePosition(block_layer, block_slice);

            if(slice > 1) {
                // Each block also needs to know about its neighbors. Here we add the previous block from this layer to the list of neighbors 
                // for this block, and vice versa. The logic regarding neighbors is carefully designed to avoid adding duplicate neighbors to the list.
                block->addNeighbor(&blocks[block_layer][block_slice-1]);
                blocks[block_layer][block_slice-1].addNeighbor(block);
            }

            // This is the index of the last slice in the previous layer that is before this slice
            int underlap_slice_i_end = (int)(slice_f*last_num_slices);

            // Each box is made of two rows of points, one made from the points from the previous layer, and one made from this layer's primaries.
            // Here we initilize the first row. The array size is the number of new intermediates + 2 for the end points
            block->points[0] = new pair<double, double>*[underlap_slice_i_end-last_primary_c+1+2]; 
            // Add the first points to the block, it was already calculated as a secondary on the previous layer
            block->addPoint(0, last_secondaries[last_secondary_c++]);

            // Adding all the primaries from the previous layer that fall on the back row of the block
            for(; last_primary_c <= underlap_slice_i_end ; last_primary_c++) 
            {
                block->addPoint(0, last_primaries[last_primary_c]);
                if(layer > 1 && last_primary_c > 0) {
                    // Each one of those points represents a neighbor that needs to be added to the list
                    block->addNeighbor(&blocks[block_layer-1][last_primary_c-1]);
                    blocks[block_layer-1][last_primary_c-1].addNeighbor(block);
                }
            }

            // Add the last point in the back row of the box
            block->addPoint(0, last_secondaries[last_secondary_c]);

            //-----------------------------------------------------------------

            // This is the index of the last slice in the next layer that is before this slice
            int overlap_slice_i_end = (int)(slice_f*next_num_slices);
            
            // Initilize the second row of the block
            block->points[1] = new pair<double, double>*[overlap_slice_i_end-secondary_c+1+2];
            // Add the previous primary point
            block->addPoint(1, primaries[slice-1]);
            
            // Adding all the secondaries from the next layer that fall on the outer row of this box
            double overlap_slice_f = (double)secondary_c/next_num_slices;
            while(secondary_c <= overlap_slice_i_end)
            {
                // Gotta calculate these points, yay circle math
                points[layer][point_c].first = (double)(radius*sin(overlap_slice_f*TAU));
                points[layer][point_c].second = (double)(radius*cos(overlap_slice_f*TAU));
                
                secondaries[secondary_c] = &points[layer][point_c];

                block->addPoint(1, &points[layer][point_c]);

                if(layer < myNumLayers-1 && secondary_c > 0 && slice_f != overlap_slice_f) {
                    // Same deal with the neighbors
                    block->addNeighbor(&blocks[block_layer+1][secondary_c-1]);
                    blocks[block_layer+1][secondary_c-1].addNeighbor(block);
                }
                
                point_c++;
                secondary_c++;
                overlap_slice_f = (double)secondary_c/next_num_slices;
            }
            
            // And here is the actual primary point
            points[layer][point_c].first = (double)(radius*sin(slice_f*TAU));
            points[layer][point_c].second = (float)(radius*cos(slice_f*TAU));

            primaries[primary_c] = &points[layer][point_c];
            block->addPoint(1, &points[layer][point_c]);

            block->finalize();

            point_c++;
            primary_c++;
            last_slice_f = slice_f;
        }
        last_num_slices = num_slices;
        num_slices = next_num_slices;

        delete[] last_secondaries;
        last_secondaries = secondaries;

        delete[] last_primaries;
        last_primaries = primaries;
    }

    delete[] primaries;
    delete[] secondaries;
}

/**
 * This formula is a good half of the magic.
 * At it's heart this is just layer*TAU so that the number of slices in each 
 * layer increases at the same rate as the circumference.
 * The "+.5" is so that when we do the cast to int we round to the closest
 * int instead of just rounding down. This makes almost no difference visually
 * but it's technically more correct :)
 * The "/4 * 4" is a little integer division trick to ensure that the result is
 * a multiple of 4. This is what allows us to generate only a quarter of the 
 * blocks and mirror the result and have everything always work out.
 */
int SquareCircle::_getNumSlices(int layer)
{
    if(layer == 0) return 1;
    return ((int) (layer * TAU+.5))/4 * 4;
}

void SquareCircle::draw()
{
    if(myBoxesID != 0) {
        glPushMatrix();
        glCallList(myBoxesID);
        glRotatef(90, 0, 0, 1);
        glCallList(myBoxesID);
        glRotatef(90, 0, 0, 1);
        glCallList(myBoxesID);
        glRotatef(90, 0, 0, 1);
        glCallList(myBoxesID);
        glPopMatrix();
    }
    
    /*for(unsigned int layer = 0; layer < point.size(); layer++)
    {
        glBegin(GL_LINE_LOOP);
        for(unsigned int slice = 0; slice < point[layer].size(); slice++)
        {
            pair<float, float>* cur_point = point[layer][slice];
            glVertex2f(cur_point->first, cur_point->second);
        }
        glEnd();
    }*/
}

void SquareCircle::_drawBoxes()
{
    for(int l = 0; l < myNumLayers-1; l++)
    {
        int num_slices = _getNumSlices(l+1);
        for(int b = 0; b < num_slices/4; b++)
        {
            blocks[l][b].draw();
        }
    }
}
