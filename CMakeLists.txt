set(APP_NAME future)

cmake_minimum_required(VERSION 3.5)
project(APP_NAME VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -O0 -Wall -Wextra -Wno-unused-parameter -Wno-write-strings")

set(CMAKE_CXX_STANDARD 11)

add_executable(${APP_NAME} main.cpp)

set(DEPENDECIES
    glib-2.0 gio-2.0 gmodule-2.0
)

find_package(PkgConfig REQUIRED)
if (NOT PkgConfig_FOUND)
    message(FATAL_ERROR "Unable to find PkgConfig")
endif ()

pkg_check_modules(LIB REQUIRED ${DEPENDECIES})
if (NOT LIB_FOUND)
    message(FATAL_ERROR "Unable to find some depencecies")
endif ()

include_directories(${LIB_INCLUDE_DIRS})
link_directories(${LIB_LIBRARY_DIRS})

target_link_libraries(${APP_NAME}
        ${LIB_LIBRARIES}
        -lpthread
)