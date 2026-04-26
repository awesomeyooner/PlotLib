#ifndef IMPLOT_PLOTTER_HPP
#define IMPLOT_PLOTTER_HPP

#include <iostream>
#include <cstdio>
#include <unordered_map>
#include <functional>

#include "plib/util/system.hpp"
#include "plib/util/status.hpp"
#include "plib/util/logger.hpp"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_sdl2.h"
#include "imgui/backends/imgui_impl_opengl3.h"

#include "implot/implot.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <cmath>

// The code below (the code inside the namespace) is from `implot_demo.cpp`, this code is not mine

namespace ImGui{

// utility structure for realtime plot
struct ScrollingBuffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    ScrollingBuffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset  = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x,y));
        else {
            Data[Offset] = ImVec2(x,y);
            Offset =  (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset  = 0;
        }
    }
    ImVec2& getLatestPoint() {
        int size = Data.size();

        if (size < MaxSize)
            return Data[size - 1];
        else {
            return Data[Offset];
        }
    }
};
    
// utility structure for realtime plot
struct RollingBuffer {
    float Span;
    ImVector<ImVec2> Data;
    RollingBuffer() {
        Span = 10.0f;
        Data.reserve(2000);
    }
    void AddPoint(float x, float y) {
        float xmod = fmodf(x, Span);
        if (!Data.empty() && xmod < Data.back().x)
            Data.shrink(0);
        Data.push_back(ImVec2(xmod, y));
    }
};

} // namespace ImGui

/**
 * @brief Plotter class using ImPlot as the backend
 * 
 */
class ImPlotter{

    public:

        // Flags
        static ImPlotAxisFlags m_axis_flags;

        /**
         * @brief Initialize ImGui and its backends
         * 
         * @param window_name `std::string` Default "My Window" - The name of the window to use
         * @param verbose `bool` Flag for displaying error output 
         * @return `status_utils::StatusCode` OK if successful, FAILED otherwise 
         */
        static status_utils::StatusCode initialize(std::string window_name = "My Window", bool verbose = false);


        /**
         * @brief Display the plot
         * 
         * @return `status_utils::StatusCode` FAILED if exit request sent. OK otherwise 
         */
        static status_utils::StatusCode update();


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

        // SDL Window
        static SDL_Window* m_window;

        // OpenGL Context
        static SDL_GLContext m_gl_context;

        // History / Width of Plot
        static float m_history;

        // Map of Data Buffer
        static std::unordered_map<std::string, ImGui::ScrollingBuffer> m_data_map;
        // static ImGui::ScrollingBuffer m_data;


        /**
         * @brief Initialize the Buffer at the specified name in the data map
         * 
         * @param name `std::string` - The name of the plot
         * @return `ImGui::ScrollingBuffer` The buffer at `name`
         */
        static ImGui::ScrollingBuffer& initialize_data_map(std::string name);

}; // class ImPlotter




#endif // IMPLOT_PLOTTER_HPP