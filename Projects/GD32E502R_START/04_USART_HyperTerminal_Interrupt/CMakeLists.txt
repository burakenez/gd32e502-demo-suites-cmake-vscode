cmake_minimum_required(VERSION 3.20)

include(${CMAKE_SOURCE_DIR}/cmake/project.cmake)

project(Application LANGUAGES C CXX ASM)

set(DRIVERS_DIR ${CMAKE_SOURCE_DIR}/../../../Drivers)
set(MIDDLEWARES_DIR ${CMAKE_SOURCE_DIR}/../../../Middlewares)
set(UTILITIES_DIR ${CMAKE_SOURCE_DIR}/../../../Utilities)
set(TOOLS_DIR ${CMAKE_SOURCE_DIR}/../../../Tools)

add_subdirectory(Application)
add_subdirectory(Drivers/CMSIS)
add_subdirectory(Drivers/GD32E502_standard_peripheral)
add_subdirectory(Drivers/BSP/GD32E502R_START)

project_add_target_properties(Application)
project_add_target_properties(GD32E502_standard_peripheral)
project_add_target_properties(GD32E502R_START)
