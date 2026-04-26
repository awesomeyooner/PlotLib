cmake_minimum_required(VERSION 3.12)

# CHANGE ME
project( PlotLib LANGUAGES C CXX )

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED CONFIG)

set(IMGUI_FILES

    # ImGUI Core
    lib/imgui/imgui_demo.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/imgui.cpp

    # ImGUI Backend
    lib/imgui/backends/imgui_impl_opengl3.cpp
    lib/imgui/backends/imgui_impl_sdl2.cpp
)

set(IMPLOT_FILES

    # ImPlot Core
    lib/implot/implot_demo.cpp
    lib/implot/implot_items.cpp
    lib/implot/implot.cpp
)

set(SRC_FILES
    src/implot_plotter.cpp
)

# === Main Library ===
add_library(${PROJECT_NAME}
    ${IMGUI_FILES}
    ${IMPLOT_FILES}
    ${SRC_FILES}
)

# Add plib
add_subdirectory(lib/plib)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:lib>

        # You don't have to include these few lines below btw
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui>
        $<INSTALL_INTERFACE:lib/imgui>
        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui/backends>
        # $<INSTALL_INTERFACE:lib/imgui/backends>
        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/implot>
        # $<INSTALL_INTERFACE:lib/implot>

        ${SDL2_INCLUDE_DIRS}
)

# This is for this project specifically
target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    SDL2::SDL2 
    plib
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

# Add the test executables only if this project is at the top level (aka not used with add_subdirectory)
if(PROJECT_IS_TOP_LEVEL)
    add_executable(Test test/test.cpp)
    target_link_libraries(Test PRIVATE 
        ${PROJECT_NAME}
    )
endif()