/*!
    \file    gd32e502_pmu.c
    \brief   PMU driver

     \version 2025-02-10, V1.5.0, firmware for GD32E502
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

#include "gd32e502_pmu.h"

/*!
    \brief      reset PMU register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_deinit(void)
{
    /* reset PMU */
    rcu_periph_reset_enable(RCU_PMURST);
    rcu_periph_reset_disable(RCU_PMURST);
}

/*!
    \brief      select low voltage detector threshold
    \param[in]  lvdt_n:
                only one parameter can be selected which is shown as below:
      \arg        PMU_LVDT_0: voltage threshold is 2.9V
      \arg        PMU_LVDT_1: voltage threshold is 3.1V
      \arg        PMU_LVDT_2: voltage threshold is 3.3V
      \arg        PMU_LVDT_3: voltage threshold is 3.5V
      \arg        PMU_LVDT_4: voltage threshold is 4.0V
      \arg        PMU_LVDT_5: voltage threshold is 4.2V
      \arg        PMU_LVDT_6: voltage threshold is 4.4V
      \arg        PMU_LVDT_7: voltage threshold is 4.6V
    \param[out] none
    \retval     none
*/
void pmu_lvd_select(uint32_t lvdt_n)
{
    /* disable LVD */
    PMU_CTL &= ~PMU_CTL_LVDEN;
    /* clear LVDT bits */
    PMU_CTL &= ~PMU_CTL_LVDT;
    /* set LVDT bits according to lvdt_n */
    PMU_CTL |= lvdt_n;
    /* enable LVD */
    PMU_CTL |= PMU_CTL_LVDEN;
}

/*!
    \brief      disable PMU lvd
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lvd_disable(void)
{
    /* disable LVD */
    PMU_CTL &= ~PMU_CTL_LVDEN;
}

/*!
    \brief      select over voltage detector threshold
    \param[in]  ovdt_n:
                only one parameter can be selected which is shown as below:
      \arg        PMU_OVDT_0: voltage threshold is 5.0V
      \arg        PMU_OVDT_1: voltage threshold is 5.5V
    \param[out] none
    \retval     none
*/
void pmu_ovd_select(uint32_t ovdt_n)
{
    /* disable OVD */
    PMU_CTL &= ~PMU_CTL_OVDEN;
    /* clear OVDT bits */
    PMU_CTL &= ~PMU_CTL_OVDT;
    /* set OVDT bits according to ovdt_n */
    PMU_CTL |= ovdt_n;
    /* enable OVD */
    PMU_CTL |= PMU_CTL_OVDEN;
}

/*!
    \brief      disable PMU ovd
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_ovd_disable(void)
{
    /* disable OVD */
    PMU_CTL &= ~PMU_CTL_OVDEN;
}

/*!
    \brief      enable low-driver mode in deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lowdriver_mode_enable(void)
{
    PMU_CTL |= PMU_CTL_LDEN;
}

/*!
    \brief      disable low-driver mode in deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_lowdriver_mode_disable(void)
{
    PMU_CTL &= ~PMU_CTL_LDEN;
}

/*!
    \brief      SRAM1 power off in deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_sram1_poweroff_mode_enable(void)
{
    PMU_CTL |= PMU_CTL_SRAMSW1;
}

/*!
    \brief      SRAM1 power on in deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_sram1_poweroff_mode_disable(void)
{
    PMU_CTL &= ~PMU_CTL_SRAMSW1;
}

/*!
    \brief      SRAM2 power off in deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_sram2_poweroff_mode_enable(void)
{
    PMU_CTL |= PMU_CTL_SRAMSW2;
}

/*!
    \brief      SRAM2 power on in deep-sleep mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_sram2_poweroff_mode_disable(void)
{
    PMU_CTL &= ~PMU_CTL_SRAMSW2;
}

/*!
    \brief      PMU work in sleep mode
    \param[in]  sleepmodecmd:
                only one parameter can be selected which is shown as below:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_sleepmode(uint8_t sleepmodecmd)
{
    /* clear sleepdeep bit of Cortex-M33 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);

    /* select WFI or WFE command to enter sleep mode */
    if(WFI_CMD == sleepmodecmd) {
        __WFI();
    } else {
        __SEV();
        __WFE();
        __WFE();
    }
}

/*!
    \brief      PMU work in deepsleep mode
    \param[in]  ldo:
                only one parameter can be selected which is shown as below:
      \arg        PMU_LDO_NORMAL: LDO work in normal power mode when pmu enter deepsleep mode
      \arg        PMU_LDO_LOWPOWER: LDO work in low power mode when pmu enter deepsleep mode
    \param[in]  lowdrive:
                only one parameter can be selected which is shown as below:
      \arg        PMU_LOWDRIVER_ENABLE: low-driver mode enable in deep-sleep mode
      \arg        PMU_LOWDRIVER_DISABLE: low-driver mode disable in deep-sleep mode
    \param[in]  deepsleepmodecmd:
                only one parameter can be selected which is shown as below:
      \arg        WFI_CMD: use WFI command
      \arg        WFE_CMD: use WFE command
    \param[out] none
    \retval     none
*/
void pmu_to_deepsleepmode(uint32_t ldo, uint32_t lowdrive, uint8_t deepsleepmodecmd)
{
    static uint32_t reg_snap[4];
    /* flash goto sleep mode when MCU enters deepsleep mode */
    REG32(0x40022000) |= (uint32_t)(1<<14);

    /* clear stbmod and ldolp bits */
    PMU_CTL &= ~((uint32_t)(PMU_CTL_STBMOD | PMU_CTL_LDOLP | PMU_CTL_LDEN));

    /* set ldolp bit according to pmu_ldo */
    PMU_CTL |= (ldo | lowdrive);

    /* set sleepdeep bit of Cortex-M33 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    reg_snap[0] = REG32(0xE000E010U);
    reg_snap[1] = REG32(0xE000E100U);
    reg_snap[2] = REG32(0xE000E104U);
    reg_snap[3] = REG32(0xE000E108U);

    REG32(0xE000E010U) &= 0x00010004U;
    REG32(0xE000E180U)  = 0xFDFFF835U;
    REG32(0xE000E184U)  = 0xBFF5FC1FU;
    REG32(0xE000E188U)  = 0xFFFFFFFFU;

    /* select WFI or WFE command to enter deepsleep mode */
    if(WFI_CMD == deepsleepmodecmd) {
        __WFI();
    } else {
        __SEV();
        __WFE();
        __WFE();
    }

    REG32(0xE000E010U) = reg_snap[0];
    REG32(0xE000E100U) = reg_snap[1];
    REG32(0xE000E104U) = reg_snap[2];
    REG32(0xE000E108U) = reg_snap[3];

    /* reset sleepdeep bit of Cortex-M33 system control register */
    SCB->SCR &= ~((uint32_t)SCB_SCR_SLEEPDEEP_Msk);
}

/*!
    \brief      pmu work in standby mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_to_standbymode(void)
{
    /* set stbmod bit */
    PMU_CTL |= PMU_CTL_STBMOD;

    /* reset wakeup flag */
    PMU_CTL |= PMU_CTL_WURST;

    /* set sleepdeep bit of Cortex-M33 system control register */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    REG32(0xE000E010U) &= 0x00010004U;
    REG32(0xE000E180U)  = 0XFFFFFFF7U;
    REG32(0xE000E184U)  = 0XFFFFFFFFU;
    REG32(0xE000E188U)  = 0xFFFFFFFFU;

    /* select WFI or WFE command to enter standby mode */
    __WFI();
}

/*!
    \brief      enable wakeup pin
    \param[in]  wakeup_pin:
                one or more parameters can be selected which are shown as below:
      \arg        PMU_WAKEUP_PIN0: WKUP Pin 0 (PA0)
      \arg        PMU_WAKEUP_PIN1: WKUP Pin 1 (PC13)
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_enable(uint32_t wakeup_pin)
{
    PMU_CS |= wakeup_pin;
}

/*!
    \brief      disable wakeup pin
    \param[in]  wakeup_pin:
                one or more parameters can be selected which are shown as below:
      \arg        PMU_WAKEUP_PIN0: WKUP Pin 0 (PA0)
      \arg        PMU_WAKEUP_PIN1: WKUP Pin 1 (PC13)
    \param[out] none
    \retval     none
*/
void pmu_wakeup_pin_disable(uint32_t wakeup_pin)
{
    PMU_CS &= ~(wakeup_pin);
}

/*!
    \brief      enable write access to the registers in backup domain
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_enable(void)
{
    PMU_CTL |= PMU_CTL_BKPWEN;
}

/*!
    \brief      disable write access to the registers in backup domain
    \param[in]  none
    \param[out] none
    \retval     none
*/
void pmu_backup_write_disable(void)
{
    PMU_CTL &= ~PMU_CTL_BKPWEN;
}

/*!
    \brief      get flag state
    \param[in]  flag:
                only one parameter can be selected which is shown as below:
      \arg        PMU_FLAG_WAKEUP: wakeup flag
      \arg        PMU_FLAG_STANDBY: standby flag
      \arg        PMU_FLAG_LVD: lvd flag
      \arg        PMU_FLAG_OVD: ovd flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus pmu_flag_get(uint32_t flag)
{
    if(0U != (PMU_CS & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear flag bit
    \param[in]  flag:
                only one parameter can be selected which is shown as below:
      \arg        PMU_FLAG_RESET_WAKEUP: reset wakeup flag
      \arg        PMU_FLAG_RESET_STANDBY: reset standby flag
    \param[out] none
    \retval     none
*/
void pmu_flag_clear(uint32_t flag)
{
    switch(flag) {
    case PMU_FLAG_RESET_WAKEUP:
        /* reset wakeup flag */
        PMU_CTL |= PMU_CTL_WURST;
        break;
    case PMU_FLAG_RESET_STANDBY:
        /* reset standby flag */
        PMU_CTL |= PMU_CTL_STBRST;
        break;
    default:
        break;
    }
}
