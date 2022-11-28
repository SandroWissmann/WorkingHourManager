add_library(whm-settings STATIC
    ./include/whm/settings/ControllerSettings.hpp
    ./include/whm/settings/Settings.hpp
    ./src/ControllerSettings.cpp
    ./src/Settings.cpp
)

target_include_directories(whm-settings
    PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(whm-settings
    PRIVATE
    Qt6::Core)
