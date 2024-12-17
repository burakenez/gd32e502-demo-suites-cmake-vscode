/*!
    \file    main.c
    \brief   comparator interrupt example

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

/* configure RCU peripheral */
void rcu_config(void);
/* configure GPIO peripheral */
void gpio_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    __IO int i;
    /* configure RCU peripheral */
    rcu_config();
    /* configure GPIO peripheral */
    gpio_config();
    
    gd_eval_led_init(LED2);
    
    /* initialize CMP0 */
    cmp_deinit(CMP0);
    /* configure CMP0 mode */
    cmp_mode_init(CMP0, CMP_MODE_HIGHSPEED, CMP_INVERTING_INPUT_VREFINT, CMP_HYSTERESIS_NO);
    /* configure CMP0 output */
    cmp_output_init(CMP0, CMP_OUTPUT_NONE, CMP_OUTPUT_POLARITY_NONINVERTED);

    /* enable CMP0 */
    cmp_enable(CMP0);
    for( i=0; i<100; i++);
    
    while (1){
        /* get the output level */
        if(CMP_OUTPUTLEVEL_HIGH == cmp_output_level_get(CMP0)){
            gd_eval_led_on(LED2);
        }else{
            gd_eval_led_off(LED2);
        }
    }
}

/*!
    \brief      configure RCU 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable GPIOF clock */
    rcu_periph_clock_enable(RCU_GPIOF);
    /* enable comparator clock */
    rcu_periph_clock_enable(RCU_CMP);
}

/*!
    \brief      configure GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure PC11 as CMP0 input */
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_11);

    /* CMP0 output selection: PF2 */
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2);
    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_af_set(GPIOF, GPIO_AF_7, GPIO_PIN_2);
}
