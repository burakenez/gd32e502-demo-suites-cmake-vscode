project(GD32E502C_START LANGUAGES C CXX ASM)

add_library(GD32E502C_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32E502C_START/gd32E502c_start.c
    )

target_include_directories(GD32E502C_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E502C_START
    )

target_link_libraries(GD32E502C_START PUBLIC GD32E502_standard_peripheral)
