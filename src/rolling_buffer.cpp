#include "PlotLib/rolling_buffer.hpp"


RollingBuffer::RollingBuffer(float _span, int _max_size)
{
    span = _span;
    max_size = _max_size;

    data.reserve(max_size);

} // end of "RollingBuffer(int)"


void RollingBuffer::add_point(float x, float y)
{
    double xmod = fmodf(x, span);

    if(!data.empty() && xmod < data.back().x)
        data.shrink(0);
    
    data.push_back(ImVec2(xmod, y));

} // end of "add_point"