/*!
    \file    main.c
    \brief   TRIGSEL timer trigger EXTI demo

    \version 2025-02-20, V1.4.0, demo for GD32E502
*/

/*
    Copyright (c) 2025, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32e502.h"
#include "systick.h"

void gpio_config(void);
void exti_config(void);
void timer_config(void);
static void led_flash(uint8_t times);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure systick */
    systick_config();

    /* configure LED GPIO */
    gpio_config();

    /* flash the LED for test */
    led_flash(1);

    delay_ms(1000);

    /* configure trigsel output pin */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_SYSCFG);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_af_set(GPIOC, GPIO_AF_7, GPIO_PIN_10);

    /* enable TRIGSEL clock */
    rcu_periph_clock_enable(RCU_TRIGSEL);
    /* select TIMER1 CH0 to trigger TRIGSEL_OUT0 */
    trigsel_init(TRIGSEL_OUTPUT_TRIGSEL_OUT0, TRIGSEL_INPUT_TIMER1_CH0);
    /* lock trigger register */
    trigsel_register_lock_set(TRIGSEL_OUTPUT_TRIGSEL_OUT0);

    /* configure EXTI */
    exti_config();
    /* configure TIMER1 */
    timer_config();

    while(1) {
    }
}

/*!
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* enable the LED1 GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure LED1 GPIO port */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    /* reset LED1 GPIO pin */
    gpio_bit_reset(GPIOA, GPIO_PIN_1);
}

/*!
    \brief      configure the EXTI
    \param[in]  none
    \param[out] none
    \retval     none
*/
void exti_config(void)
{
    /* enable the Wakeup key GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SYSCFG);

    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_10);

    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI10_15_IRQn, 2U, 0U);
    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN10);
    /* configure key EXTI line */
    exti_init(EXTI_10, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_10);
}

/*!
    \brief      timer configure function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
    /* TIMER1CLK = SystemCoreClock / 2000 = 50KHz */
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocinitpara;

    /* enable TIMER clock */
    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* configure TIMER */
    timer_initpara.prescaler         = 1999;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 9999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;

    timer_init(TIMER1, &timer_initpara);

    /* initialize TIMER channel output parameter struct */
    timer_channel_output_struct_para_init(&timer_ocinitpara);
    /* configure CH0 in PWM mode */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocinitpara);

    /* configure TIMER channel output pulse value */
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 5000);
    /* configure CH0 in PWM0 mode */
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    /* configure TIMER channel output shadow function */
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);

    /* enable auto-reload preload */
    timer_auto_reload_shadow_enable(TIMER1);

    /* enable TIMER */
    timer_enable(TIMER1);
}

/*!
    \brief      flash the LED for test
    \param[in]  times: times to flash the LED
    \param[out] none
    \retval     none
*/
static void led_flash(uint8_t times)
{
    uint8_t i;
    for(i = 0; i < times; i++) {
        /* delay 500 ms */
        delay_ms(500);
        /* turn on the LED1 */
        gpio_bit_set(GPIOA, GPIO_PIN_1);

        /* delay 500 ms */
        delay_ms(500);
        /* turn off the LED1 */
        gpio_bit_reset(GPIOA, GPIO_PIN_1);
    }
}
