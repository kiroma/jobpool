cmake_minimum_required(VERSION 3.8)
project(jobpool)
add_executable(jobpool
main.cpp)
find_package(Threads)
target_link_libraries(jobpool Threads::Threads)
