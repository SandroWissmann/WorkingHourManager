add_library(whm-year STATIC
    ./include/whm/year/ControllerYear.hpp
    ./include/whm/year/ControllerSettingsYear.hpp
    ./include/whm/year/HelperControllerYear.hpp
    ./include/whm/year/SettingsYear.hpp
    ./src/ControllerYear.cpp
    ./src/ControllerSettingsYear.cpp
    ./src/HelperControllerYear.cpp
    ./src/SettingsYear.cpp
)

target_include_directories(whm-year
    PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(whm-year
    PRIVATE
    Qt6::Core
    whm-day
    whm-week
    whm-month
    whm-types)
