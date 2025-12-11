cmake_minimum_required(VERSION 4.0)
project(philosophers)

set(CMAKE_CXX_STANDARD 20)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(PROJECT_HEADERS
        include/Philosopher.h
        include/Table.h
        include/Visualization.h
)

set(PROJECT_SOURCES
        main.cpp
        src/Philosopher.cpp
        src/Table.cpp
        src/Visualization.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(philosophers
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
)
target_link_libraries(philosophers PRIVATE ${CURSES_LIBRARIES})

