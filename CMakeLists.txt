cmake_minimum_required(VERSION 3.13)
project(Driver)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB_RECURSE SOURCES "./common/*.cpp")

add_executable(${PROJECT_NAME} main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/DriverConfiguration.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE -pthread -ldl ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME}
    RUNTIME
        DESTINATION target
)
