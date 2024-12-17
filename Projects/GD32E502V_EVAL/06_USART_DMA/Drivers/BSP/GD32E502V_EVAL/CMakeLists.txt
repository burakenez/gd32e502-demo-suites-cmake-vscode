project(GD32E502V_EVAL LANGUAGES C CXX ASM)

add_library(GD32E502V_EVAL OBJECT
    ${DRIVERS_DIR}/BSP/GD32E502V_EVAL/gd32e502v_eval.c
    )

target_include_directories(GD32E502V_EVAL PUBLIC
    ${DRIVERS_DIR}/BSP/GD32E502V_EVAL
    )

target_link_libraries(GD32E502V_EVAL PUBLIC GD32E502_standard_peripheral)
