cmake_minimum_required(VERSION 3.10)
project(vispointcloud)

set(CMAKE_CXX_STANDARD 17)

find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(vispointcloud src/visPointcloud.cpp)
target_include_directories(vispointcloud PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(vispointcloud ${PCL_LIBRARIES} yaml-cpp)