cmake_minimum_required(VERSION 3.26)
project(BreakoutSFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(BreakoutSFML main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)

include_directories(SFML/include)
target_link_libraries(BreakoutSFML sfml-system sfml-window sfml-graphics sfml-audio)