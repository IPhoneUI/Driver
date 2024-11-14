cmake_minimum_required(VERSION 3.6)
project(Driver)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(${PROJECT_NAME} main.cpp)

install(TARGETS ${PROJECT_NAME}
    RUNTIME
        DESTINATION target
)

