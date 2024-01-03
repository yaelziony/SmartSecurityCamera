cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(WatchfulEyeServer C CXX)

set(CMAKE_CXX_STANDARD 17)

# ---------------------------------------------------------------------------------------
# Sub-projects
# ---------------------------------------------------------------------------------------
enable_testing()

add_subdirectory(ServerLib)
add_subdirectory(ServerProcess)
add_subdirectory(Service)
add_subdirectory(ServerTest)
if(WIN32)
    add_subdirectory(ServerUI)
endif()