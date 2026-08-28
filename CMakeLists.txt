cmake_minimum_required(VERSION 3.20)
project(pixel_anime_visualizer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG 4.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raygui)

add_executable(pixel_anime_visualizer
    src/main.cpp
    src/canvas.h
    src/palette.h
    src/palette.cpp
    src/generator.h
    src/generator.cpp
    src/eyes.cpp
    src/hair.cpp
    src/avatar.h
    src/avatar.cpp
    src/rng.h
    src/ui.cpp
    src/ui.h
    src/export.cpp
    src/export.h
)
target_link_libraries(pixel_anime_visualizer PRIVATE raylib)
target_include_directories(pixel_anime_visualizer PRIVATE ${raygui_SOURCE_DIR}/src)
