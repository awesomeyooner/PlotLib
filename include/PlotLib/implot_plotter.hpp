#ifndef IMPLOT_PLOTTER_HPP
#define IMPLOT_PLOTTER_HPP


#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <functional>

#include "plib/util/system.hpp"
#include "plib/util/status.hpp"
#include "plib/util/logger.hpp"

#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include "PlotLib/rolling_buffer.hpp"
#include "PlotLib/scrolling_buffer.hpp"

#include "implot.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cmath>


/**
 * @brief Plotter class using ImPlot as the backend
 * 
 */
class ImPlotter{

    public:

        // Flags
        static inline ImPlotAxisFlags m_axis_flags = ImPlotAxisFlags_PanStretch; // ImPlotAxisFlags_AutoFit;

        /**
         * @brief Initialize ImGui and its backends
         * 
         * @param window_name `std::string` Default "My Window" - The name of the window to use
         * @param verbose `bool` Flag for displaying error output 
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        static status_utils::StatusCode init(std::string window_name = "My Window", bool verbose = false);


        /**
         * @brief Display the plot
         * 
         * @param add_input `std::function<void()> = nullptr` Used to add any forms of input like
         * ```
         * ImGui::SliderFloat("Hello World!", &my_var, MIN, MAX, "%.1f units");
         * ImGui::InputFloat("Hello World!", &my_var, STEP_CLICK, STEP_HOLD, "%.1f units");
         * ```
         * This gets called before `ImPlot::begin`
         * 
         * @return `status_utils::StatusCode` FAILED if exit request sent. OK otherwise 
         */
        static status_utils::StatusCode update(std::function<void()> add_input = nullptr);


        /**
         * @brief Display a plot with a fixed data buffer
         * 
         * @param data_x `std::vector<double>&` The X-Axis data buffer
         * @param data_y `std::vector<double>&` The Y-Axis data buffer
         * @return `status_utils::StatusCode` FAILED if exit request send. OK otherwise 
         */
        static status_utils::StatusCode plot_fixed(std::vector<double>& data_x, std::vector<double>& data_y);


        /**
         * @brief Shorthand for creating custom plots that aren't in this class
         * 
         * @param runnable `std::function<status_utils::StatusCode(SDL_Window*, SDL_GLContext&)>` Custom Runnable
         * @return `status_utils::StatusCode` FAILED if exit request sent. OK if successful, ERROR otherwise 
         */
        static status_utils::StatusCode plot_custom(std::function<status_utils::StatusCode(SDL_Window*, SDL_GLContext&)> runnable);


        /**
         * @brief Push new data to the buffer
         * 
         * @param x_data `double` The X Axis value. Typically Time
         * @param y_data `double` The Y Axis value. Your actual data 
         * @param name `std::string` Default `"My Line Plot"` - The name of the plot
         */
        static void push_data(double x_data, double y_data, std::string name = "My Line Plot");


        /**
         * @brief Push new data to the buffer with time automatically filled in
         * 
         * @param data `double` The data to plot
         * @param name `std::string` Default `"My Line Plot"` - The name of the plot
         */
        static void push_data(double data, std::string name = "My Line Plot");


        /**
         * @brief Shutdown ImGui and the backends
         * 
         */
        static void shutdown(); 

        
        /**
         * @brief Get the SDL_Window* object
         * 
         * @return `SDL_Window*` The SDL Window Pointer 
         */
        static SDL_Window* get_window();

        
        /**
         * @brief Get the SDL_GLContext& object
         * 
         * @return `SDL_GLContext&` The SDL OpenGL Context reference 
         */
        static SDL_GLContext& get_context();


    private:

        /**
         * @brief Initialize the Buffer at the specified name in the data map
         * 
         * @param name `std::string` - The name of the plot
         * @return `ScrollingBuffer` The buffer at `name`
         */
        static ScrollingBuffer& init_data_map(std::string name);

        /**
         * @brief Displays the current X and Y values for each buffer as text
         * 
         * @return `status_utils::StatusCode` Always OK 
         */
        static status_utils::StatusCode display_buffer_XY();

        /**
         * @brief Plots the entire buffer data
         * 
         * @return `status_utils::StatusCode` Always OK 
         */
        static status_utils::StatusCode plot_buffer_data();

        /**
         * @brief Start an ImGUI window and read SDL events. Returns `StatusCode::FAILED` if SDL event
         * was EXIT. OK otherwise
         * 
         * @param name `std::string` The name of the ImGUI window
         * @return `status_utils::StatusCode` OK if successful, FAILED if request was sent to exit 
         */
        static status_utils::StatusCode begin_window(std::string name = "Plotter");

        /**
         * @brief End the ImGUI window
         * 
         * @return `status_utils::StatusCode` Always returns OK (for now) 
         */
        static status_utils::StatusCode end_window();

        // SDL Window
        static inline SDL_Window* m_window = nullptr;

        // OpenGL Context
        static inline SDL_GLContext m_gl_context = NULL;

        // History / Width of Plot
        static inline float m_history = 10;

        // Map of Data Buffer
        static inline std::unordered_map<std::string, ScrollingBuffer> m_data_map;
        // static ScrollingBuffer m_data;

}; // class ImPlotter




#endif // IMPLOT_PLOTTER_HPP