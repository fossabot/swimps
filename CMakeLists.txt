cmake_minimum_required(VERSION 3.16)
project(swimps VERSION 0.0.1 LANGUAGES CXX)

# Dump variables for debugging purposes.
get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

enable_testing()

add_compile_options(-Wall -Wextra -pedantic -Werror)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -fsanitize=undefined -g3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=undefined -g3")

option(SWIMPS_ENABLE_COVERAGE "Enable code coverage flags." OFF)
if(SWIMPS_ENABLE_COVERAGE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    link_libraries(gcov)
endif()

add_subdirectory(swimps-test)
add_subdirectory(swimps-container)
add_subdirectory(swimps-preload)
add_subdirectory(swimps-log)
add_subdirectory(swimps-io)
add_subdirectory(swimps-profile)
add_subdirectory(swimps-error)
add_subdirectory(swimps-time)
add_subdirectory(swimps-trace)
add_subdirectory(swimps-trace-file)
add_subdirectory(swimps-assert)

add_executable(swimps source/swimps.cpp)
target_link_libraries(swimps PRIVATE swimps-profile)
