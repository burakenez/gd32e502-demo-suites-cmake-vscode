project(GD32E502K_START LANGUAGES C CXX ASM)

add_library(GD32E502K_START OBJECT
    ${DRIVERS_DIR}/BSP/GD32E502K_START/gd32e502k_start.c
    )

target_include_directories(GD32E502K_START PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E502K_START
    )

target_link_libraries(GD32E502K_START PUBLIC GD32E502_standard_peripheral)
