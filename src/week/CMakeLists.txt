add_library(whm-week STATIC
    ./include/whm/week/ControllerWeek.hpp
    ./src/ControllerWeek.cpp
)

target_include_directories(whm-week
    PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(whm-week
    PRIVATE
    Qt6::Core
    whm-day
    whm-types)
