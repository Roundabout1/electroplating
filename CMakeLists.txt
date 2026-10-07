cmake_minimum_required(VERSION 4.4.3)

project(electroplating-ovc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    csv_parser
    GIT_REPOSITORY https://github.com/vincentlaucsb/csv-parser.git
    GIT_TAG        v5.4.0
)

FetchContent_Declare(
    inih
    GIT_REPOSITORY https://github.com/benhoyt/inih.git
    GIT_TAG        r62
)
FetchContent_MakeAvailable(
    csv_parser
    inih
)

find_package(OpenCV REQUIRED)

add_executable(electroplating-ovc main.cpp
    film_classify.hpp
    range.hpp
    film_classify.cpp
    config_reader.hpp
    config_reader.cpp
)

target_link_libraries(electroplating-ovc
    ${OpenCV_LIBS}
    csv
    inih
)

target_precompile_headers(electroplating-ovc
    PRIVATE
    <csv.hpp>
    <opencv2/opencv.hpp>
    <ini.h>
)

include(GNUInstallDirs)
install(TARGETS electroplating-ovc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
