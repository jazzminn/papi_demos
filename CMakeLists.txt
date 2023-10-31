cmake_minimum_required(VERSION 3.0.0)

project(PapiEvaluation VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
add_compile_options(-std=c++20)

add_executable(papi_demo1 papi_demo1.cpp)
target_link_libraries(papi_demo1 -lpapi)

add_executable(papi_demo2 papi_demo2.cpp PapiMetrics.cpp)
target_link_libraries(papi_demo2 -lpapi)
