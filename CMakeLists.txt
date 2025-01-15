cmake_minimum_required(VERSION 3.13)
project(Driver)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/services
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/hardware
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/wireless
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/entertainment
    ${CMAKE_CURRENT_SOURCE_DIR}/drivers/weather
    /usr/include/nlohmann
)

add_definitions(-DDRIVERDIR=\"${BUILDDIR}/Driver\")

find_package(Boost 1.65 REQUIRED COMPONENTS system filesystem)
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/DriverConfiguration.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE -pthread -ldl lbaseservice ${Boost_LIBRARIES} -lcurl)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME}
    RUNTIME
        DESTINATION target
)
