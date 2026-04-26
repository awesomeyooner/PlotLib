#ifndef SCROLLING_BUFFER_HPP
#define SCROLLING_BUFFER_HPP


#include "implot/implot.h"

// The code below (the code inside the namespace) is from `implot_demo.cpp`, this code is not mine

// THIS CODE IS NOT MINE, I SIMPLY AM REWRITING IT
// ORIGINAL AUTHOR IS BY IMPLOT CREATORS


// Struct for Real-time plots
struct ScrollingBuffer
{
    int max_size;
    int offset;
    ImVector<ImVec2> data;

    ScrollingBuffer(int _max_size = 2000);

    void add_point(float x, float y);

    void erase();

    ImVec2& get_latest_point();

    ImPlotSpec get_spec();

}; // struct ScrollingBuffer


#endif // SCROLLING_BUFFER_HPP