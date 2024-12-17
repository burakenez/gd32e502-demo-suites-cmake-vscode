/*!
    \file    main.c
    \brief   USART transmit and detect break frame

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
#include "gd32e502v_eval.h"
#include "systick.h"

__IO uint8_t bk_flag0 = 0;
__IO uint8_t bk_flag1 = 0;

void led_init(void);
void led_flash(int times);
void usart_config(void);
void nvic_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize the LEDs */
    led_init();

    /* configure systick */
    systick_config();

    nvic_config();

    /* configure tamper key and wakeup key */
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_GPIO);
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);

    /* flash the LEDs for 1 time */
    led_flash(1);

    /* initialize USART */
    usart_config();

    while(1) {
        /* check if the tamper key or wakeup key is pressed */
        if(RESET == gd_eval_key_state_get(KEY_TAMPER)) {
            delay_ms(50);
            if(RESET == gd_eval_key_state_get(KEY_TAMPER)) {
                delay_ms(50);
                if(RESET == gd_eval_key_state_get(KEY_TAMPER)) {
                    /* USART0 send a break frame */
                    usart_command_enable(USART0, USART_CMD_SBKCMD);
                    while(0 == bk_flag0) {
                    }
                    bk_flag0 = 0;
                }
            }
        } else if(RESET == gd_eval_key_state_get(KEY_WAKEUP)) {
            delay_ms(50);
            if(RESET == gd_eval_key_state_get(KEY_WAKEUP)) {
                delay_ms(50);
                if(RESET == gd_eval_key_state_get(KEY_WAKEUP)) {
                    /* USART1 send a break frame */
                    usart_command_enable(USART1, USART_CMD_SBKCMD);
                    while(0 == bk_flag1) {
                    }
                    bk_flag1 = 0;
                }
            }
        }
    }
}

/*!
    \brief      initialize the LEDs
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_init(void)
{
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
}

/*!
    \brief      flash the LEDs for test
    \param[in]  times: times to flash the LEDs
    \param[out] none
    \retval     none
*/
void led_flash(int times)
{
    int i;
    for(i = 0; i < times; i++) {
        /* delay 400 ms */
        delay_ms(400);

        /* turn on LEDs */
        gd_eval_led_on(LED1);
        gd_eval_led_on(LED2);

        /* delay 400 ms */
        delay_ms(400);
        /* turn off LEDs */
        gd_eval_led_off(LED1);
        gd_eval_led_off(LED2);
    }
}

/*!
    \brief      initialize USART
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
    rcu_usart_clock_config(USART0, RCU_USARTSRC_CKSYS);
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART1);
    rcu_usart_clock_config(USART1, RCU_USARTSRC_CKSYS);

    /* connect port to USART0 TX */
    gpio_af_set(GPIOA, GPIO_AF_5, GPIO_PIN_3);
    /* connect port to USART0 RX */
    gpio_af_set(GPIOA, GPIO_AF_5, GPIO_PIN_4);
    /* connect port to USART1 TX */
    gpio_af_set(GPIOC, GPIO_AF_5, GPIO_PIN_2);
    /* connect port to USART1 RX */
    gpio_af_set(GPIOC, GPIO_AF_5, GPIO_PIN_3);

    /* configure USART0 TX as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);

    /* configure USART0 RX as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_4);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_4);

    /* configure USART1 TX as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

    /* configure USART1 RX as alternate function push-pull */
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);

    /* configure USART0 */
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_baudrate_set(USART0, 9600U);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    /* enable LIN mode */
    usart_lin_mode_enable(USART0);
    /* 10 bits break detection */
    usart_lin_break_detection_length_config(USART0, USART_LBLEN_10B);
    /* enable LIN break detection interrupt */
    usart_interrupt_enable(USART0, USART_INT_LBD);
    usart_enable(USART0);

    /* configure USART1 */
    usart_deinit(USART1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_baudrate_set(USART1, 9600U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);
    /* enable LIN mode */
    usart_lin_mode_enable(USART1);
    /* 10 bits break detection */
    usart_lin_break_detection_length_config(USART1, USART_LBLEN_10B);
    /* enable LIN break detection interrupt */
    usart_interrupt_enable(USART1, USART_INT_LBD);
    usart_enable(USART1);
}

/*!
    \brief      initialize NVIC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(USART0_IRQn, 0, 0);
    nvic_irq_enable(USART1_IRQn, 0, 1);
}
