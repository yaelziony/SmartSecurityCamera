cmake_minimum_required(VERSION 3.16)

project(CameraTest)
set(PROJECT_NAME CameraTest)

# ---------------------------------------------------------------------------------------
# Source groups
# ---------------------------------------------------------------------------------------

set(Source_Files "cmpFrames.cpp")
set(ALL_FILES ${Source_Files})

# ---------------------------------------------------------------------------------------
# Catch2
# ---------------------------------------------------------------------------------------

find_package(Catch2 3 REQUIRED)

# ---------------------------------------------------------------------------------------
# Target
# ---------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${ALL_FILES})

target_link_libraries(${PROJECT_NAME}
					  PUBLIC
					  Catch2::Catch2WithMain
					  CameraLib)