cmake_minimum_required(VERSION 3.5)
project(BuhLog)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Concurrent REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Concurrent REQUIRED)
find_package(Threads REQUIRED)

include_directories(BuhLog
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

#create executable
add_executable(BuhLog main.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/BuhLog.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/LogManager.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/ThreadPool.cpp
                 )

target_link_libraries(BuhLog Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Concurrent pthread)

