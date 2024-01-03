set(PROJECT_NAME ServerLib)

# ---------------------------------------------------------------------------------------
# Source groups
# ---------------------------------------------------------------------------------------
set(Header_Files
    "Yolo.h"
    "Logging.h"
    "Server.h"
    "Queue.h"
    "Sqlite.h"
    "sqlite-amalgamation/sqlite3.h"
    "ReadConfigServer.h"
    "ServerService.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Yolo.cpp"
    "Logging.cpp"
    "Server.cpp"
    "Queue.cpp"
    "Sqlite.cpp"
    "sqlite-amalgamation/sqlite3.c"
    "ReadConfigServer.cpp"
    "ServerService.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

# ---------------------------------------------------------------------------------------
# Threads
# ---------------------------------------------------------------------------------------

find_package(Threads REQUIRED)

# ---------------------------------------------------------------------------------------
# OpenCV
# ---------------------------------------------------------------------------------------

list(APPEND CMAKE_PREFIX_PATH ${OpenCV_BIN_DIR})

# Set OpenCV_DIR to the OpenCV installation directory
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
set(OpenCV_DIR "C:/openCV/opencv/build")
endif()

# Find OpenCV package
find_package(OpenCV REQUIRED)


# Include OpenCV libraries and directories
include_directories(${OpenCV_INCLUDE_DIRS})

# ---------------------------------------------------------------------------------------
# spdlog
# ---------------------------------------------------------------------------------------

find_package(spdlog REQUIRED)

# ---------------------------------------------------------------------------------------
# Target
# ---------------------------------------------------------------------------------------
add_library(${PROJECT_NAME} ${ALL_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME} PUBLIC 
                       Threads::Threads
                       ${OpenCV_LIBS}
                       ${CMAKE_DL_LIBS}
                       spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
                       myproto)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
target_link_libraries(${PROJECT_NAME} PUBLIC ncurses)
endif()