add_library(whm-month STATIC
    ./include/whm/month/ControllerMonth.hpp
    ./src/ControllerMonth.cpp
)

target_include_directories(whm-month
    PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(whm-month
    PRIVATE
    Qt6::Core
    whm-day
    whm-week
    whm-types)
