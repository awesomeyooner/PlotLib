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
#include <iostream>
#include <cstdio>

#include "plib/util/system.hpp"

#include "PlotLib/scrolling_buffer.hpp"
#include "PlotLib/implot_plotter.hpp"


using namespace status_utils;


float input = 0;
float slider = 0;

float input_on_change = 0;


int main(int argc, char* argv[])
{
    // Initialize Plotter
    ImPlotter::init();

    // Loop until user requests to quit
    while (System::is_alive())
    {
        // Sample Data
        ImPlotter::push_data(sin(System::get_epoch()), "My First Line Plot");
        ImPlotter::push_data(cos(System::get_epoch()), "My Second Line Plot");

        // Add additional inputs
        std::function<void()> add_inputs = []()
        {
            ImGui::InputFloat("My Input", &input, 0.1f, 1.0f, "%.3f s");
            ImGui::SliderFloat("My Slider", &slider, 0, 100, "%.3f units");

            // Run code upon pressing enter
            if(ImGui::InputFloat("My Input Trigger", &input, 0.1f, 1.0f, "%.3f s", ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::cout << "Input Changed!" << std::endl;
            }
        };

        // If the user requests to quit, then shutdown
        if (ImPlotter::update(add_inputs) == status_utils::StatusCode::FAILED)
            System::shutdown();
    }

    // Shutdown Plotter
    ImPlotter::shutdown();

    return 0;
}
```