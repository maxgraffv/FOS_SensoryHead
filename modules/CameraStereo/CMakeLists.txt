find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED libcamera)

add_library(CameraStereo
    CameraStereo.cpp
    SingleCamera.cpp
) 

target_include_directories(CameraStereo
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBCAMERA_INCLUDE_DIRS}
)


target_link_libraries(CameraStereo
    PUBLIC
        ${LIBCAMERA_LIBRARIES}
)

target_compile_options(CameraStereo
    PUBLIC
        ${LIBCAMERA_CFLAGS_OTHER}
)

target_compile_features(CameraStereo PUBLIC cxx_std_17)