add_library(whm-settings STATIC
    ./include/whm/settings/SettingsDay.hpp
    ./include/whm/settings/SettingsYear.hpp
    ./src/SettingsDay.cpp
    ./src/SettingsYear.cpp
)

target_include_directories(whm-settings
    PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(whm-settings
    PRIVATE
    Qt6::Core
    whm-types)
