add_library(CameraStereo
    CameraStereo.cpp
    Camera.cpp
) 

target_include_directories(StereoCamera
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package((PkgConfig REQUIRED))
pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET libcamera)

target_link_libraries(StereoCamera
    PUBLIC
        PkgConfig::LIBCAMERA
)

target_compile_features(StereoCamera PUBLIC cxx_std_17)