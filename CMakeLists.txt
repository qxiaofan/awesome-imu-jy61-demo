cmake_minimum_required(VERSION 2.8)

project(imu_jy61)

SET(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_FLAGS "-std=c++11")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -O0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -O0")

find_package(OpenCV 3.0 QUIET)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_INCLUDE_DIRS}
)

add_executable(imu_jy_61 main.cpp src/Uart61.cpp src/utils.cpp )

target_link_libraries(imu_jy_61 ${OpenCV_LIBS})

