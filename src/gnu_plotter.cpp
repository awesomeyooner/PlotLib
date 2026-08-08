#include "PlotLib/gnu_plotter.hpp"

FILE* GNUPlotter::m_gnuplot;


status_utils::StatusCode GNUPlotter::initialize(
    std::string window_name,
    std::string plot_name,
    std::string x_label,
    std::string y_label)
{
    // Open the m_gnuplot pipe (it's a file) with write permissions
    m_gnuplot = popen("m_gnuplot", "w");

    // If the file / pipe failed to initialize, return FAILED status code
    if(!m_gnuplot)
        return status_utils::StatusCode::FAILED;

    // m_gnuplot settings...
    fprintf(m_gnuplot, "set terminal qt title '%s'\n", window_name.c_str());
    fprintf(m_gnuplot, "set title '%s'\n", plot_name.c_str());
    fprintf(m_gnuplot, "set xlabel '%s'\n", x_label.c_str());
    fprintf(m_gnuplot, "set ylabel '%s'\n", y_label.c_str());
    // fprintf(m_gnuplot, "set autoscale xy\n");
    fprintf(m_gnuplot, "set autoscale y\n");
    fprintf(m_gnuplot, "set grid\n");

    // Create a file to store the data
    std::ofstream file("data.txt");

    // Return OK
    return status_utils::StatusCode::OK;

} // end of "initialize"


void GNUPlotter::push_data(std::string time, std::string data)
{
    // Open `data.txt` without recreating it
    std::ofstream file("data.txt", std::ios::app);

    // Push time onto X-Axis and data onto Y-Axis
    file << time << " " << data << std::endl;

    // Close the file
    file.close();

} // end of "push_data"


void GNUPlotter::push_data(std::string data)
{
    // Automatically fill in time as the time since start
    push_data(util::to_string(System::get_epoch()), data);

} // end of "push_data"


void GNUPlotter::push_data(double data)
{
    // Automatically fill in time as time since start and convert data to string
    push_data(util::to_string(System::get_epoch()), util::to_string(data));

} // end of "push_data"


void GNUPlotter::push_data(float data)
{
    // Automatically fill in time as time since start and convert data to string
    push_data(util::to_string(System::get_epoch()), util::to_string(data));

} // end of "push_data"


void GNUPlotter::push_data(int data)
{
    // Automatically fill in time as time since start and convert data to string
    push_data(util::to_string(System::get_epoch()), util::to_string(data));

} // end of "push_


void GNUPlotter::plot_with_range(double x_axis_width)
{
    // Set the range from (now - width) to now
    std::string range_cmd = "set xrange [";
    range_cmd += std::to_string(System::get_epoch() - x_axis_width) + " : " + std::to_string(System::get_epoch()) + "]\n";
    fprintf(m_gnuplot, range_cmd.c_str());
    fprintf(m_gnuplot, "plot 'data.txt' using 1:2 with lines title 'Y Axis'\n");
    fflush(m_gnuplot);

} // end of "plot"


void GNUPlotter::plot_with_range(std::string time, std::string data, double x_axis_width)
{
    // Push the data
    push_data(time, data);

    // Plot the data
    plot_with_range(x_axis_width);

} // end of "plot"


void GNUPlotter::plot()
{
    // Send the plot command to the pipe
    fprintf(m_gnuplot, "plot 'data.txt' using 1:2 with lines title 'Y Axis'\n");
    fflush(m_gnuplot);

} // end of "plot"


void GNUPlotter::plot(std::string time, std::string data)
{
    // Push the data
    push_data(time, data);

    // Plot the data
    plot();

} // end of "plot"


void GNUPlotter::plot(std::string data)
{
    // Plot with time autofilled
    plot_with_range(util::to_string(System::get_epoch()), data);

} // end of "plot"


void GNUPlotter::plot(double data)
{
    plot(util::to_string(data));

} // end of "plot"


void GNUPlotter::plot(float data)
{
    plot(util::to_string(data));
    
} // end of "plot"


void GNUPlotter::plot(int data)
{
    plot(util::to_string(data));
    
} // end of "plot"


void GNUPlotter::close()
{
    pclose(m_gnuplot);
}