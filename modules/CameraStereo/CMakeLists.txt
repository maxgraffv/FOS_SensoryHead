add_library(CameraStereo
    CameraStereo.cpp
    Camera.cpp
) 

target_include_directories(CameraStereo
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${LIBCAMERA_INCLUDE_DIRS}
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCAMERA REQUIRED IMPORTED_TARGET)

target_link_libraries(CameraStereo
    PUBLIC
        PkgConfig::LIBCAMERA
)

target_compile_features(CameraStereo PUBLIC cxx_std_17)