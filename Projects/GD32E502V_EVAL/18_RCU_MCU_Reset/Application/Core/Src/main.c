/*!
    \file    main.c
    \brief   get system reset reason demo

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

typedef enum {
    SYSRST_SOFTWARE = 0,
    SYSRST_WWDGT,
    SYSRST_FWDGT,
    SYSRST_OBL,
    SYSRST_LOCKUP,
    SUM_SYSRST
} system_reset_enum;

void value_bkp_get(uint8_t *val);
void system_reset_reason_get(void);
void system_reset_select(uint8_t rst);

__IO extern uint8_t g_button_press_flag;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint8_t func_seq = 0;

    /* initialize the USART */
    gd_eval_com_init(EVAL_COM);

    /* initialize the wakeup key */
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_EXTI);

    printf("\r\n/=========== Gigadevice System Reset Demo ===========/ \r\n");
    printf("press key_Wakeup to select a method to generate a system reset \r\n");
    /* get the system reset reason */
    system_reset_reason_get();
    /* clear all reset flags */
    rcu_all_reset_flag_clear();

    while(1) {
        if(1 == g_button_press_flag) {
            /* if the button is pressed */
            g_button_press_flag = 0;
            /* get the value from BKP DATA area */
            value_bkp_get(&func_seq);
            gd_eval_led_on((led_typedef_enum)(func_seq % 2));
            /* select a system reset method */
            system_reset_select(func_seq);
        }
    }
}

/*!
    \brief      get the reset reason
    \param[in]  none
    \param[out] none
    \retval     none
*/
void system_reset_reason_get(void)
{
    /* power reset */
    if(RESET != rcu_flag_get(RCU_FLAG_PORRST)) {
        printf("\r\nReset reason: Power reset generated!\r\n");
    }

    /* software reset */
    if(RESET != rcu_flag_get(RCU_FLAG_SWRST)) {
        printf("\r\nReset reason: Software reset generated!\r\n");
    }

    /* external pin reset */
    if((RESET != rcu_flag_get(RCU_FLAG_EPRST)) && (RESET == rcu_flag_get(RCU_FLAG_PORRST))) {
        printf("\r\nReset reason: External PIN reset generated!\r\n");
    }

    /* windows watchdog reset */
    if(RESET != rcu_flag_get(RCU_FLAG_WWDGTRST)) {
        printf("\r\nReset reason: Windows watchdog reset generated!\r\n");
    }

    /* free watchdog reset */
    if(RESET != rcu_flag_get(RCU_FLAG_FWDGTRST)) {
        printf("\r\nReset reason: Free watchdog reset generated!\r\n");
    }

    /* option byte loader reset */
    if(RESET != rcu_flag_get(RCU_FLAG_OBLRST)) {
        printf("\r\nReset reason: Option byte loader generated!\r\n");
    }

    /* CPU lockup reset */
    if(RESET != rcu_flag_get(RCU_FLAG_LOCKUPRST)) {
        printf("\r\nReset reason: CPU lockup reset generated!\r\n");
    }
}

/*!
    \brief      get the data which is stored in BKP DATA area
    \param[in]  none
    \param[out] val: the stored value
    \retval     none
*/
void value_bkp_get(uint8_t *val)
{
    uint32_t tmp_val = 0;
    /* PMU lock enable */
    rcu_periph_clock_enable(RCU_PMU);
    /* BKP clock enable */
    rcu_periph_clock_enable(RCU_BKP);
    /* enable write access to the registers in backup domain */
    pmu_backup_write_enable();

    /* the default value of BKP DATA0 is 0 */
    tmp_val = BKP_DATA0_9(0);
    *val = (uint8_t)tmp_val;
    tmp_val++;
    tmp_val %= SUM_SYSRST;
    /* store the date in BKP DATA area */
    BKP_DATA0_9(0) = tmp_val;
}

/*!
    \brief      select system reset method
    \param[in]  rst: reset method
      \arg        SYSRST_SOFTWARE: software reset
      \arg        SYSRST_WWDGT: windows watchdog reset
      \arg        SYSRST_FWDGT: free watchdog reset
      \arg        SYSRST_OBL: option byte loader reset
      \arg        SYSRST_LOCKUP: CPU lockup reset
    \param[out] none
    \retval     none
*/
void system_reset_select(uint8_t rst)
{
    switch(rst) {
    case SYSRST_SOFTWARE:
        printf("\r\nGenerate software reset, system resetting...... \r\n");
        NVIC_SystemReset();
        break;
    case SYSRST_WWDGT:
        printf("\r\nGenerate windows watchdog reset, system resetting...... \r\n");
        rcu_periph_clock_enable(RCU_WWDGT);
        wwdgt_config(127, 80, WWDGT_CFG_PSC_DIV8);
        wwdgt_enable();
        break;
    case SYSRST_FWDGT:
        printf("\r\nGenerate free watchdog reset, system resetting...... \r\n");
        /* configure FWDGT counter clock: 40KHz(IRC40K) / 64 = 0.625 KHz */
        fwdgt_config(50, FWDGT_PSC_DIV64);
        fwdgt_enable();
        break;
    case SYSRST_OBL:
        printf("\r\nGenerate option byte loader reset, system resetting...... \r\n");
        /* option byte loader reset */
        fmc_unlock();
        ob_unlock();
        ob_data_program(0x00);
        ob_reset();
        break;
    case SYSRST_LOCKUP:
        printf("\r\nGenerate CPU lockup reset, system resetting...... \r\n");
        /* enable CPU lockup reset */
        rcu_system_reset_enable(RCU_SYSRST_LOCKUP);
        /* enter hardfault, illegal access */
        *(__IO uint32_t *)(0xCBDC1234) = 123;
        break;

    default:
        printf("\r\nsystem reset select parameter error! \r\n");
        while(1);
    }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TC));

    return ch;
}
