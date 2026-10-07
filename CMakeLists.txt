cmake_minimum_required(VERSION 4.4.3)

project(electroplating-ovc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    csv_parser
    GIT_REPOSITORY https://github.com/vincentlaucsb/csv-parser.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(csv_parser)

find_package(OpenCV REQUIRED)

add_executable(electroplating-ovc main.cpp
    film_classify.hpp
    range.hpp
    film_classify.cpp
)

target_link_libraries(electroplating-ovc
    ${OpenCV_LIBS}
    csv
)

target_precompile_headers(electroplating-ovc PRIVATE <csv.hpp> <opencv2/opencv.hpp>)

include(GNUInstallDirs)
install(TARGETS electroplating-ovc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
