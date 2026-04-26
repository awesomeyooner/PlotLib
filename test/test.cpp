#include <iostream>
#include <cstdio>
#include <type_traits>
#include <thread>

#include "plib/util/util.hpp"
#include "plib/util/system.hpp"

#include "PlotLib/implot_plotter.hpp"

#include "plib/math/motion/cubic_hermite_spline.hpp"

#include "plib/math/control/pid_controller.hpp"

#include "plib/i2c/i2c.hpp"
#include "plib/i2c/wire_device.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "implot/implot.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


using namespace status_utils;


void test_plotter()
{
    // Initialize Plotter
    ImPlotter::initialize();

    // Loop until user requests to quit
    while (System::is_alive())
    {
        // Sample Data
        ImPlotter::push_data(sin(System::get_time_since_start()), "My First Line Plot");
        ImPlotter::push_data(cos(System::get_time_since_start()), "My Second Line Plot");

        // If the user requests to quit, then shutdown
        if (ImPlotter::update() == status_utils::StatusCode::FAILED)
            System::shutdown();
    }

    // Shutdown Plotter
    ImPlotter::shutdown();
}


void run_user_input()
{
    std::thread(
        []()
        {
            while(System::is_alive())
            {
                StatusedValue<std::string> input = util::get_user_input_string("Type exit to quit the program >> ");

                if(!input.is_OK())
                {
                    System::shutdown();
                    break;
                }
            }

            Logger::info("Shutting down...");
        }
    ).detach();
}

int main(int argc, char* argv[])
{
    // run_user_input();
    test_plotter();

    return 0;
}