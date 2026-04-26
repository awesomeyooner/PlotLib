// https://github.com/epezent/implot/blob/master/implot_demo.cpp
// https://traineq.org/implot_demo/src/implot_demo.html

#include "PlotLib/implot_plotter.hpp"


using namespace status_utils;


status_utils::StatusCode ImPlotter::initialize(std::string window_name, bool verbose)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
    {
        // If verbose is enabled, log the error
        if(verbose)
            Logger::error(std::string("ImPlotter::initialize - ") + SDL_GetError());

        return status_utils::StatusCode::FAILED;
    }

    // OpenGL Settings
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create the window
    m_window = SDL_CreateWindow(
        window_name.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600, // Width, Height
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    // Create Context
    m_gl_context = SDL_GL_CreateContext(m_window);
    SDL_GL_MakeCurrent(m_window, m_gl_context);
    SDL_GL_SetSwapInterval(1); // vsync

    // Initialize ImGui
    if(verbose)
        IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui::StyleColorsDark();

    // Initialize SDL2 and OpenGL for ImGui
    ImGui_ImplSDL2_InitForOpenGL(m_window, m_gl_context);
    ImGui_ImplOpenGL3_Init("#version 330");

    return StatusCode::OK;

} // end of "initialize"


status_utils::StatusCode ImPlotter::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            return status_utils::StatusCode::FAILED;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Center the plot
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    
    // Make the plot size equal to the main window size
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

    // Start the plot. Make the widget fixed in size
    ImGui::Begin("Plotter", nullptr, ImGuiWindowFlags_NoResize);

    // Display Each Axis Value for each Data Buffer
    for(const auto& pair : m_data_map)
    {
        const char* name = pair.first.c_str();

        ImGui::ScrollingBuffer data = pair.second;

        if(ImGui::TreeNodeEx(name))
        {
            std::string x_text = "X Axis: " + util::to_string(data.getLatestPoint().x);

            ImGui::BulletText(x_text.c_str());

            std::string y_text = "Y Axis: " + util::to_string(data.getLatestPoint().y);

            ImGui::BulletText(y_text.c_str());

            ImGui::TreePop();
        }
    }

    // Add Slider for changing the history
    ImGui::SliderFloat("History", &m_history, 1, 30, "%.1f s");

    if(ImPlot::BeginPlot("My Plot", ImVec2(-1, -1)))
    {
        
        ImPlot::SetupAxes("Time (s)", "Y Axis", m_axis_flags, m_axis_flags);
        ImPlot::SetupAxisLimits(ImAxis_X1, System::get_time_since_start() - m_history, System::get_time_since_start(), ImGuiCond_Always);
        // ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);

        // For every pair in the map, plot the data buffer
        for(const auto& pair : m_data_map)
        {
            // The name of the plot
            const char* name = pair.first.c_str();

            // The data buffer
            ImGui::ScrollingBuffer data = pair.second;

            ImPlotSpec spec;
            spec.Offset = data.Offset;
            spec.Stride = 2 * sizeof(float);

            ImPlot::PlotLine(
                name,
                &data.Data[0].x,
                &data.Data[0].y,
                data.Data.size(),
                spec
            );

            // // Plot it
            // ImPlot::PlotLine(
            //     name, 
            //     &data.Data[0].x, 
            //     &data.Data[0].y, 
            //     data.Data.size(), 
            //     0, // Flags
            //     data.Offset, 
            //     2*sizeof(float)
            // );
        }
            
        ImPlot::EndPlot();
    }

    ImGui::End();

    ImGui::Render();

    glViewport(0, 0, 800, 600);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_window);

    return status_utils::StatusCode::OK;

} // end of "update"


status_utils::StatusCode ImPlotter::plot_fixed(std::vector<double>& data_x, std::vector<double>& data_y)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            return status_utils::StatusCode::FAILED;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Center the plot
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    
    // Make the plot size equal to the main window size
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

    // Start the plot. Make the widget fixed in size
    ImGui::Begin("Plotter", nullptr, ImGuiWindowFlags_NoResize);

    if(ImPlot::BeginPlot("My Plot", ImVec2(-1, -1)))
    {
        
        ImPlot::SetupAxes("X Axis", "Y Axis", m_axis_flags, m_axis_flags);
        // ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 1);

        ImPlot::PlotLine("My Plot", data_x.data(), data_y.data(), data_x.size());
            
        ImPlot::EndPlot();
    }

    ImGui::End();

    ImGui::Render();

    glViewport(0, 0, 800, 600);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_window);

    return status_utils::StatusCode::OK;

} // end of "update"


status_utils::StatusCode ImPlotter::plot_custom(std::function<status_utils::StatusCode(SDL_Window*, SDL_GLContext&)> runnable)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            return status_utils::StatusCode::FAILED;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Center the plot
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    
    // Make the plot size equal to the main window size
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Always);

    // Start the plot. Make the widget fixed in size
    ImGui::Begin("Plotter", nullptr, ImGuiWindowFlags_NoResize);

    runnable(m_window, m_gl_context);

    ImGui::End();

    ImGui::Render();

    glViewport(0, 0, 800, 600);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(m_window);

    return status_utils::StatusCode::OK;

} // end of "plot_custom"


void ImPlotter::push_data(double x_data, double y_data, std::string name)
{
    // Push new data to the buffer at the specified name
    initialize_data_map(name).AddPoint(x_data, y_data);

} // end of "push_data"


void ImPlotter::push_data(double data, std::string name)
{
    // Fill in X Axis value as time since start
    push_data(System::get_time_since_start(), data, name);
    
} // end of "push_data"


void ImPlotter::shutdown()
{
    // Cleanup everything
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();

    SDL_GL_DeleteContext(m_gl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

} // end of "shutdown"


SDL_Window* ImPlotter::get_window()
{
    return m_window;

} // end of "get_window"


SDL_GLContext& ImPlotter::get_context()
{
    return m_gl_context;

} // end of "get_context"


ImGui::ScrollingBuffer& ImPlotter::initialize_data_map(std::string name)
{
    // Add a new ScrollingBuffer to the map. `try_emplace` already protects
    // From the key already existing

    ImGui::ScrollingBuffer buffer;

    m_data_map.try_emplace(name, buffer);

    // Return the buffer at `name`
    return m_data_map.at(name);

} // end of "initialize_data_map"


SDL_Window* ImPlotter::m_window = nullptr;
SDL_GLContext ImPlotter::m_gl_context = NULL;

float ImPlotter::m_history = 10;
std::unordered_map<std::string, ImGui::ScrollingBuffer> ImPlotter::m_data_map;
ImPlotAxisFlags ImPlotter::m_axis_flags = ImPlotAxisFlags_PanStretch; // ImPlotAxisFlags_AutoFit;