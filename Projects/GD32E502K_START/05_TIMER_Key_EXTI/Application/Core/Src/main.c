/*!
    \file    main.c
    \brief   Key LED and generate TIMER PWM to toggle LED in IRQ demo

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
#include "gd32e502k_start.h"

/* configure the LEDs */
void led_config(void);
/* flash the LED for test */
void led_flash(int times);
/* configure EXTI */
void exti_config(void);
/* configure the GPIO ports */
void gpio_config(void);
/* configure the TIMER peripheral */
void timer_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize KEY and LED, configure SysTick */
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_EXTI);
    led_config();
    systick_config();

    /* flash the LED for test */
    led_flash(1);

    /* configure EXTI, TIMER */
    exti_config();
    gpio_config();
    timer_config();

    while(1){
    }
}

/*! 
    \brief      configure the LEDs
    \param[in]  none
    \param[out] none
    \retval     none
  */
void led_config(void)
{
    gd_eval_led_init(LED1);
}

/*! 
    \brief      configure the EXTI
    \param[in]  none
    \param[out] none
    \retval     none
  */
void exti_config(void)
{
    /* enable the clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    /* configure GPIO as input */
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_2);

    /* enable and set EXTI interrupt */
    nvic_irq_enable(EXTI2_IRQn, 1U, 0U);
    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN2);

    /* configure EXTI line */
    exti_init(EXTI_2, EXTI_INTERRUPT, EXTI_TRIG_FALLING);
    exti_interrupt_flag_clear(EXTI_2);
}

/*! 
    \brief      configure the GPIO ports
    \param[in]  none
    \param[out] none
    \retval     none
  */
void gpio_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);

    /*configure PA7(TIMER1 CH1) as alternate function*/
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_7);
}

/*!
    \brief      configure the TIMER peripheral
    \param[in]  none
    \param[out] none
    \retval     none
  */
void timer_config(void)
{
    /* -----------------------------------------------------------------------
    TIMER1CLK is 100KHz
    TIMER1 channel0 duty cycle = (25000/ 50000)* 100  = 50%
    ----------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER1);

    timer_deinit(TIMER1);

    /* TIMER configuration */
    timer_initpara.prescaler         = 999;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 49999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);

    /* configurate CH1 in PWM mode0 */
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;

    timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 25000);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER1);
    /* auto-reload preload enable */
    timer_enable(TIMER1);
}

/*!
    \brief      flash the LED for test
    \param[in]  times: times to flash the LEDs
    \param[out] none
    \retval     none
*/
void led_flash(int times)
{
    int i;
    for(i = 0; i < times; i++){
        /* delay 500 ms */
        delay_ms(500);
        /* toggle the LED */
        gd_eval_led_toggle(LED1);

        /* delay 500 ms */
        delay_ms(500);
        /* toggle the LED */
        gd_eval_led_toggle(LED1);
    }
}
