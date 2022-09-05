cmake_minimum_required(VERSION 3.16)

project(WorkingHourManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Core Quick REQUIRED)

add_subdirectory(src/app)
add_subdirectory(src/file)
add_subdirectory(src/settings)
add_subdirectory(src/types)

