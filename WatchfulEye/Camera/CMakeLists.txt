cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(WatchfulEyeCamera C CXX)

set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------------------------------------
# Sub-projects
# ---------------------------------------------------------------------------------------

add_subdirectory(CameraLib)
add_subdirectory(CameraProcess)
add_subdirectory(CameraTest)
add_subdirectory(Service)
if(WIN32)
    add_subdirectory(CameraUI)
endif()