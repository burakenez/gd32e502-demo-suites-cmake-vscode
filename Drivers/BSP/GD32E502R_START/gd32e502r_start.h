/*!
    \file    gd32e502r_start.h
    \brief   definitions for EVAL's leds, keys and COM ports hardware resources

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

#ifndef GD32E502R_START_H
#define GD32E502R_START_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gd32e502.h"

/* exported types */
typedef enum {
    LED1 = 0,                           /* LED1 */
    LED2 = 1,                           /* LED2 */
    LED3 = 2,                           /* LED3 */
    LED4 = 3,                           /* LED4 */
    LED_MAX
} led_typedef_enum;

typedef enum {
    KEY_USER = 0,                       /* USER key */
    KEY_MAX
} key_typedef_enum;

typedef enum {
    KEY_MODE_GPIO = 0,                  /* GPIO mode */
    KEY_MODE_EXTI = 1                   /* EXTI mode */
} keymode_typedef_enum;

/* LEDs */
#define LED1_PIN                         GPIO_PIN_2
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK                    RCU_GPIOF

#define LED2_PIN                         GPIO_PIN_5
#define LED2_GPIO_PORT                   GPIOF
#define LED2_GPIO_CLK                    RCU_GPIOF

#define LED3_PIN                         GPIO_PIN_0
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCU_GPIOC

#define LED4_PIN                         GPIO_PIN_1
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCU_GPIOC

/* User push-button */
#define USER_KEY_PIN                     GPIO_PIN_0
#define USER_KEY_GPIO_PORT               GPIOA
#define USER_KEY_GPIO_CLK                RCU_GPIOA
#define USER_KEY_EXTI_LINE               EXTI_0
#define USER_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define USER_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN0
#define USER_KEY_EXTI_IRQn               EXTI0_IRQn

/* eval board low layer COM */
#define COMn                             1U

/* definition for COM, connected to USART */
#define EVAL_COM                         USART0
#define EVAL_COM_CLK                     RCU_USART0

#define EVAL_COM_TX_PIN                  GPIO_PIN_3
#define EVAL_COM_RX_PIN                  GPIO_PIN_4

#define EVAL_COM_TX_PORT                 GPIOA
#define EVAL_COM_TX_CLK                  RCU_GPIOA
#define EVAL_COM_TX_AF                   GPIO_AF_5

#define EVAL_COM_RX_PORT                 GPIOA
#define EVAL_COM_RX_CLK                  RCU_GPIOA
#define EVAL_COM_RX_AF                   GPIO_AF_5

/* function declarations */
/* configure led GPIO */
void gd_eval_led_init(led_typedef_enum lednum);
/* turn on selected led */
void gd_eval_led_on(led_typedef_enum lednum);
/* turn off selected led */
void gd_eval_led_off(led_typedef_enum lednum);
/* toggle the selected led */
void gd_eval_led_toggle(led_typedef_enum lednum);
/* configure key */
void gd_eval_key_init(key_typedef_enum keynum, keymode_typedef_enum keymode);
/* return the selected key state */
uint8_t gd_eval_key_state_get(key_typedef_enum keynum);
/* configure COM port */
void gd_eval_com_init(uint32_t com);

#ifdef __cplusplus
}
#endif

#endif /* GD32E502R_START_H */
