add_executable(appWorkingHourManager
    src/Main.cpp
    src/Main/Backend.hpp
    src/Main/Backend.cpp    
)

qt_add_qml_module(appWorkingHourManager
    URI WorkingHourManager
    VERSION 1.0
    QML_FILES
    ressources/qml/Types/DoubleInput.qml
    ressources/qml/Types/HourMinInput.qml
    ressources/qml/Main.qml
    ressources/qml/Main/PopupEnterStartYear.qml
    ressources/qml/Main/Years.qml
    ressources/qml/Main/Years/Year.qml
    ressources/qml/Main/Years/Year/FooterYear.qml
    ressources/qml/Main/Years/Year/Month.qml
    ressources/qml/Main/Years/Year/Month/HeaderMonth.qml
    ressources/qml/Main/Years/Year/Month/Week.qml
    ressources/qml/Main/Years/Year/Month/FooterMonth.qml
    ressources/qml/Main/Years/Year/Month/Week/Day.qml
    ressources/qml/Main/Years/Year/Month/Week/FooterWeek.qml
    ressources/qml/Main/Years/Year/SettingsYear.qml
    ressources/qml/Main/Years/Year/SettingsYear/HeaderSettingsYear.qml
    ressources/qml/Main/Years/Year/SettingsYear/FlextimeDays.qml
    ressources/qml/Main/Years/Year/SettingsYear/SettingsDay.qml
    ressources/qml/Main/Years/Year/SettingsYear/VacationDays.qml
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
