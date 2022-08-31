add_library(whm-types STATIC
    ./include/whm/types/Day.hpp
    ./include/whm/types/DayType.hpp
    ./include/whm/types/Date.hpp
    ./include/whm/types/HoursAndMinutes.hpp
    ./include/whm/types/Time.hpp
    ./include/whm/types/Weekday.hpp
    ./src/Day.cpp
    ./src/Date.cpp
    ./src/HoursAndMinutes.cpp
    ./src/Time.cpp
)

target_include_directories(whm-types
    PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(whm-types
    PRIVATE Qt6::Core)
