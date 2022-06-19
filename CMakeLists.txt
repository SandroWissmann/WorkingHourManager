cmake_minimum_required(VERSION 3.16)

project(WorkingHourManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Core Quick REQUIRED)

qt_add_executable(appWorkingHourManager
    backend/main.cpp
    backend/ControllerDay.hpp
    backend/ControllerDay.cpp
    backend/ControllerWeek.hpp
    backend/ControllerWeek.cpp
    backend/HoursAndMinutes.hpp
    backend/HoursAndMinutes.cpp
)

qt_add_qml_module(appWorkingHourManager
    URI WorkingHourManager
    VERSION 1.0
    QML_FILES
    frontend/main.qml
    frontend/Header.qml
    frontend/Day.qml
    frontend/HourMinInput.qml
    frontend/Footer.qml
    frontend/Week.qml
)

target_compile_definitions(appWorkingHourManager
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appWorkingHourManager
    PRIVATE Qt6::Quick)
