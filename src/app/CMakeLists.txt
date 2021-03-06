add_executable(appWorkingHourManager
    src/Main.cpp
    src/Main/Backend.hpp
    src/Main/Backend.cpp    
    src/Main/Controller/ControllerDay.hpp
    src/Main/Controller/ControllerDay.cpp
    src/Main/Controller/ControllerMonth.hpp
    src/Main/Controller/ControllerMonth.cpp
    src/Main/Controller/ControllerWeek.hpp
    src/Main/Controller/ControllerWeek.cpp
    src/Main/Controller/ControllerYear.hpp
    src/Main/Controller/ControllerYear.cpp
    src/Main/FileIO/FileReader.hpp
    src/Main/FileIO/FileReader.cpp
    src/Main/FileIO/FileWriter.hpp
    src/Main/FileIO/FileWriter.cpp
)

qt_add_qml_module(appWorkingHourManager
    URI WorkingHourManager
    VERSION 1.0
    QML_FILES
    ressources/qml/Main.qml
    ressources/qml/Main/Year.qml
    ressources/qml/Main/Year/Month.qml
    ressources/qml/Main/Year/Month/Header.qml
    ressources/qml/Main/Year/Month/Week.qml
    ressources/qml/Main/Year/Month/Week/Day.qml
    ressources/qml/Main/Year/Month/Week/Day/HourMinInput.qml
    ressources/qml/Main/Year/Month/Week/Footer.qml
)

target_compile_definitions(appWorkingHourManager
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(appWorkingHourManager
    PRIVATE Qt6::Quick
    whm-types)
