#include "PlotLib/scrolling_buffer.hpp"


ScrollingBuffer::ScrollingBuffer(int _max_size)
{
    max_size = _max_size;

} // end of "ScrollingBuffer(int)"


void ScrollingBuffer::add_point(float x, float y)
{
    // If the data hasn't wrapped around yet
    // Then simply append
    if(data.size() < max_size)
        data.push_back(ImVec2(x, y));
    // If the data did wrap 
    // then we set the value at index of `offset`
    // and incrememnt offset with modulus applied
    else
    {
        data[offset] = ImVec2(x, y);
        offset = (offset + 1) % max_size;
    }

} // end of "add_point"


void ScrollingBuffer::erase()
{
    if(data.size() > 0)
    {
        data.shrink(0);
        offset = 0;
    }

} // end of "erase()"


ImVec2& ScrollingBuffer::get_latest_point()
{
    int size = data.size();

    // If we haven't wrapped around
    // Then simply return the last data point
    if(size < max_size)
        return data[size - 1];
    // If we have wrapped around
    // Then return the offset point
    else
        return data[offset];

} // end of "get_latest_point()"


ImPlotSpec ScrollingBuffer::get_spec()
{
    ImPlotSpec spec;
    spec.Offset = offset;
    spec.Stride = 2 * sizeof(float);

    return spec;
    
} // end of "get_spec()"