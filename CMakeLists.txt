cmake_minimum_required(VERSION 3.16)
project(FOS_HEAD LANGUAGES CXX C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)


add_subdirectory(modules/CameraStereo)

add_executable(fos_head core/core.cpp)

target_include_directories(fos_head
    PRIVATE
        ${CMAKE_SOURCE_DIR}/modules/CameraStereo
        ${LIBCAMERA_INCLUDE_DIRS}
)

target_link_libraries(fos_head
    PRIVATE
        CameraStereo
)