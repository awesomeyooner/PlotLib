#ifndef ROLLING_BUFFER_HPP
#define ROLLING_BUFFER_HPP


#include <cmath>

#include "implot.h"

// The code below (the code inside the namespace) is from `implot_demo.cpp`, this code is not mine

// THIS CODE IS NOT MINE, I SIMPLY AM REWRITING IT
// ORIGINAL AUTHOR IS BY IMPLOT CREATORS


// Struct for Real-time plots
struct RollingBuffer
{
    int max_size;
    float span;

    ImVector<ImVec2> data;

    RollingBuffer(float span, int max_size = 2000);

    void add_point(float x, float y);

}; // struct ScrollingBuffer


#endif // SCROLLING_BUFFER_HPP