/*!
    \file    main.c
    \brief   CAN network demo

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
#include <stdio.h>
#include "systick.h"
#include "string.h"

const uint8_t tx_data[8] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
uint8_t rx_data[8];
uint8_t i = 0;

can_mailbox_descriptor_struct g_transmit_message;
can_mailbox_descriptor_struct g_receive_message;
FlagStatus can0_receive_flag;
FlagStatus can_tx_state;

/*!
    \brief      configure GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
void can_gpio_config(void)
{
    /* enable CAN clock */
    rcu_can_clock_config(CAN0, RCU_CANSRC_PCLK2_DIV_2);
    rcu_can_clock_config(CAN1, RCU_CANSRC_PCLK2_DIV_2);
    rcu_periph_clock_enable(RCU_CAN0);
    rcu_periph_clock_enable(RCU_CAN1);
    /* enable CAN port clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOD);

    /* configure CAN0 GPIO */
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_13);
    gpio_af_set(GPIOB, GPIO_AF_6, GPIO_PIN_13);

    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_14);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_af_set(GPIOB, GPIO_AF_6, GPIO_PIN_14);

    /* configure CAN1 GPIO */
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_af_set(GPIOD, GPIO_AF_6, GPIO_PIN_6);

    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_af_set(GPIOD, GPIO_AF_6, GPIO_PIN_7);
}

/*!
    \brief      configure BSP
    \param[in]  none
    \param[out] none
    \retval     none
*/
void bsp_board_config(void)
{
    /* configure USART */
    gd_eval_com_init(EVAL_COM);

    /* configure WAKEUP key */
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);

    /* configure LED2 */
    gd_eval_led_init(LED2);
    gd_eval_led_off(LED2);

}

/*!
    \brief      initialize CAN function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void can_config(void)
{
    can_parameter_struct can_parameter;

    /* initialize CAN register */
    can_deinit(CAN0);
    can_deinit(CAN1);
    /* initialize CAN */
    can_struct_para_init(CAN_INIT_STRUCT, &can_parameter);

    /* initialize CAN parameters */
    can_parameter.internal_counter_source = CAN_TIMER_SOURCE_BIT_CLOCK;   //internal time counter increase 1 when send or receive a bit
    can_parameter.self_reception = DISABLE;                               //receive the frame sent by itself or not
    can_parameter.mb_tx_order = CAN_TX_HIGH_PRIORITY_MB_FIRST;            //transmit frame priority by MB priority or MB number
    can_parameter.mb_tx_abort_enable = ENABLE;                            //support transmission abort function or not
    can_parameter.local_priority_enable = DISABLE;                        //MB priority structure includes local priority value or not
    can_parameter.mb_rx_ide_rtr_type = CAN_IDE_RTR_FILTERED;              //when receive, (filter IDE && RTR bit), or (always compare IDE && not compare RTR bit)
    can_parameter.mb_remote_frame = CAN_STORE_REMOTE_REQUEST_FRAME;       //when receive remote request frame, store it as data frame or generate a remote response frame
    can_parameter.rx_private_filter_queue_enable = DISABLE;               //use separate filters / a same filter for all received frames
    can_parameter.edge_filter_enable = DISABLE;                           //used for bus intergration state, enable to detect two continuous norminal dominant bit for hardware synchronous edge
    can_parameter.protocol_exception_enable = DISABLE;                    //enable to detect protocol exception event(when not in FD mode, but receive a FD frame)
    can_parameter.rx_filter_order = CAN_RX_FILTER_ORDER_MAILBOX_FIRST;    //if RX FIFO is enabled, received frame to match mailbox/FIFO first
    can_parameter.memory_size = CAN_MEMSIZE_32_UNIT;                      //32*4 words specific RAM memory for mailbox and RX FIFO
    /* filter configuration */
    can_parameter.mb_public_filter = 0x0;                                 //configure CAN_RMPUBF register (refer to can_private_filter_config() to configure CAN_RFIFOMPFx registers if separate filters are used)
    can_parameter.resync_jump_width = 1;                                  //SJW
    can_parameter.prop_time_segment = 2;                                  //PTS segment
    can_parameter.time_segment_1 = 4;                                     //PBS1 segment
    can_parameter.time_segment_2 = 3;                                     //PBS2 segment
    /* 125Kbps */
    can_parameter.prescaler = 40;                                         //baudrate = fCANCLK/prescaler/(1+PTS+PBS1+PBS2)

    /* initialize CAN */
    can_init(CAN0, &can_parameter);
    can_init(CAN1, &can_parameter);

    /* configure CAN0 NVIC */
    nvic_irq_enable(CAN0_Message_IRQn, 0, 0);

    /* enable CAN MB0 interrupt */
    can_interrupt_enable(CAN0, CAN_INT_MB0);

    can_operation_mode_enter(CAN1, CAN_NORMAL_MODE);
    can_operation_mode_enter(CAN0, CAN_NORMAL_MODE);
}

void communication_check(void)
{
    /* CAN0 receive data correctly, the received data is printed */
    if(SET == can0_receive_flag) {
        can0_receive_flag = RESET;

        /* check the receive message */
        can_mailbox_receive_data_read(CAN0, 0, &g_receive_message);
        if(0 == memcmp(rx_data, tx_data, 8)) {
            printf("\r\nCAN0 receive data: \r\n");
            for(i = 0; i < 8; i++) {
                printf("%02x\r\n", rx_data[i]);
            }
            gd_eval_led_toggle(LED2);
        }
    }
}

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
    /* configure board */
    bsp_board_config();
    /* configure GPIO */
    can_gpio_config();
    /* initialize CAN and filter */
    can_config();

    printf("\r\n communication test between CAN0 and CAN1, please press WAKEUP key to start! \r\n");

    can_struct_para_init(CAN_MDSC_STRUCT, &g_transmit_message);
    can_struct_para_init(CAN_MDSC_STRUCT, &g_receive_message);
    /* initialize transmit message */
    g_transmit_message.rtr = 0;
    g_transmit_message.ide = 0;
    g_transmit_message.code = CAN_MB_TX_STATUS_DATA;
    g_transmit_message.brs = 0;
    g_transmit_message.fdf = 0;
    g_transmit_message.prio = 0;
    g_transmit_message.data_bytes = 8;
    /* tx message content */
    g_transmit_message.data = (uint32_t *)(tx_data);
    g_transmit_message.id = 0x55;

    g_receive_message.rtr = 0;
    g_receive_message.ide = 0;
    g_receive_message.code = CAN_MB_RX_STATUS_EMPTY;
    /* rx mailbox */
    g_receive_message.id = 0x55;
    g_receive_message.data = (uint32_t *)(rx_data);
    can_mailbox_config(CAN0, 0, &g_receive_message);
    can_tx_state = RESET;

    while(1) {
        /* test whether the WAKEUP key is pressed */
        if(0 == gd_eval_key_state_get(KEY_WAKEUP)) {
            delay_ms(100);
            if(0 == gd_eval_key_state_get(KEY_WAKEUP)) {
                if((RESET == can_tx_state) || (SET == can_flag_get(CAN1, CAN_FLAG_MB1))){
                    can_tx_state = SET;
                    can_flag_clear(CAN1, CAN_FLAG_MB1);
                    /* transmit message */
                    can_mailbox_config(CAN1, 1, &g_transmit_message);

                    printf("\r\nCAN1 transmit data: \r\n");
                    for(i = 0; i < 8; i++) {
                        printf("%02x\r\n", tx_data[i]);
                    }
                }

                /* waiting for the WAKEUP key up */
                while(0 == gd_eval_key_state_get(KEY_WAKEUP));
            }
        }
        communication_check();
    }
}

#ifdef __GNUC__
/* retarget the C library printf function to the usart, in Eclipse GCC environment */
int __io_putchar(int ch)
{
    usart_data_transmit(EVAL_COM, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TBE));
    return ch;
}
#else
/* retarget the C library printf function to the usart */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TBE));
    return ch;
}
#endif /* __GNUC__ */
