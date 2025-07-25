/*!
    \file    gd32e502_dma.c
    \brief   DMA driver

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

#include "gd32e502_dma.h"
#include <stdlib.h>

#define DMA_WRONG_HANDLE        while(1){}

/* DMA functions */
/*!
    \brief      deinitialize DMA channel registers 
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel is deinitialized
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none
*/
void dma_deinit(uint32_t dma_periph, dma_channel_enum channelx)
{
    /* disable DMA a channel */
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
    /* reset DMA channel registers */
    DMA_CHCTL(dma_periph, channelx) = DMA_CHCTL_RESET_VALUE;
    DMA_CHCNT(dma_periph, channelx) = DMA_CHCNT_RESET_VALUE;
    DMA_CHPADDR(dma_periph, channelx) = DMA_CHPADDR_RESET_VALUE;
    DMA_CHMADDR(dma_periph, channelx) = DMA_CHMADDR_RESET_VALUE;
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(DMA_CHINTF_RESET_VALUE, channelx);
}

/*!
    \brief      initialize the parameters of DMA struct with the default values
    \param[in]  init_struct: the initialization data needed to initialize DMA channel
    \param[out] none
    \retval     none
*/
void dma_struct_para_init(dma_parameter_struct* init_struct)
{
    if(NULL == init_struct) {
        DMA_WRONG_HANDLE
    }
    
    /* set the DMA struct with the default values */
    init_struct->periph_addr  = 0U;
    init_struct->periph_width = 0U; 
    init_struct->periph_inc   = (uint8_t)DMA_PERIPH_INCREASE_DISABLE;
    init_struct->memory_addr  = 0U;
    init_struct->memory_width = 0U;
    init_struct->memory_inc   = (uint8_t)DMA_MEMORY_INCREASE_DISABLE;
    init_struct->number       = 0U;
    init_struct->direction    = (uint8_t)DMA_PERIPHERAL_TO_MEMORY;
    init_struct->priority     = (uint32_t)DMA_PRIORITY_LOW;
    init_struct->request      = DMA_REQUEST_M2M;
}

/*!
    \brief      initialize DMA channel
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel is initialized
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  init_struct: the data needed to initialize DMA channel
                  periph_addr: peripheral base address
                  periph_width: DMA_PERIPHERAL_WIDTH_8BIT,DMA_PERIPHERAL_WIDTH_16BIT,DMA_PERIPHERAL_WIDTH_32BIT
                  periph_inc: DMA_PERIPH_INCREASE_ENABLE,DMA_PERIPH_INCREASE_DISABLE 
                  memory_addr: memory base address
                  memory_width: DMA_MEMORY_WIDTH_8BIT,DMA_MEMORY_WIDTH_16BIT,DMA_MEMORY_WIDTH_32BIT
                  memory_inc: DMA_MEMORY_INCREASE_ENABLE,DMA_MEMORY_INCREASE_DISABLE
                  direction: DMA_PERIPHERAL_TO_MEMORY,DMA_MEMORY_TO_PERIPHERAL
                  number: the number of remaining data to be transferred by the DMA
                  priority: DMA_PRIORITY_LOW,DMA_PRIORITY_MEDIUM,DMA_PRIORITY_HIGH,DMA_PRIORITY_ULTRA_HIGH
                  request: DMA_REQUEST_M2M,          DMA_REQUEST_GENERATOR0,   DMA_REQUEST_GENERATOR1,   DMA_REQUEST_GENERATOR2
                           DMA_REQUEST_GENERATOR3,   DMA_REQUEST_ADC,          DMA_REQUEST_DAC_CH0,      DMA_REQUEST_I2C1_RX
                           DMA_REQUEST_I2C1_TX,      DMA_REQUEST_I2C0_RX,      DMA_REQUEST_I2C0_TX,      DMA_REQUESR_SSTAT0
                           DMA_REQUESR_SSTAT1,       DMA_REQUESR_SSTAT2,       DMA_REQUESR_SSTAT3,       DMA_REQUEST_SPI0_RX
                           DMA_REQUEST_SPI0_TX,      DMA_REQUEST_SPI1_RX,      DMA_REQUEST_SPI1_TX,      DMA_REQUEST_TIMER0_CH0
                           DMA_REQUEST_TIMER0_CH1,   DMA_REQUEST_TIMER0_CH2,   DMA_REQUEST_TIMER0_CH3,   DMA_REQUEST_TIMER0_TI
                           DMA_REQUEST_TIMER0_UP,    DMA_REQUEST_TIMER0_CO,    DMA_REQUEST_TIMER0_MCH0,  DMA_REQUEST_TIMER0_MCH1
                           DMA_REQUEST_TIMER0_MCH2,  DMA_REQUEST_TIMER0_MCH3,  DMA_REQUEST_TIMER1_CH0,   DMA_REQUEST_TIMER1_CH1
                           DMA_REQUEST_TIMER1_CH2,   DMA_REQUEST_TIMER1_CH3,   DMA_REQUEST_TIMER1_TI,    DMA_REQUEST_TIMER1_UP
                           DMA_REQUEST_TIMER7_CH0,   DMA_REQUEST_TIMER7_CH1,   DMA_REQUEST_TIMER7_CH2,   DMA_REQUEST_TIMER7_CH3
                           DMA_REQUEST_TIMER7_TI,    DMA_REQUEST_TIMER7_UP,    DMA_REQUEST_TIMER7_CO,    DMA_REQUEST_TIMER7_MCH0
                           DMA_REQUEST_TIMER7_MCH1,  DMA_REQUEST_TIMER7_MCH2,  DMA_REQUEST_TIMER7_MCH3,  DMA_REQUEST_CAN1
                           DMA_REQUEST_CAN0,         DMA_REQUEST_USART0_RX,    DMA_REQUEST_USART0_TX,    DMA_REQUEST_USART1_RX
                           DMA_REQUEST_USART1_TX,    DMA_REQUEST_USART2_RX,    DMA_REQUEST_USART2_TX,    DMA_REQUEST_TIMER5_UP
                           DMA_REQUEST_TIMER6_UP,    DMA_REQUEST_TIMER19_CH0,  DMA_REQUEST_TIMER19_CH1,  DMA_REQUEST_TIMER19_CH2
                           DMA_REQUEST_TIMER19_CH3,  DMA_REQUEST_TIMER19_TI,   DMA_REQUEST_TIMER19_UP,   DMA_REQUEST_TIMER19_CO
                           DMA_REQUEST_TIMER19_MCH0, DMA_REQUEST_TIMER19_MCH1, DMA_REQUEST_TIMER19_MCH2, DMA_REQUEST_TIMER19_MCH3
                           DMA_REQUEST_TIMER20_CH0,  DMA_REQUEST_TIMER20_CH1,  DMA_REQUEST_TIMER20_CH2,  DMA_REQUEST_TIMER20_CH3
                           DMA_REQUEST_TIMER20_TI,   DMA_REQUEST_TIMER20_UP,   DMA_REQUEST_TIMER20_CO,   DMA_REQUEST_TIMER20_MCH0
                           DMA_REQUEST_TIMER20_MCH1, DMA_REQUEST_TIMER20_MCH2, DMA_REQUEST_TIMER20_MCH3
    \param[out] none
    \retval     none
*/
void dma_init(uint32_t dma_periph, dma_channel_enum channelx, dma_parameter_struct* init_struct)
{
    uint32_t ctl;

    if(NULL == init_struct) {
        DMA_WRONG_HANDLE
    }

    dma_channel_disable(dma_periph, channelx);

    /* configure peripheral base address */
    DMA_CHPADDR(dma_periph, channelx) = init_struct->periph_addr;

    /* configure memory base address */
    DMA_CHMADDR(dma_periph, channelx) = init_struct->memory_addr;

    /* configure the number of remaining data to be transferred */
    DMA_CHCNT(dma_periph, channelx) = (init_struct->number & DMA_CHANNEL_CNT_MASK);

    /* configure peripheral transfer width, memory transfer width, channel priotity */
    ctl = DMA_CHCTL(dma_periph, channelx);
    ctl &= ~(DMA_CHXCTL_PWIDTH | DMA_CHXCTL_MWIDTH | DMA_CHXCTL_PRIO);
    ctl |= (init_struct->periph_width | init_struct->memory_width | init_struct->priority);
    DMA_CHCTL(dma_periph, channelx) = ctl;

    /* configure peripheral increasing mode */
    if(DMA_PERIPH_INCREASE_ENABLE == init_struct->periph_inc) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
    } else {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
    }

    /* configure memory increasing mode */
    if(DMA_MEMORY_INCREASE_ENABLE == init_struct->memory_inc) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
    } else {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
    }

    /* configure the direction of  data transfer */
    if(DMA_PERIPHERAL_TO_MEMORY == init_struct->direction) {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_DIR;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_DIR;
    }

    if(DMA0 == dma_periph) {
        DMAMUX_RM_CHXCFG((uint32_t)channelx) &= ~DMAMUX_RM_CHXCFG_MUXID;
        DMAMUX_RM_CHXCFG((uint32_t)channelx) |= init_struct->request;
    } else {
        DMAMUX_RM_CHXCFG((uint32_t)channelx + 7U ) &= ~DMAMUX_RM_CHXCFG_MUXID;
        DMAMUX_RM_CHXCFG((uint32_t)channelx + 7U ) |= init_struct->request;
    }
}

/*!
    \brief      enable DMA circulation mode
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none 
*/
void dma_circulation_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CMEN;
}

/*!
    \brief      disable DMA circulation mode
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none 
*/
void dma_circulation_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CMEN;
}

/*!
    \brief      enable memory to memory mode
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none
*/
void dma_memory_to_memory_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_M2M;
}

/*!
    \brief      disable memory to memory mode
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none
*/
void dma_memory_to_memory_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_M2M;
}

/*!
    \brief      enable DMA channel
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none 
*/
void dma_channel_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CHEN;
}

/*!
    \brief      disable DMA channel
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none 
*/
void dma_channel_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
}

/*!
    \brief      set DMA peripheral base address
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set peripheral base address 
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  address: peripheral base address
    \param[out] none
    \retval     none 
*/
void dma_periph_address_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t address)
{
    DMA_CHPADDR(dma_periph, channelx) = address;
}

/*!
    \brief      set DMA memory base address
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set memory base address 
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  address: memory base address
    \param[out] none
    \retval     none 
*/
void dma_memory_address_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t address)
{
    DMA_CHMADDR(dma_periph, channelx) = address;
}

/*!
    \brief      set the number of remaining data to be transferred by the DMA
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set number 
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  number: the number of remaining data to be transferred by the DMA
    \param[out] none
    \retval     none 
*/
void dma_transfer_number_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t number)
{
    DMA_CHCNT(dma_periph, channelx) = (number & DMA_CHANNEL_CNT_MASK);
}

/*!
    \brief      get the number of remaining data to be transferred by the DMA
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set number 
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     the number of remaining data to be transferred by the DMA 
*/
uint32_t dma_transfer_number_get(uint32_t dma_periph, dma_channel_enum channelx)
{
    return (uint32_t)DMA_CHCNT(dma_periph, channelx);
}

/*!
    \brief      configure priority level of DMA channel
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  priority: priority level of this channel
                only one parameter can be selected which is shown as below:
      \arg        DMA_PRIORITY_LOW: low priority
      \arg        DMA_PRIORITY_MEDIUM: medium priority
      \arg        DMA_PRIORITY_HIGH: high priority
      \arg        DMA_PRIORITY_ULTRA_HIGH: ultra high priority
    \param[out] none
    \retval     none
*/
void dma_priority_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t priority)
{
    uint32_t ctl;

    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PRIO;
    ctl |= priority;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    \brief      configure transfer data width of memory
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  mwidth: transfer data width of memory
                only one parameter can be selected which is shown as below:
      \arg        DMA_MEMORY_WIDTH_8BIT: transfer data width of memory is 8-bit
      \arg        DMA_MEMORY_WIDTH_16BIT: transfer data width of memory is 16-bit
      \arg        DMA_MEMORY_WIDTH_32BIT: transfer data width of memory is 32-bit
    \param[out] none
    \retval     none
*/
void dma_memory_width_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t mwidth)
{
    uint32_t ctl;
    
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_MWIDTH;
    ctl |= mwidth;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    \brief      configure transfer data width of peripheral
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  pwidth: transfer data width of peripheral
                only one parameter can be selected which is shown as below:
      \arg        DMA_PERIPHERAL_WIDTH_8BIT: transfer data width of peripheral is 8-bit
      \arg        DMA_PERIPHERAL_WIDTH_16BIT: transfer data width of peripheral is 16-bit
      \arg        DMA_PERIPHERAL_WIDTH_32BIT: transfer data width of peripheral is 32-bit
    \param[out] none
    \retval     none
*/
void dma_periph_width_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t pwidth)
{
    uint32_t ctl;

    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PWIDTH;
    ctl |= pwidth;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    \brief      enable next address increasement algorithm of memory
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none
*/
void dma_memory_increase_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
}

/*!
    \brief      disable next address increasement algorithm of memory
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none
*/
void dma_memory_increase_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
}

/*!
    \brief      enable next address increasement algorithm of peripheral
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none
*/
void dma_periph_increase_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
}

/*!
    \brief      disable next address increasement algorithm of peripheral
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[out] none
    \retval     none
*/
void dma_periph_increase_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
}

/*!
    \brief      configure the direction of data transfer on the channel  
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  direction: specify the direction of  data transfer
                only one parameter can be selected which is shown as below:
      \arg        DMA_PERIPHERAL_TO_MEMORY: read from peripheral and write to memory
      \arg        DMA_MEMORY_TO_PERIPHERAL: read from memory and write to peripheral
    \param[out] none
    \retval     none
*/
void dma_transfer_direction_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t direction)
{
    if(DMA_PERIPHERAL_TO_MEMORY == direction) {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_DIR;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_DIR;
    }
}

/*!
    \brief      check DMA flag is set or not
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to get flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_FLAG_G: global interrupt flag of channel
      \arg        DMA_FLAG_FTF: full transfer finish flag of channel
      \arg        DMA_FLAG_HTF: half transfer finish flag of channel
      \arg        DMA_FLAG_ERR: error flag of channel
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dma_flag_get(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    FlagStatus reval;

    if(0U != (DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx))) {
        reval = SET;
    } else {
        reval = RESET;
    }
    
    return reval;
}

/*!
    \brief      clear a DMA channel flag
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to clear flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_FLAG_G: global interrupt flag of channel
      \arg        DMA_FLAG_FTF: full transfer finish flag of channel
      \arg        DMA_FLAG_HTF: half transfer finish flag of channel
      \arg        DMA_FLAG_ERR: error flag of channel
    \param[out] none
    \retval     none
*/
void dma_flag_clear(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
}

/*!
    \brief      enable DMA interrupt
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  source: specify which interrupt to enable
                only one parameter can be selected which is shown as below:
      \arg        DMA_INT_ERR: channel error interrupt
      \arg        DMA_INT_HTF: channel half transfer finish interrupt
      \arg        DMA_INT_FTF: channel full transfer finish interrupt
    \param[out] none
    \retval     none
*/
void dma_interrupt_enable(uint32_t dma_periph, dma_channel_enum channelx, uint32_t source)
{
    DMA_CHCTL(dma_periph, channelx) |= source;
}

/*!
    \brief      disable DMA interrupt
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to set
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  source: specify which interrupt to disable
                only one parameter can be selected which is shown as below:
      \arg        DMA_INT_ERR: channel error interrupt
      \arg        DMA_INT_HTF: channel half transfer finish interrupt
      \arg        DMA_INT_FTF: channel full transfer finish interrupt
    \param[out] none
    \retval     none
*/
void dma_interrupt_disable(uint32_t dma_periph, dma_channel_enum channelx, uint32_t source)
{
    DMA_CHCTL(dma_periph, channelx) &= ~source;
}

/*!
    \brief      check DMA flag and interrupt enable bit is set or not
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to get flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_INT_FLAG_FTF: transfer finish flag of channel
      \arg        DMA_INT_FLAG_HTF: half transfer finish flag of channel
      \arg        DMA_INT_FLAG_ERR: error flag of channel
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dma_interrupt_flag_get(uint32_t dma_periph, dma_channel_enum channelx, uint32_t int_flag)
{
    uint32_t interrupt_enable = 0U, interrupt_flag = 0U;
    
    switch(int_flag) {
        case DMA_INT_FLAG_FTF:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(int_flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_FTFIE;
            break;
        case DMA_INT_FLAG_HTF:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(int_flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_HTFIE;
            break;
        case DMA_INT_FLAG_ERR:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(int_flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_ERRIE;
            break;
        default:
            break;
        }

    if(interrupt_flag && interrupt_enable) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    \brief      clear a DMA channel interrupt flag
    \param[in]  dma_periph: DMAx(x=0,1)
    \param[in]  channelx: specify which DMA channel to clear flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_CHx(x=0..6 for DMA0, x = 0..4 for DMA1)
    \param[in]  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      \arg        DMA_INT_FLAG_G: global interrupt flag of channel
      \arg        DMA_INT_FLAG_FTF: transfer finish flag of channel
      \arg        DMA_INT_FLAG_HTF: half transfer finish flag of channel
      \arg        DMA_INT_FLAG_ERR: error flag of channel
    \param[out] none
    \retval     none
*/
void dma_interrupt_flag_clear(uint32_t dma_periph, dma_channel_enum channelx, uint32_t int_flag)
{
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(int_flag, channelx);
}

/* DMAMUX functions */
/*!
    \brief      initialize the parameters of DMAMUX synchronization mode structure with the default values
    \param[in]  none
    \param[out] init_struct: the initialization data needed to initialize DMAMUX request multiplexer channel synchronization mode
    \retval     none
*/
void dmamux_sync_struct_para_init(dmamux_sync_parameter_struct *init_struct)
{
    if(NULL == init_struct) {
        DMA_WRONG_HANDLE
    }

    /* set the DMAMUX synchronization struct with the default values */
    init_struct->sync_id        = DMAMUX_SYNC_EVTX_OUT0;
    init_struct->sync_polarity  = DMAMUX_SYNC_RISING;
    init_struct->request_number = 1U;
}

/*!
    \brief      initialize DMAMUX request multiplexer channel synchronization mode
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is initialized
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MULTIPLEXER_CHx(x=0..11)
    \param[in]  init_struct: the data needed to initialize DMAMUX request multiplexer channel
                  sync_id: DMAMUX_SYNC_EXTI0, DMAMUX_SYNC_EXTI1, DMAMUX_SYNC_EXTI2, DMAMUX_SYNC_EXTI3,
                           DMAMUX_SYNC_EXTI4, DMAMUX_SYNC_EXTI5, DMAMUX_SYNC_EXTI6, DMAMUX_SYNC_EXTI7,
                           DMAMUX_SYNC_EXTI8, DMAMUX_SYNC_EXTI9, DMAMUX_SYNC_EXTI10, DMAMUX_SYNC_EXTI11,
                           DMAMUX_SYNC_EXTI12, DMAMUX_SYNC_EXTI13, DMAMUX_SYNC_EXTI14, DMAMUX_SYNC_EXTI15,
                           DMAMUX_SYNC_EVTX_OUT0, DMAMUX_SYNC_EVTX_OUT1, DMAMUX_SYNC_EVTX_OUT2, DMAMUX_SYNC_EVTX_OUT3,
                           DMAMUX_SYNC_TIMER20_CH0_O
                  sync_polarity: DMAMUX_SYNC_NO_EVENT, DMAMUX_SYNC_RISING, DMAMUX_SYNC_FALLING, DMAMUX_SYNC_RISING_FALLING
                  request_number: the number of DMA request that will be authorized after a sync event, from 1 to 32
    \param[out] none
    \retval     none
*/
void dmamux_synchronization_init(dmamux_multiplexer_channel_enum channelx, dmamux_sync_parameter_struct *init_struct)
{
    uint32_t cfg;

    if(NULL == init_struct) {
        DMA_WRONG_HANDLE
    }

    /* disable synchronization mode and event generation for DMA request forward number configuration */
    DMAMUX_RM_CHXCFG(channelx) &= ~(DMAMUX_RM_CHXCFG_SYNCEN | DMAMUX_RM_CHXCFG_EVGEN);

    /* configure synchronization input identification, synchronization input polarity, number of DMA requests to forward */
    cfg = DMAMUX_RM_CHXCFG(channelx);
    cfg &= ~(DMAMUX_RM_CHXCFG_SYNCID | DMAMUX_RM_CHXCFG_NBR | DMAMUX_RM_CHXCFG_SYNCP);
    cfg |= (init_struct->sync_polarity | (init_struct->sync_id) | RM_CHXCFG_NBR(init_struct->request_number - 1U));
    DMAMUX_RM_CHXCFG(channelx) = cfg;
}

/*!
    \brief      enable synchronization mode
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MULTIPLEXER_CHx(x=0..11)
    \param[out] none
    \retval     none
*/
void dmamux_synchronization_enable(dmamux_multiplexer_channel_enum channelx)
{
    DMAMUX_RM_CHXCFG(channelx) |= DMAMUX_RM_CHXCFG_SYNCEN;
}

/*!
    \brief      disable synchronization mode
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MULTIPLEXER_CHx(x=0..11)
    \param[out] none
    \retval     none
*/
void dmamux_synchronization_disable(dmamux_multiplexer_channel_enum channelx)
{
    DMAMUX_RM_CHXCFG(channelx) &= (~DMAMUX_RM_CHXCFG_SYNCEN);
}

/*!
    \brief      enable event generation
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MULTIPLEXER_CHx(x=0..11)
    \param[out] none
    \retval     none
*/
void dmamux_event_generation_enable(dmamux_multiplexer_channel_enum channelx)
{
    DMAMUX_RM_CHXCFG(channelx) |= DMAMUX_RM_CHXCFG_EVGEN;
}

/*!
    \brief      disable event generation
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MULTIPLEXER_CHx(x=0..11)
    \param[out] none
    \retval     none
*/
void dmamux_event_generation_disable(dmamux_multiplexer_channel_enum channelx)
{
    DMAMUX_RM_CHXCFG(channelx) &= (~DMAMUX_RM_CHXCFG_EVGEN);
}

/*!
    \brief      initialize the parameters of DMAMUX request generator structure with the default values
    \param[in]  init_struct: the initialization data needed to initialize DMAMUX request generator channel
    \param[out] none
    \retval     none
*/
void dmamux_gen_struct_para_init(dmamux_gen_parameter_struct *init_struct)
{
    if(NULL == init_struct) {
        DMA_WRONG_HANDLE
    }

    /* set the DMAMUX request generator structure with the default values */
    init_struct->trigger_id        = DMAMUX_TRIGGER_EVTX_OUT0;
    init_struct->trigger_polarity  = DMAMUX_GEN_RISING;
    init_struct->request_number = 1U;
}

/*!
    \brief      initialize DMAMUX request generator channel
    \param[in]  channelx: specify which DMAMUX request generator channel is initialized
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_GENCHx(x=0..3)
    \param[in]  init_struct: the data needed to initialize DMAMUX request generator channel
                  trigger_id: DMAMUX_TRIGGER_EXTI0, DMAMUX_TRIGGER_EXTI1, DMAMUX_TRIGGER_EXTI2, DMAMUX_TRIGGER_EXTI3,
                              DMAMUX_TRIGGER_EXTI4, DMAMUX_TRIGGER_EXTI5, DMAMUX_TRIGGER_EXTI6, DMAMUX_TRIGGER_EXTI7,
                              DMAMUX_TRIGGER_EXTI8, DMAMUX_TRIGGER_EXTI9, DMAMUX_TRIGGER_EXTI10, DMAMUX_TRIGGER_EXTI11,
                              DMAMUX_TRIGGER_EXTI12, DMAMUX_TRIGGER_EXTI13, DMAMUX_TRIGGER_EXTI14, DMAMUX_TRIGGER_EXTI15,
                              DMAMUX_TRIGGER_EVTX_OUT0, DMAMUX_TRIGGER_EVTX_OUT0, DMAMUX_TRIGGER_EVTX_OUT0, DMAMUX_TRIGGER_EVTX_OUT0,
                              DMAMUX_TRIGGER_TIMER20_CH0_O
                  trigger_polarity: DMAMUX_GEN_NO_EVENT, DMAMUX_GEN_RISING, DMAMUX_GEN_FALLING, DMAMUX_GEN_RISING_FALLING
                  request_number: the number of DMA request that will be generated after a signal event, from 1 to 32
    \param[out] none
    \retval     none
*/
void dmamux_request_generator_init(dmamux_generator_channel_enum channelx, dmamux_gen_parameter_struct *init_struct)
{
    uint32_t cfg;

    if(NULL == init_struct) {
        DMA_WRONG_HANDLE
    }

    /* disable DMAMUX request generator channel for DMA request generation number configuration */
    DMAMUX_RG_CHXCFG(channelx) &= ~(DMAMUX_RG_CHXCFG_RGEN);

    /* configure trigger input identification, trigger polarity, number of DMA requests to be generated */
    cfg = DMAMUX_RG_CHXCFG(channelx);
    cfg &= ~(DMAMUX_RG_CHXCFG_TID | DMAMUX_RG_CHXCFG_NBRG | DMAMUX_RG_CHXCFG_RGTP);
    cfg |= (RG_CHXCFG_NBRG(init_struct->request_number - 1U) | init_struct->trigger_id | init_struct->trigger_polarity);
    DMAMUX_RG_CHXCFG(channelx) = cfg;
}

/*!
    \brief      enable DMAMUX request generator channel
    \param[in]  channelx: specify which DMAMUX request generator channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_GENCHx(x=0..3)
    \param[out] none
    \retval     none
*/
void dmamux_request_generator_channel_enable(dmamux_generator_channel_enum channelx)
{
    DMAMUX_RG_CHXCFG(channelx) |= DMAMUX_RG_CHXCFG_RGEN;
}

/*!
    \brief      disable DMAMUX request generator channel
    \param[in]  channelx: specify which DMAMUX request generator channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_GENCHx(x=0..3)
    \param[out] none
    \retval     none
*/
void dmamux_request_generator_channel_disable(dmamux_generator_channel_enum channelx)
{
    DMAMUX_RG_CHXCFG(channelx) &= (~DMAMUX_RG_CHXCFG_RGEN);
}

/*!
    \brief      configure synchronization input polarity
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MUXCHx(x=0..11)
    \param[in]  polarity: synchronization input polarity
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_SYNC_NO_EVENT: no event detection
      \arg        DMAMUX_SYNC_RISING: rising edge
      \arg        DMAMUX_SYNC_FALLING: falling edge
      \arg        DMAMUX_SYNC_RISING_FALLING: rising and falling edges
    \param[out] none
    \retval     none
*/
void dmamux_synchronization_polarity_config(dmamux_multiplexer_channel_enum channelx, uint32_t polarity)
{
    DMAMUX_RM_CHXCFG(channelx) &= ~DMAMUX_RM_CHXCFG_SYNCP;
    DMAMUX_RM_CHXCFG(channelx) |= polarity;
}

/*!
    \brief      configure number of DMA requests to forward
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MUXCHx(x=0..6)
    \param[in]  number: DMA requests number to forward
                only one parameter can be selected which is shown as below:
      \arg        1 - 32
    \param[out] none
    \retval     none
*/
void dmamux_request_forward_number_config(dmamux_multiplexer_channel_enum channelx, uint32_t number)
{
    DMAMUX_RM_CHXCFG(channelx) &= ~DMAMUX_RM_CHXCFG_NBR;
    DMAMUX_RM_CHXCFG(channelx) |= RM_CHXCFG_NBR(number - 1U);
}

/*!
    \brief      configure synchronization input identification
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MUXCHx(x=0..11)
    \param[in]  id: synchronization input identification
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_SYNC_EXTI0: synchronization input is EXTI0
      \arg        DMAMUX_SYNC_EXTI1: synchronization input is EXTI1
      \arg        DMAMUX_SYNC_EXTI2: synchronization input is EXTI2
      \arg        DMAMUX_SYNC_EXTI3: synchronization input is EXTI3
      \arg        DMAMUX_SYNC_EXTI4: synchronization input is EXTI4
      \arg        DMAMUX_SYNC_EXTI5: synchronization input is EXTI5
      \arg        DMAMUX_SYNC_EXTI6: synchronization input is EXTI6
      \arg        DMAMUX_SYNC_EXTI7: synchronization input is EXTI7
      \arg        DMAMUX_SYNC_EXTI8: synchronization input is EXTI8
      \arg        DMAMUX_SYNC_EXTI9: synchronization input is EXTI9
      \arg        DMAMUX_SYNC_EXTI10: synchronization input is EXTI10
      \arg        DMAMUX_SYNC_EXTI11: synchronization input is EXTI11
      \arg        DMAMUX_SYNC_EXTI12: synchronization input is EXTI12
      \arg        DMAMUX_SYNC_EXTI13: synchronization input is EXTI13
      \arg        DMAMUX_SYNC_EXTI14: synchronization input is EXTI14
      \arg        DMAMUX_SYNC_EXTI15: synchronization input is EXTI15
      \arg        DMAMUX_SYNC_EVTX_OUT0: synchronization input is Evtx_out0
      \arg        DMAMUX_SYNC_EVTX_OUT1: synchronization input is Evtx_out1
      \arg        DMAMUX_SYNC_EVTX_OUT2: synchronization input is Evtx_out2
      \arg        DMAMUX_SYNC_EVTX_OUT3: synchronization input is Evtx_out3
      \arg        DMAMUX_SYNC_TIMER20_CH0_O: synchronization input is TIMER20_CH0_O
    \param[out] none
    \retval     none
*/
void dmamux_sync_id_config(dmamux_multiplexer_channel_enum channelx, uint32_t id)
{
    DMAMUX_RM_CHXCFG(channelx) &= ~DMAMUX_RM_CHXCFG_SYNCID;
    DMAMUX_RM_CHXCFG(channelx) |= id;
}

/*!
    \brief      configure multiplexer input identification
    \param[in]  channelx: specify which DMAMUX request multiplexer channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_MUXCHx(x=0..11)
    \param[in]  id: input DMA request identification
                only one parameter can be selected which is shown as below:
      \arg        DMA_REQUEST_M2M: memory to memory transfer
      \arg        DMA_REQUEST_GENERATOR0: DMAMUX request generator 0
      \arg        DMA_REQUEST_GENERATOR1: DMAMUX request generator 1
      \arg        DMA_REQUEST_GENERATOR2: DMAMUX request generator 2
      \arg        DMA_REQUEST_GENERATOR3: DMAMUX request generator 3
      \arg        DMA_REQUEST_ADC: DMAMUX ADC request
      \arg        DMA_REQUEST_DAC_CH0: DMAMUX DAC_CH0 request
      \arg        DMA_REQUEST_I2C1_RX: DMAMUX I2C1 RX request
      \arg        DMA_REQUEST_I2C1_TX: DMAMUX I2C1 TX request
      \arg        DMA_REQUEST_I2C0_RX: DMAMUX I2C0 RX request
      \arg        DMA_REQUEST_I2C0_TX: DMAMUX I2C0 TX request
      \arg        DMA_REQUESR_SSTAT0: DMAMUX SSTAT0 request
      \arg        DMA_REQUESR_SSTAT1: DMAMUX SSTAT1 request
      \arg        DMA_REQUESR_SSTAT2: DMAMUX SSTAT2 request
      \arg        DMA_REQUESR_SSTAT3: DMAMUX SSTAT3 request
      \arg        DMA_REQUEST_SPI0_RX DMAMUX SPI0 RX request
      \arg        DMA_REQUEST_SPI0_TX: DMAMUX SPI0 TX request
      \arg        DMA_REQUEST_SPI1_RX: DMAMUX SPI1 RX request
      \arg        DMA_REQUEST_SPI1_TX: DMAMUX SPI1 TX request
      \arg        DMA_REQUEST_TIMER0_CH0: DMAMUX TIMER0 CH0 request
      \arg        DMA_REQUEST_TIMER0_CH1: DMAMUX TIMER0 CH1 request
      \arg        DMA_REQUEST_TIMER0_CH2: DMAMUX TIMER0 CH2 request
      \arg        DMA_REQUEST_TIMER0_CH3: DMAMUX TIMER0 CH3 request
      \arg        DMA_REQUEST_TIMER0_TI: DMAMUX TIMER0 TI request
      \arg        DMA_REQUEST_TIMER0_UP: DMAMUX TIMER0 UP request
      \arg        DMA_REQUEST_TIMER0_CO: DMAMUX TIMER0 CO request
      \arg        DMA_REQUEST_TIMER0_MCH0: DMAMUX TIMER0 MCH0 request
      \arg        DMA_REQUEST_TIMER0_MCH1: DMAMUX TIMER0 MCH1 request
      \arg        DMA_REQUEST_TIMER0_MCH2: DMAMUX TIMER0 MCH2 request
      \arg        DMA_REQUEST_TIMER0_MCH3: DMAMUX TIMER0 MCH3 request
      \arg        DMA_REQUEST_TIMER1_CH0: DMAMUX TIMER1 CH0 request
      \arg        DMA_REQUEST_TIMER1_CH1: DMAMUX TIMER1 CH1 request
      \arg        DMA_REQUEST_TIMER1_CH2: DMAMUX TIMER1 CH2 request
      \arg        DMA_REQUEST_TIMER1_CH3: DMAMUX TIMER1 CH3 request
      \arg        DMA_REQUEST_TIMER1_TI: DMAMUX TIMER1 TI request
      \arg        DMA_REQUEST_TIMER1_UP: DMAMUX TIMER1 UP request
      \arg        DMA_REQUEST_TIMER7_CH0: DMAMUX TIMER7 CH0 request
      \arg        DMA_REQUEST_TIMER7_CH1: DMAMUX TIMER7 CH1 request
      \arg        DMA_REQUEST_TIMER7_CH2: DMAMUX TIMER7 CH2 request
      \arg        DMA_REQUEST_TIMER7_CH3: DMAMUX TIMER7 CH3 request
      \arg        DMA_REQUEST_TIMER7_TI: DMAMUX TIMER7 TI request
      \arg        DMA_REQUEST_TIMER7_UP: DMAMUX TIMER7 UP request
      \arg        DMA_REQUEST_TIMER7_CO: DMAMUX TIMER7 CO request
      \arg        DMA_REQUEST_TIMER7_MCH0: DMAMUX TIMER7 MCH0 request
      \arg        DMA_REQUEST_TIMER7_MCH1: DMAMUX TIMER7 MCH1 request
      \arg        DMA_REQUEST_TIMER7_MCH2: DMAMUX TIMER7 MCH2 request
      \arg        DMA_REQUEST_TIMER7_MCH3: DMAMUX TIMER7 MCH3 request
      \arg        DMA_REQUEST_CAN1: DMAMUX CAN1 request
      \arg        DMA_REQUEST_CAN0: DMAMUX CAN0 request
      \arg        DMA_REQUEST_USART0_RX: DMAMUX USART0 RX request
      \arg        DMA_REQUEST_USART0_TX: DMAMUX USART0 TX request
      \arg        DMA_REQUEST_USART1_RX: DMAMUX USART1 RX request
      \arg        DMA_REQUEST_USART1_TX: DMAMUX USART1 TX request
      \arg        DMA_REQUEST_USART2_RX: DMAMUX USART2 RX request
      \arg        DMA_REQUEST_USART2_TX: DMAMUX USART2 TX request
      \arg        DMA_REQUEST_TIMER5_UP: DMAMUX TIMER5 UP request
      \arg        DMA_REQUEST_TIMER6_UP: DMAMUX TIMER6 UP request
      \arg        DMA_REQUEST_TIMER19_CH0: DMAMUX TIMER19 CH0 request
      \arg        DMA_REQUEST_TIMER19_CH1: DMAMUX TIMER19 CH1 request
      \arg        DMA_REQUEST_TIMER19_CH2: DMAMUX TIMER19 CH2 request
      \arg        DMA_REQUEST_TIMER19_CH3: DMAMUX TIMER19 CH3 request
      \arg        DMA_REQUEST_TIMER19_TI: DMAMUX TIMER19 TI request
      \arg        DMA_REQUEST_TIMER19_UP: DMAMUX TIMER19 UP request
      \arg        DMA_REQUEST_TIMER19_CO: DMAMUX TIMER19 CO request
      \arg        DMA_REQUEST_TIMER19_MCH0: DMAMUX TIMER19 MCH0 request
      \arg        DMA_REQUEST_TIMER19_MCH1: DMAMUX TIMER19 MCH1 request
      \arg        DMA_REQUEST_TIMER19_MCH2: DMAMUX TIMER19 MCH2 request
      \arg        DMA_REQUEST_TIMER19_MCH3: DMAMUX TIMER19 MCH3 request
      \arg        DMA_REQUEST_TIMER20_CH0: DMAMUX TIMER20 CH0 request
      \arg        DMA_REQUEST_TIMER20_CH1: DMAMUX TIMER20 CH1 request
      \arg        DMA_REQUEST_TIMER20_CH2: DMAMUX TIMER20 CH2 request
      \arg        DMA_REQUEST_TIMER20_CH3: DMAMUX TIMER20 CH3 request
      \arg        DMA_REQUEST_TIMER20_TI: DMAMUX TIMER20 TI request
      \arg        DMA_REQUEST_TIMER20_UP: DMAMUX TIMER20 UP request
      \arg        DMA_REQUEST_TIMER20_CO: DMAMUX TIMER20 CO request
      \arg        DMA_REQUEST_TIMER20_MCH0: DMAMUX TIMER20 MCH0 request
      \arg        DMA_REQUEST_TIMER20_MCH1: DMAMUX TIMER20 MCH1 request
      \arg        DMA_REQUEST_TIMER20_MCH2: DMAMUX TIMER20 MCH2 request
      \arg        DMA_REQUEST_TIMER20_MCH3: DMAMUX TIMER20 MCH3 request
    \param[out] none
    \retval     none
*/
void dmamux_request_id_config(dmamux_multiplexer_channel_enum channelx, uint32_t id)
{
    DMAMUX_RM_CHXCFG(channelx) &= ~DMAMUX_RM_CHXCFG_MUXID;
    DMAMUX_RM_CHXCFG(channelx) |= id;
}

/*!
    \brief      configure trigger input polarity
    \param[in]  channelx: specify which DMAMUX request generator channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_GENCHx(x=0..3)
    \param[in]  polarity: trigger input polarity
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_GEN_NO_EVENT: no event detection
      \arg        DMAMUX_GEN_RISING: rising edge
      \arg        DMAMUX_GEN_FALLING: falling edge
      \arg        DMAMUX_GEN_RISING_FALLING: rising and falling edges
    \param[out] none
    \retval     none
*/
void dmamux_trigger_polarity_config(dmamux_generator_channel_enum channelx, uint32_t polarity)
{
    DMAMUX_RG_CHXCFG(channelx) &= ~DMAMUX_RG_CHXCFG_RGTP;
    DMAMUX_RG_CHXCFG(channelx) |= polarity;
}

/*!
    \brief      configure number of DMA requests to be generated
    \param[in]  channelx: specify which DMAMUX request generator channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_GENCHx(x=0..3)
    \param[in]  number: DMA requests number to be generated
                only one parameter can be selected which is shown as below:
      \arg        1 - 32
    \param[out] none
    \retval     none
*/
void dmamux_request_generate_number_config(dmamux_generator_channel_enum channelx, uint32_t number)
{
    DMAMUX_RG_CHXCFG(channelx) &= ~DMAMUX_RG_CHXCFG_NBRG;
    DMAMUX_RG_CHXCFG(channelx) |= (number - 1U);
}

/*!
    \brief      configure trigger input identification
    \param[in]  channelx: specify which DMAMUX request generator channel is configured
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_GENCHx(x=0..3)
    \param[in]  id: trigger input identification
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_TRIGGER_EXTI0: trigger input is EXTI0
      \arg        DMAMUX_TRIGGER_EXTI1: trigger input is EXTI1
      \arg        DMAMUX_TRIGGER_EXTI2: trigger input is EXTI2
      \arg        DMAMUX_TRIGGER_EXTI3: trigger input is EXTI3
      \arg        DMAMUX_TRIGGER_EXTI4: trigger input is EXTI4
      \arg        DMAMUX_TRIGGER_EXTI5: trigger input is EXTI5
      \arg        DMAMUX_TRIGGER_EXTI6: trigger input is EXTI6
      \arg        DMAMUX_TRIGGER_EXTI7: trigger input is EXTI7
      \arg        DMAMUX_TRIGGER_EXTI8: trigger input is EXTI8
      \arg        DMAMUX_TRIGGER_EXTI9: trigger input is EXTI9
      \arg        DMAMUX_TRIGGER_EXTI10: trigger input is EXTI10
      \arg        DMAMUX_TRIGGER_EXTI11: trigger input is EXTI11
      \arg        DMAMUX_TRIGGER_EXTI12: trigger input is EXTI12
      \arg        DMAMUX_TRIGGER_EXTI13: trigger input is EXTI13
      \arg        DMAMUX_TRIGGER_EXTI14: trigger input is EXTI14
      \arg        DMAMUX_TRIGGER_EXTI15: trigger input is EXTI15
      \arg        DMAMUX_TRIGGER_EVTX_OUT0: trigger input is Evtx_out0
      \arg        DMAMUX_TRIGGER_EVTX_OUT0: trigger input is Evtx_out1
      \arg        DMAMUX_TRIGGER_EVTX_OUT0: trigger input is Evtx_out2
      \arg        DMAMUX_TRIGGER_EVTX_OUT0: trigger input is Evtx_out3
      \arg        DMAMUX_TRIGGER_TIMER20_CH0_O: trigger input is TIMER20_CH0_O
    \param[out] none
    \retval     none
*/
void dmamux_trigger_id_config(dmamux_generator_channel_enum channelx, uint32_t id)
{
    DMAMUX_RG_CHXCFG(channelx) &= ~DMAMUX_RG_CHXCFG_TID;
    DMAMUX_RG_CHXCFG(channelx) |= id;
}

/*!
    \brief      get DMAMUX flag
    \param[in]  flag: flag type
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_FLAG_MUXCH0_SO: DMAMUX request multiplexer channel 0 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH1_SO: DMAMUX request multiplexer channel 1 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH2_SO: DMAMUX request multiplexer channel 2 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH3_SO: DMAMUX request multiplexer channel 3 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH4_SO: DMAMUX request multiplexer channel 4 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH5_SO: DMAMUX request multiplexer channel 5 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH6_SO: DMAMUX request multiplexer channel 6 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH7_SO: DMAMUX request multiplexer channel 7 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH8_SO: DMAMUX request multiplexer channel 8 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH9_SO: DMAMUX request multiplexer channel 9 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH10_SO: DMAMUX request multiplexer channel 10 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH11_SO: DMAMUX request multiplexer channel 11 synchronization overrun flag
      \arg        DMAMUX_FLAG_GENCH0_TO: DMAMUX request generator channel 0 trigger overrun flag
      \arg        DMAMUX_FLAG_GENCH1_TO: DMAMUX request generator channel 1 trigger overrun flag
      \arg        DMAMUX_FLAG_GENCH2_TO: DMAMUX request generator channel 2 trigger overrun flag
      \arg        DMAMUX_FLAG_GENCH3_TO: DMAMUX request generator channel 3 trigger overrun flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dmamux_flag_get(dmamux_flag_enum flag)
{
    FlagStatus reval;

    if(0U != (DMAMUX_REG_VAL(flag) & BIT(DMAMUX_BIT_POS(flag)))) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      clear DMAMUX flag
    \param[in]  flag: flag type
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_FLAG_MUXCH0_SO: DMAMUX request multiplexer channel 0 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH1_SO: DMAMUX request multiplexer channel 1 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH2_SO: DMAMUX request multiplexer channel 2 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH3_SO: DMAMUX request multiplexer channel 3 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH4_SO: DMAMUX request multiplexer channel 4 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH5_SO: DMAMUX request multiplexer channel 5 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH6_SO: DMAMUX request multiplexer channel 6 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH7_SO: DMAMUX request multiplexer channel 7 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH8_SO: DMAMUX request multiplexer channel 8 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH9_SO: DMAMUX request multiplexer channel 9 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH10_SO: DMAMUX request multiplexer channel 10 synchronization overrun flag
      \arg        DMAMUX_FLAG_MUXCH11_SO: DMAMUX request multiplexer channel 11 synchronization overrun flag
      \arg        DMAMUX_FLAG_GENCH0_TO: DMAMUX request generator channel 0 trigger overrun flag
      \arg        DMAMUX_FLAG_GENCH1_TO: DMAMUX request generator channel 1 trigger overrun flag
      \arg        DMAMUX_FLAG_GENCH2_TO: DMAMUX request generator channel 2 trigger overrun flag
      \arg        DMAMUX_FLAG_GENCH3_TO: DMAMUX request generator channel 3 trigger overrun flag
    \param[out] none
    \retval     none
*/
void dmamux_flag_clear(dmamux_flag_enum flag)
{
    DMAMUX_REG_VAL3(flag) = BIT(DMAMUX_BIT_POS(flag));
}

/*!
    \brief      enable DMAMUX interrupt
    \param[in]  interrupt: specify which interrupt to enable
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_INT_MUXCH0_SO: DMAMUX request multiplexer channel 0 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH1_SO: DMAMUX request multiplexer channel 1 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH2_SO: DMAMUX request multiplexer channel 2 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH3_SO: DMAMUX request multiplexer channel 3 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH4_SO: DMAMUX request multiplexer channel 4 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH5_SO: DMAMUX request multiplexer channel 5 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH6_SO: DMAMUX request multiplexer channel 6 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH7_SO: DMAMUX request multiplexer channel 7 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH8_SO: DMAMUX request multiplexer channel 8 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH9_SO: DMAMUX request multiplexer channel 9 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH10_SO: DMAMUX request multiplexer channel 10 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH11_SO: DMAMUX request multiplexer channel 11 synchronization overrun interrupt
      \arg        DMAMUX_INT_GENCH0_TO: DMAMUX request generator channel 0 trigger overrun interrupt
      \arg        DMAMUX_INT_GENCH1_TO: DMAMUX request generator channel 1 trigger overrun interrupt
      \arg        DMAMUX_INT_GENCH2_TO: DMAMUX request generator channel 2 trigger overrun interrupt
      \arg        DMAMUX_INT_GENCH3_TO: DMAMUX request generator channel 3 trigger overrun interrupt
    \param[out] none
    \retval     none
*/
void dmamux_interrupt_enable(dmamux_interrupt_enum interrupt)
{
    DMAMUX_REG_VAL(interrupt) |= BIT(DMAMUX_BIT_POS(interrupt));
}

/*!
    \brief      disable DMAMUX interrupt
    \param[in]  interrupt: specify which interrupt to disable
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_INT_MUXCH0_SO: DMAMUX request multiplexer channel 0 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH1_SO: DMAMUX request multiplexer channel 1 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH2_SO: DMAMUX request multiplexer channel 2 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH3_SO: DMAMUX request multiplexer channel 3 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH4_SO: DMAMUX request multiplexer channel 4 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH5_SO: DMAMUX request multiplexer channel 5 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH6_SO: DMAMUX request multiplexer channel 6 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH7_SO: DMAMUX request multiplexer channel 7 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH8_SO: DMAMUX request multiplexer channel 8 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH9_SO: DMAMUX request multiplexer channel 9 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH10_SO: DMAMUX request multiplexer channel 10 synchronization overrun interrupt
      \arg        DMAMUX_INT_MUXCH11_SO: DMAMUX request multiplexer channel 11 synchronization overrun interrupt
      \arg        DMAMUX_INT_GENCH0_TO: DMAMUX request generator channel 0 trigger overrun interrupt
      \arg        DMAMUX_INT_GENCH1_TO: DMAMUX request generator channel 1 trigger overrun interrupt
      \arg        DMAMUX_INT_GENCH2_TO: DMAMUX request generator channel 2 trigger overrun interrupt
      \arg        DMAMUX_INT_GENCH3_TO: DMAMUX request generator channel 3 trigger overrun interrupt
    \param[out] none
    \retval     none
*/
void dmamux_interrupt_disable(dmamux_interrupt_enum interrupt)
{
    DMAMUX_REG_VAL(interrupt) &= ~BIT(DMAMUX_BIT_POS(interrupt));
}

/*!
    \brief      get DMAMUX interrupt flag
    \param[in]  int_flag: flag type
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_INT_FLAG_MUXCH0_SO: DMAMUX request multiplexer channel 0 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH1_SO: DMAMUX request multiplexer channel 1 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH2_SO: DMAMUX request multiplexer channel 2 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH3_SO: DMAMUX request multiplexer channel 3 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH4_SO: DMAMUX request multiplexer channel 4 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH5_SO: DMAMUX request multiplexer channel 5 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH6_SO: DMAMUX request multiplexer channel 6 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH7_SO: DMAMUX request multiplexer channel 7 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH8_SO: DMAMUX request multiplexer channel 8 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH9_SO: DMAMUX request multiplexer channel 9 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH10_SO: DMAMUX request multiplexer channel 10 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH11_SO: DMAMUX request multiplexer channel 11 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_GENCH0_TO: DMAMUX request generator channel 0 trigger overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_GENCH1_TO: DMAMUX request generator channel 1 trigger overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_GENCH2_TO: DMAMUX request generator channel 2 trigger overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_GENCH3_TO: DMAMUX request generator channel 3 trigger overrun interrupt flag
    \param[out] none
    \retval     FlagStatus: SET or RESET
*/
FlagStatus dmamux_interrupt_flag_get(dmamux_interrupt_flag_enum int_flag)
{
    FlagStatus reval;
    uint32_t intenable = 0U, flagstatus = 0U;

    /* get the interrupt enable bit status */
    intenable = (DMAMUX_REG_VAL2(int_flag) & BIT(DMAMUX_BIT_POS2(int_flag)));
    /* get the corresponding flag bit status */
    flagstatus = (DMAMUX_REG_VAL(int_flag) & BIT(DMAMUX_BIT_POS(int_flag)));

    if(flagstatus && intenable) {
        reval = SET;
    } else {
        reval = RESET;
    }

    return reval;
}

/*!
    \brief      clear DMAMUX interrupt flag
    \param[in]  int_flag: flag type
                only one parameter can be selected which is shown as below:
      \arg        DMAMUX_INT_FLAG_MUXCH0_SO: DMAMUX request multiplexer channel 0 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH1_SO: DMAMUX request multiplexer channel 1 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH2_SO: DMAMUX request multiplexer channel 2 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH3_SO: DMAMUX request multiplexer channel 3 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH4_SO: DMAMUX request multiplexer channel 4 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH5_SO: DMAMUX request multiplexer channel 5 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH6_SO: DMAMUX request multiplexer channel 6 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH7_SO: DMAMUX request multiplexer channel 7 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH8_SO: DMAMUX request multiplexer channel 8 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH9_SO: DMAMUX request multiplexer channel 9 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH10_SO: DMAMUX request multiplexer channel 10 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_MUXCH11_SO: DMAMUX request multiplexer channel 11 synchronization overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_GENCH0_TO: DMAMUX request generator channel 0 trigger overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_GENCH1_TO: DMAMUX request generator channel 1 trigger overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_GENCH2_TO: DMAMUX request generator channel 2 trigger overrun interrupt flag
      \arg        DMAMUX_INT_FLAG_GENCH3_TO: DMAMUX request generator channel 3 trigger overrun interrupt flag
    \param[out] none
    \retval     none
*/
void dmamux_interrupt_flag_clear(dmamux_interrupt_flag_enum int_flag)
{
    DMAMUX_REG_VAL3(int_flag) = BIT(DMAMUX_BIT_POS(int_flag));
}
