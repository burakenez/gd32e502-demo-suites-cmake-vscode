/*!
    \file    gd32e502k_start.c
    \brief   functions to manage leds, keys, COM ports

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

#include "gd32e502k_start.h"

/* private variables */
static const rcu_periph_enum GPIO_CLK[LED_MAX]     = {LED1_GPIO_CLK};

static const uint32_t GPIO_PORT[LED_MAX]           = {LED1_GPIO_PORT};

static const uint32_t GPIO_PIN[LED_MAX]            = {LED1_PIN};

static const rcu_periph_enum COM_CLK[COMn]         = {EVAL_COM_CLK};

static const uint32_t COM_TX_PIN[COMn]             = {EVAL_COM_TX_PIN};

static const uint32_t COM_RX_PIN[COMn]             = {EVAL_COM_RX_PIN};

static const uint32_t KEY_PORT[KEY_MAX]            = {WAKEUP_KEY_GPIO_PORT};

static const uint32_t KEY_PIN[KEY_MAX]             = {WAKEUP_KEY_PIN};

static const rcu_periph_enum KEY_CLK[KEY_MAX]      = {WAKEUP_KEY_GPIO_CLK};

static const exti_line_enum KEY_EXTI_LINE[KEY_MAX] = {WAKEUP_KEY_EXTI_LINE};

static const uint8_t KEY_PORT_SOURCE[KEY_MAX]      = {WAKEUP_KEY_EXTI_PORT_SOURCE};

static const uint8_t KEY_PIN_SOURCE[KEY_MAX]       = {WAKEUP_KEY_EXTI_PIN_SOURCE};

static const IRQn_Type KEY_IRQn[KEY_MAX]           = {WAKEUP_KEY_EXTI_IRQn};

/*!
    \brief      configure led GPIO
    \param[in]  lednum: specify the led to be configured
      \arg        LED1
    \param[out] none
    \retval     none
*/
void gd_eval_led_init(led_typedef_enum lednum)
{
    /* enable the led clock */
    rcu_periph_clock_enable(GPIO_CLK[lednum]);
    /* configure led GPIO port */
    gpio_mode_set(GPIO_PORT[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN[lednum]);
    gpio_output_options_set(GPIO_PORT[lednum], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN[lednum]);

    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn on selected led
    \param[in]  lednum: specify the led to be turned on
      \arg        LED1
    \param[out] none
    \retval     none
*/
void gd_eval_led_on(led_typedef_enum lednum)
{
    GPIO_BOP(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      turn off selected led
    \param[in]  lednum: specify the led to be turned off
      \arg        LED1
    \param[out] none
    \retval     none
*/
void gd_eval_led_off(led_typedef_enum lednum)
{
    GPIO_BC(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      toggle selected led
    \param[in]  lednum: specify the led to be toggled
      \arg        LED1
    \param[out] none
    \retval     none
*/
void gd_eval_led_toggle(led_typedef_enum lednum)
{
    GPIO_TG(GPIO_PORT[lednum]) = GPIO_PIN[lednum];
}

/*!
    \brief      configure key
    \param[in]  keynum: specify the key to be configured
      \arg        KEY_WAKEUP: WAKEUP key
    \param[in]  keymode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void gd_eval_key_init(key_typedef_enum keynum, keymode_typedef_enum keymode)
{
    /* enable the key clock */
    rcu_periph_clock_enable(KEY_CLK[keynum]);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* configure button pin as input */
    gpio_mode_set(KEY_PORT[keynum], GPIO_MODE_INPUT, GPIO_PUPD_NONE, KEY_PIN[keynum]);

    if(keymode == KEY_MODE_EXTI) {
        /* enable and set key EXTI interrupt to the lowest priority */
        nvic_irq_enable(KEY_IRQn[keynum], 2U, 0U);
        /* connect key EXTI line to key GPIO pin */
        syscfg_exti_line_config(KEY_PORT_SOURCE[keynum], KEY_PIN_SOURCE[keynum]);
        /* configure key EXTI line */
        exti_init(KEY_EXTI_LINE[keynum], EXTI_INTERRUPT, EXTI_TRIG_RISING);
        exti_interrupt_flag_clear(KEY_EXTI_LINE[keynum]);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  keynum: specify the key to be checked
      \arg        KEY_WAKEUP: WAKEUP key
    \param[out] none
    \retval     the key's GPIO pin value
*/
uint8_t gd_eval_key_state_get(key_typedef_enum keynum)
{
    return gpio_input_bit_get(KEY_PORT[keynum], KEY_PIN[keynum]);
}

/*!
    \brief      configure COM port
    \param[in]  com: COM on the board
      \arg        EVAL_COM: COM on the board
    \param[out] none
    \retval     none
*/
void gd_eval_com_init(uint32_t com)
{
    uint32_t com_id = 0U;

    if(EVAL_COM == com) {
        com_id = 0U;
    } else {
        while(1) {
        }
    }

    /* enable COM GPIO clock */
    rcu_periph_clock_enable(EVAL_COM_TX_CLK);
    rcu_periph_clock_enable(EVAL_COM_RX_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(COM_CLK[com_id]);
    rcu_usart_clock_config(com, RCU_USARTSRC_CKSYS);
    /* connect port to USARTx_Tx */
    gpio_af_set(EVAL_COM_TX_PORT, EVAL_COM_TX_AF, COM_TX_PIN[com_id]);

    /* connect port to USARTx_Rx */
    gpio_af_set(EVAL_COM_RX_PORT, EVAL_COM_RX_AF, COM_RX_PIN[com_id]);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(EVAL_COM_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_TX_PIN[com_id]);
    gpio_output_options_set(EVAL_COM_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, COM_TX_PIN[com_id]);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(EVAL_COM_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, COM_RX_PIN[com_id]);
    gpio_output_options_set(EVAL_COM_RX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, COM_RX_PIN[com_id]);

    /* USART configure */
    usart_deinit(com);
    usart_baudrate_set(com, 115200U);
    usart_receive_config(com, USART_RECEIVE_ENABLE);
    usart_transmit_config(com, USART_TRANSMIT_ENABLE);
    usart_enable(com);
}
