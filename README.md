# PlotLib
CMake library for real-time plotting.

## Usage

```c++
#include "PlotLib/implot_plotter.hpp"

#include "plib/util/util.hpp"
#include "plib/util/system.hpp"


using namespace status_utils;


int main(int argc, char* argv[])
{
    // Initialize Plotter
    ImPlotter::initialize();

    // Loop until user requests to quit
    while (System::is_alive())
    {
        // Sample Data

        // First plot is with sin
        ImPlotter::push_data(
            sin(System::get_time_since_start()), 
            "My First Line Plot"
        );

        // Second plot is with cos
        ImPlotter::push_data(
            cos(System::get_time_since_start()), 
            "My Second Line Plot"
        );

        // If the user requests to quit, then shutdown
        if (ImPlotter::update() == StatusCode::FAILED)
            System::shutdown();
    }

    // Shutdown Plotter
    ImPlotter::shutdown();

    return 0;
}
```