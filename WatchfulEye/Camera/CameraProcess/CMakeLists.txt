cmake_minimum_required(VERSION 3.16)

project(CameraProcess)
set(PROJECT_NAME CameraProcess)

# ---------------------------------------------------------------------------------------
# Source groups
# ---------------------------------------------------------------------------------------

set(Source_Files "main.cpp" )
set(ALL_FILES ${Source_Files})

# ---------------------------------------------------------------------------------------
# Target
# ---------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} PUBLIC CameraLib)
