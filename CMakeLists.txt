cmake_minimum_required(VERSION 3.16)
project(FOS_HEAD LANGUAGES CXX C)

set(CMAKE_C_STANDARD C17)


add_subdirectory(modules/StereoCamera)


add_executable(fos_head core/core.cpp)
target_link_libraries(fos_head 
    PUBLIC
    PkgConfig::LIBCAMERA
)