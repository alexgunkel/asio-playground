project(asio-playground)

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

include(cmake/asio-config.cmake)
add_executable(main main.cpp)
target_include_directories(main PRIVATE ${asio_INCLUDES})
target_link_libraries(main PRIVATE pthread)
