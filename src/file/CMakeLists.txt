add_library(whm-file STATIC
    ./include/whm/file/FileReader.hpp
    ./include/whm/file/FileWriter.hpp
    ./src/FileReader.cpp
    ./src/FileWriter.cpp
)

target_include_directories(whm-file
    PUBLIC INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(whm-file
    PRIVATE
    Qt6::Core
    whm-day
    whm-types
    whm-year)
