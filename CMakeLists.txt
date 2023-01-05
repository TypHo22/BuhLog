cmake_minimum_required(VERSION 3.5)
project(BuhLog)
set(CMAKE_CXX_STANDARD 17)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)
find_package(Threads REQUIRED)

include_directories(BuhLog
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

#create executable
add_executable(BuhLog main.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/source/BuhLog.cpp)

target_link_libraries(BuhLog Qt${QT_VERSION_MAJOR}::Core pthread)
