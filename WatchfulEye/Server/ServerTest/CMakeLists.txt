cmake_minimum_required(VERSION 3.16)

project(ServerTest)
set(PROJECT_NAME ServerTest)

# ---------------------------------------------------------------------------------------
# Source groups
# ---------------------------------------------------------------------------------------

set(Source_Files "protectedQueue.cpp"
				 "calcAvg.cpp"
				 "improveQueue.cpp"
				 )
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
					  ServerLib)
