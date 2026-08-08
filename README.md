# PlotLib
CMake library for real-time plotting. 

## Setup

The folder structure should resemble the following:

```c++
.
├── build
├── CMakeLists.txt
├── lib // Put here!
├── include
├── src
└── CMakeLists.txt
```

First, clone the repo into `/lib`

```bash
# Clone as standalone
$ git clone https://github.com/awesomeyooner/PlotLib.git && git submodule update --init --recursive

# Clone as submodule
$ git submodule add https://github.com/awesomeyooner/PlotLib.git && git submodule update --init --recursive
```

Also make sure that [plib](https://github.com/awesomeyooner/plib) is also put in `/lib`!

Then add these lines to your `CMakeLists.txt` file

```cmake
add_subdirectory(lib/PlotLib)
target_link_libraries( ${MAIN_EXEC} PlotLib )
```

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

} // end of "main(int, char*)
```