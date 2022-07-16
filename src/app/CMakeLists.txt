add_executable(appWorkingHourManager
    src/Main.cpp
    src/Main/Backend.hpp
    src/Main/Backend.cpp
    src/Main/Backend/ControllerYear.hpp
    src/Main/Backend/ControllerYear.cpp
    src/Main/Backend/ControllerYear/ControllerMonth.hpp
    src/Main/Backend/ControllerYear/ControllerMonth.cpp
    src/Main/Backend/FileReader.hpp
    src/Main/Backend/FileReader.cpp
    src/Main/Backend/FileWriter.hpp
    src/Main/Backend/FileWriter.cpp
    src/Main/Common/ControllerWeek.hpp
    src/Main/Common/ControllerWeek.cpp
    src/Main/Common/ControllerWeek/ControllerDay.hpp
    src/Main/Common/ControllerWeek/ControllerDay.cpp
)

qt_add_qml_module(appWorkingHourManager
    URI WorkingHourManager
    VERSION 1.0
    QML_FILES
    ressources/qml/Main.qml
    ressources/qml/Main/Header.qml
    ressources/qml/Main/Week.qml
    ressources/qml/Main/Week/Day.qml
    ressources/qml/Main/Week/Footer.qml
    ressources/qml/Main/Week/Day/HourMinInput.qml
)

target_compile_definitions(appWorkingHourManager
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(appWorkingHourManager
    PRIVATE Qt6::Quick
    whm-types)
