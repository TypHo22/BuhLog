cmake_minimum_required(VERSION 3.5)
project(Mandelbrot)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

find_library(Logger REQUIRED)


include_directories(Mandelbrot
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#create executable
add_executable(Mandelbrot main.cpp Mandelbrot.cpp
               )

target_link_libraries(Mandelbrot
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Logger)


