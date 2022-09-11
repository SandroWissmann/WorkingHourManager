add_library(whm-day STATIC
    ./include/whm/day/Day.hpp
    ./include/whm/day/DayType.hpp
    ./include/whm/day/ControllerDay.hpp
    ./include/whm/day/ControllerSettingsDay.hpp
    ./include/whm/day/HelperDay.hpp
    ./include/whm/day/SettingsDay.hpp
    ./src/ControllerDay.cpp
    ./src/ControllerDay/ControllerDayState.hpp
    ./src/ControllerDay/ControllerDayStateFlextime.hpp
    ./src/ControllerDay/ControllerDayStateFlextime.cpp
    ./src/ControllerDay/ControllerDayStateHoliday.hpp
    ./src/ControllerDay/ControllerDayStateHoliday.cpp
    ./src/ControllerDay/ControllerDayStateIgnore.hpp
    ./src/ControllerDay/ControllerDayStateIgnore.cpp
    ./src/ControllerDay/ControllerDayStateParty.hpp
    ./src/ControllerDay/ControllerDayStateParty.cpp
    ./src/ControllerDay/ControllerDayStateSick.hpp
    ./src/ControllerDay/ControllerDayStateSick.cpp
    ./src/ControllerDay/ControllerDayStateVacation.hpp
    ./src/ControllerDay/ControllerDayStateVacation.cpp
    ./src/ControllerDay/ControllerDayStateParty.hpp
    ./src/ControllerDay/ControllerDayStateParty.cpp
    ./src/ControllerDay/ControllerDayStateWork.hpp
    ./src/ControllerDay/ControllerDayStateWork.cpp
    ./src/ControllerSettingsDay.cpp
    ./src/Daytype.cpp
    ./src/Day.cpp
    ./src/HelperDay.cpp
    ./src/SettingsDay.cpp
)

target_include_directories(whm-day
    PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(whm-day
    PRIVATE
    Qt6::Core
    whm-types)
