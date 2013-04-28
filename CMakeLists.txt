project(runfb CXX)

cmake_minimum_required(VERSION 2.8)

add_executable( runfb main.cpp common.h common.cpp )
target_link_libraries(runfb bcm2835)
