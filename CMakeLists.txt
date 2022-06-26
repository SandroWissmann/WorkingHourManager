cmake_minimum_required(VERSION 3.16)

project(WorkingHourManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Core Quick REQUIRED)

qt_add_executable(appWorkingHourManager
    Backend/Main.cpp
    Backend/Main/Backend.hpp
    Backend/Main/Backend.cpp
    Backend/Main/Backend/
    Backend/Main/Backend/ControllerYear.hpp
    Backend/Main/Backend/ControllerYear.cpp
    Backend/Main/Backend/ControllerYear/ControllerMonth.hpp
    Backend/Main/Backend/ControllerYear/ControllerMonth.cpp
    Backend/Main/Common/ControllerWeek.hpp
    Backend/Main/Common/ControllerWeek.cpp
    Backend/Main/Common/ControllerWeek/ControllerDay.hpp
    Backend/Main/Common/ControllerWeek/ControllerDay.cpp
    Backend/Main/Common/ControllerWeek/HoursAndMinutes.hpp
    Backend/Main/Common/ControllerWeek/HoursAndMinutes.cpp
)

qt_add_qml_module(appWorkingHourManager
    URI WorkingHourManager
    VERSION 1.0
    QML_FILES
    Frontend/Main.qml
    Frontend/Main/Header.qml
    Frontend/Main/Week.qml
    Frontend/Main/Week/Day.qml
    Frontend/Main/Week/Footer.qml
    Frontend/Main/Week/Day/HourMinInput.qml
)

target_compile_definitions(appWorkingHourManager
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appWorkingHourManager
    PRIVATE Qt6::Quick)
