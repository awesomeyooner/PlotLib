#ifndef GNU_PLOTTER_HPP
#define GNU_PLOTTER_HPP

#include <string>
#include <iostream>
#include <unistd.h>
#include <cstdint>
#include <unistd.h>
#include <thread>
#include <ctime>
#include <mutex>
#include <fstream>
#include <chrono>

#include "util.hpp"
#include "status.hpp"
#include "system.hpp"

class GNUPlotter{

    public:

        // The GNUPlot pipe object
        static FILE* m_gnuplot;


        /**
         * @brief Initialized GNUPlot by setting the start time and opening the pipe
         * 
         * @param window_name `std::string` The name of the window
         * @param plot_name `std::string` The name of the plot
         * @param x_label `std::string` The name of the X-Axis
         * @param y_label `std::string` The name of the Y-Axis
         * @return `status_utils::StatusCode` Status code for if GNUPlot successfully initialized 
         */
        static status_utils::StatusCode initialize(
            std::string window_name = "Window", 
            std::string plot_name = "Plot", 
            std::string x_label = "X Axis", 
            std::string y_label = "Y Axis");


        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void push_data(std::string time, std::string data);


        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param data Y-Axis data
         */
        static void push_data(std::string data);


        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param data Y-Axis data
         */
        static void push_data(double data);
        

        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param data Y-Axis data
         */
        static void push_data(float data);


        /**
         * @brief Pushes new data to `data.txt`
         * 
         * @param data Y-Axis data
         */
        static void push_data(int data);


        /**
         * @brief Plots `data.txt` onto the screen
         * 
         * @param x_axis_width `double` The width for the X-Axis
         */
        static void plot_with_range(double x_axis_width = 10);


        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         * @param x_axis_width `double` The width for the X-Axis
         */
        static void plot_with_range(std::string time, std::string data, double x_axis_width = 10);


        /**
         * @brief Plots `data.txt` onto the screen
         * 
         */
        static void plot();


        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(std::string time, std::string data);


        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(std::string data);


        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(double data);


        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(float data);
        /**
         * @brief Pushes new data and plots `data.txt`
         * 
         * @param time X-Axis default (`time`)
         * @param data Y-Axis data
         */
        static void plot(int data);
 

        /**
         * @brief Closes the GNUPlot pipe
         * 
         */
        static void close();


    private:

    /*
    to plot multiple lines, you have to have multiple columsn in data.txt
        file << time << " " << data_a << " " << data_b << std::endl;

    then use
        fprintf(gnuplot, "plot 'data.txt' using 1:2 with lines title 'Data A', \
                'data.txt' using 1:3 with lines title 'Data B'\n");

    the 'using 1:2' 'using 1:3' means it's plotting column 1 against column 2, and 1 to 3. Since column 1 is time you are plotting column_n against time.
    */
        

}; // class GNUPlotter

#endif // GNU_PLOTTER_HPP