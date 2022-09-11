add_executable(appWorkingHourManager
    src/Main.cpp
    src/Main/Backend.hpp
    src/Main/Backend.cpp    
)

qt_add_qml_module(appWorkingHourManager
    URI WorkingHourManager
    VERSION 1.0
    QML_FILES
    ressources/qml/Day/Day.qml
    ressources/qml/Month/Month.qml
    ressources/qml/Month/Month/HeaderMonth.qml
    ressources/qml/Month/Month/FooterMonth.qml
    ressources/qml/Types/DoubleInput.qml
    ressources/qml/Types/HourMinInput.qml
    ressources/qml/Week/Week.qml
    ressources/qml/Week/Week/FooterWeek.qml
    ressources/qml/Year/Year.qml
    ressources/qml/Year/Year/FooterYear.qml
    ressources/qml/Year/Year/SettingsYear.qml
    ressources/qml/Year/Year/SettingsYear/HeaderSettingsYear.qml
    ressources/qml/Year/Year/SettingsYear/FlextimeDays.qml
    ressources/qml/Year/Year/SettingsYear/SettingsDay.qml
    ressources/qml/Year/Year/SettingsYear/VacationDays.qml
    ressources/qml/Main.qml
    ressources/qml/Main/PopupEnterStartYear.qml
    ressources/qml/Main/Years.qml
)

target_compile_definitions(appWorkingHourManager
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)

target_link_libraries(appWorkingHourManager
    PRIVATE
    Qt6::Quick
    whm-day
    whm-file
    whm-month
    whm-types
    whm-week
    whm-year)
