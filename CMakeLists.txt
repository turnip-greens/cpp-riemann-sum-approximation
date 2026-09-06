# Init cmake and setup project
cmake_minimum_required(VERSION 3.14)
project(cpp-riemann-sum-approximation VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# FetchContent gets external dependencies
include(FetchContent)

if(EMSCRIPTEN)
    # WebAssembly (Emscripten) mode
    # Use Emscripten's built-in GLFW port instead of fetching desktop GLFW source
    set(EMSCRIPTEN_FLAGS 
        "-sUSE_GLFW=3"
        "-sMIN_WEBGL_VERSION=2"
        "-sMAX_WEBGL_VERSION=2"
    )
else()
    # Desktop mode: Fetch GLFW from GitHub
    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
    )
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glfw)

    # Cross-platform OpenGL target lookup for desktop
    find_package(OpenGL REQUIRED)
endif()

# Get ImGui from github
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        v1.90.8
)
FetchContent_MakeAvailable(imgui)

# Load ImGui src static library
add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include ImGui directories
target_include_directories(imgui PUBLIC 
    ${imgui_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}/backends
)

if(EMSCRIPTEN)
    target_compile_options(imgui PUBLIC ${EMSCRIPTEN_FLAGS})
    target_link_options(imgui PUBLIC ${EMSCRIPTEN_FLAGS})
else()
    target_link_libraries(imgui PUBLIC glfw OpenGL::GL)
endif()

# add executable 
add_executable(${PROJECT_NAME} main.cpp)

# link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

if(EMSCRIPTEN)
    # Pass Emscripten flags and generate index.html alongside .wasm
    target_compile_options(${PROJECT_NAME} PRIVATE ${EMSCRIPTEN_FLAGS})
    target_link_options(${PROJECT_NAME} PRIVATE ${EMSCRIPTEN_FLAGS})
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()