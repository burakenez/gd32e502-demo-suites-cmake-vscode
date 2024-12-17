project(GD32E502_standard_peripheral LANGUAGES C CXX ASM)

# Comment-out unused source files.
add_library(GD32E502_standard_peripheral OBJECT
	${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_adc.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_bkp.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_can.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_cmp.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_crc.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_dac.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_dbg.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_dma.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_exti.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_fmc.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_fwdgt.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_gpio.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_i2c.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_mfcom.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_misc.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_pmu.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_rcu.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_rtc.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_spi.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_syscfg.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_timer.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_trigsel.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_usart.c
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Source/gd32e502_wwdgt.c
    )

target_include_directories(GD32E502_standard_peripheral PUBLIC
    ${DRIVERS_DIR}/GD32E502_standard_peripheral/Include
    )

# CMSIS header only library is linked.
target_link_libraries(GD32E502_standard_peripheral PUBLIC CMSIS)
