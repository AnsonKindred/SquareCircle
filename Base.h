/*
 *  Some standard necessary files
 */

#ifndef BASE_HEADER
#define BASE_HEADER

#define OS_WINDOWS

// Hide the console window
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>
#include <map>
#include <list>
#include <iterator>
#include <time.h>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include "VectorUtil.h"

#ifdef OS_WINDOWS
    #include <windows.h>
#endif

#define TAU 6.283185307179586
#define PI 3.141592653589793

const int X = 0;
const int Y = 1;
const int Z = 2;

using namespace std;

#endif