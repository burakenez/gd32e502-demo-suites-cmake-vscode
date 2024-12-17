project(GD32E502R_START LANGUAGES C CXX ASM)

add_library(GD32E502R_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32E502R_START/gd32E502r_start.c
    )

target_include_directories(GD32E502R_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E502R_START
    )

target_link_libraries(GD32E502R_START PUBLIC GD32E502_standard_peripheral)
