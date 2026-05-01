cmake_minimum_required(VERSION 3.16)
project(ShopFilter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${CMAKE_SOURCE_DIR}/src)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/scraper.cpp
    src/product.cpp
    src/filter.cpp
    src/platform/demo_scraper.cpp
)

set(HEADERS
    src/mainwindow.h
    src/scraper.h
    src/product.h
    src/filter.h
    src/platform/iscraper.h
    src/platform/demo_scraper.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    nlohmann_json
)
