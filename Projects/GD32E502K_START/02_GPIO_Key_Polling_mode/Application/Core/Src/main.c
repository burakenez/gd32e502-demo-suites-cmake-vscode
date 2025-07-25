/*!
    \file    main.c
    \brief  keyboard polling mode

    \version 2023-06-30, V1.1.0, demo for GD32E502
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

    /* enable the LED1 GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure LED1 GPIO pin */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    /* reset LED2 GPIO pin */
    gpio_bit_reset(GPIOA, GPIO_PIN_8);

    /* enable the key GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure key pin as input */
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO_PIN_0);

    while(1) {
        /* check whether the key is pressed */
        if(SET == gpio_input_bit_get(GPIOA, GPIO_PIN_0)) {
            delay_ms(100);

            /* check whether the key is pressed */
            if(SET == gpio_input_bit_get(GPIOA, GPIO_PIN_0)) {
                /* toggle LED2 GPIO pin */
                gpio_bit_toggle(GPIOA, GPIO_PIN_8);
            }
            while(SET == gpio_input_bit_get(GPIOA, GPIO_PIN_0)) {
            }
        }
    }
}
