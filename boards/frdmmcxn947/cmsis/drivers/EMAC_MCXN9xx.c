/* --------------------------------------------------------------------------
 * Copyright (c) 2025 Arm Limited (or its affiliates).
 * All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * $Date:        5. December 2024
 * $Revision:    V1.0
 *
 * Driver:       Driver_ETH_MAC0
 * Configured:   pin/clock configuration via MCUXpresso Config Tools
 * Project:      CMSIS Ethernet Media Access (MAC) Driver for NXP MCXN9xx
 * --------------------------------------------------------------------------
 * Use the following configuration settings in the middleware component
 * to connect to this driver.
 *
 *   Configuration Setting                     Value
 *   ---------------------                     -----
 *   Connect to hardware via Driver_ETH_MAC# = 0
 * -------------------------------------------------------------------------- */

/* History:
 *  Version 1.0
 *    Initial release
 */

#include "EMAC_MCXN9xx.h"
#include "fsl_enet.h"

/* Receive/transmit checksum offload enabled by default */
#ifndef EMAC_CHECKSUM_OFFLOAD
  #define EMAC_CHECKSUM_OFFLOAD 1
#endif

#define ARM_ETH_MAC_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(1, 0) /* driver version */

/* EMAC Memory Buffer configuration */
#define EMAC_BUF_SIZE           1536U   /* ETH Receive/Transmit buffer size   */
#define EMAC_RX_BUF_CNT         4U      /* 0x1800 for Rx (4*1536=6K)          */
#define EMAC_TX_BUF_CNT         4U      /* 0x1800 for Tx (4*1536=6K)          */

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = {
    ARM_ETH_MAC_API_VERSION,
    ARM_ETH_MAC_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_ETH_MAC_CAPABILITIES DriverCapabilities = {
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_rx_ip4  */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_rx_ip6  */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_rx_udp  */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_rx_tcp  */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_rx_icmp */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_tx_ip4  */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_tx_ip6  */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_tx_udp  */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_tx_tcp  */
  (EMAC_CHECKSUM_OFFLOAD) ? 1U : 0U,    /* checksum_offload_tx_icmp */
  (ENET_RMII) ? ARM_ETH_INTERFACE_RMII :
                ARM_ETH_INTERFACE_MII,  /* media_interface          */
  0U,                                   /* mac_address              */
  1U,                                   /* event_rx_frame           */
  1U,                                   /* event_tx_frame           */
  1U,                                   /* event_wakeup             */
  0U,                                   /* precision_timer          */
  0U                                    /* reserved                 */
};

/* Frame buffers */
static uint32_t Rx_Buf[EMAC_RX_BUF_CNT][EMAC_BUF_SIZE>>2];
static uint32_t Tx_Buf[EMAC_TX_BUF_CNT][EMAC_BUF_SIZE>>2];

/* Frame buffer descriptors */
static enet_rx_bd_struct_t Rx_Desc[EMAC_RX_BUF_CNT];
static enet_tx_bd_struct_t Tx_Desc[EMAC_TX_BUF_CNT];

/* EMAC control structure */
static EMAC_INFO emac = { 0 };

/**
  \fn          ARM_DRIVER_VERSION GetVersion (void)
  \brief       Get driver version.
  \return      \ref ARM_DRIVER_VERSION
*/
static ARM_DRIVER_VERSION GetVersion (void) {
  return DriverVersion;
}

/**
  \fn          ARM_ETH_MAC_CAPABILITIES GetCapabilities (void)
  \brief       Get driver capabilities.
  \return      \ref ARM_ETH_MAC_CAPABILITIES
*/
static ARM_ETH_MAC_CAPABILITIES GetCapabilities (void) {
  return DriverCapabilities;
}

/**
  \fn          int32_t Initialize (ARM_ETH_MAC_SignalEvent_t cb_event)
  \brief       Initialize Ethernet MAC Device.
  \param[in]   cb_event  Pointer to \ref ARM_ETH_MAC_SignalEvent
  \return      \ref execution_status
*/
static int32_t Initialize (ARM_ETH_MAC_SignalEvent_t cb_event) {

  if (emac.flags & EMAC_FLAG_INIT) {
    return ARM_DRIVER_OK;
  }

  /* Clear control structure */
  memset (&emac, 0, sizeof (EMAC_INFO));

   /* Register driver callback function */
  emac.cb_event = cb_event;

  emac.flags    = EMAC_FLAG_INIT;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t Uninitialize (void)
  \brief       De-initialize Ethernet MAC Device.
  \return      \ref execution_status
*/
static int32_t Uninitialize (void) {

  emac.flags = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t PowerControl (ARM_POWER_STATE state)
  \brief       Control Ethernet MAC Device Power.
  \param[in]   state  Power state
  \return      \ref execution_status
*/
static int32_t PowerControl (ARM_POWER_STATE state) {
  uint32_t rx_buffer[EMAC_RX_BUF_CNT];
  enet_buffer_config_t buf_cfg;
  enet_config_t cfg;
  int32_t i, retv;

  switch (state) {
    case ARM_POWER_OFF:
      if (!(emac.flags & EMAC_FLAG_POWER)) {
        break;
      }

      /* Disable ENET module */
      ENET_Deinit (ENET);

      /* Disable ENET interrupts in NVIC */
      NVIC_DisableIRQ (ETHERNET_IRQn);

      emac.flags = EMAC_FLAG_INIT;
      break;

    case ARM_POWER_LOW:
      return ARM_DRIVER_ERROR_UNSUPPORTED;

    case ARM_POWER_FULL:
      if (!(emac.flags & EMAC_FLAG_INIT)) {
        return ARM_DRIVER_ERROR;
      }
      if (emac.flags & EMAC_FLAG_POWER) {
        break;
      }

      /* Use external reference clock */
      CLOCK_AttachClk (kNONE_to_ENETRMII);
      CLOCK_EnableClock (kCLOCK_Enet);

      /* Setup ENET configuration */
      ENET_GetDefaultConfig (&cfg);
#if (EMAC_CHECKSUM_OFFLOAD)
      cfg.specialControl = kENET_RxChecksumOffloadEnable | kENET_StoreAndForward;
#endif
      cfg.miiMode        = (ENET_RMII) ? kENET_RmiiMode : kENET_MiiMode;
      cfg.interrupt      = kENET_DmaTx | kENET_DmaRx;

      /* Initialize ENET */
      ENET_Init (ENET, &cfg, NULL, 50000000U);

      /* Setup ENET-DMA descriptors */
      buf_cfg.rxRingLen            = EMAC_RX_BUF_CNT;
      buf_cfg.txRingLen            = EMAC_TX_BUF_CNT;
      buf_cfg.txDescStartAddrAlign = &Tx_Desc[0];
      buf_cfg.txDescTailAddrAlign  = &Tx_Desc[0];
      buf_cfg.txDirtyStartAddr     = NULL;
      buf_cfg.rxDescStartAddrAlign = &Rx_Desc[0];
      buf_cfg.rxDescTailAddrAlign  = &Rx_Desc[EMAC_RX_BUF_CNT];
      buf_cfg.rxBufferStartAddr    = &rx_buffer[0];
      buf_cfg.rxBuffSizeAlign      = EMAC_BUF_SIZE;

      /* Get Rx buffer addresses */
      for (i = 0; i < EMAC_RX_BUF_CNT; i++) {
        rx_buffer[i] = (uint32_t)&Rx_Buf[i];
      }

      /* Initialize ENET-DMA descriptors */
      retv = ENET_DescriptorInit (ENET, &cfg, &buf_cfg);

      /* Initialize MDIO management interface */
      ENET_SetSMI (ENET, CLOCK_GetCoreSysClkFreq());

      /* Enable ENET peripheral interrupts in NVIC */
      NVIC_EnableIRQ (ETHERNET_IRQn);

      emac.flags |= EMAC_FLAG_POWER;

      if (retv != kStatus_Success) {
        return ARM_DRIVER_ERROR;
      }
      break;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
    }

    return ARM_DRIVER_OK;
}

/**
  \fn          int32_t GetMacAddress (ARM_ETH_MAC_ADDR *ptr_addr)
  \brief       Get Ethernet MAC Address.
  \param[in]   ptr_addr  Pointer to address
  \return      \ref execution_status
*/
static int32_t GetMacAddress (ARM_ETH_MAC_ADDR *ptr_addr) {
  uint32_t val;

  if (ptr_addr == NULL) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  // ENET_GetMacAddr (ENET, (uint8_t *)ptr_addr);

  val = ENET->MAC_ADDRESS0_HIGH;
  ptr_addr->b[5] = (uint8_t)(val >> 8);
  ptr_addr->b[4] = (uint8_t)(val);
  val = ENET->MAC_ADDRESS0_LOW;
  ptr_addr->b[3] = (uint8_t)(val >> 24);
  ptr_addr->b[2] = (uint8_t)(val >> 16);
  ptr_addr->b[1] = (uint8_t)(val >>  8);
  ptr_addr->b[0] = (uint8_t)(val);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SetMacAddress (const ARM_ETH_MAC_ADDR *ptr_addr)
  \brief       Set Ethernet MAC Address.
  \param[in]   ptr_addr  Pointer to address
  \return      \ref execution_status
*/
static int32_t SetMacAddress (const ARM_ETH_MAC_ADDR *ptr_addr) {

  if (ptr_addr == NULL) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (!(emac.flags & EMAC_FLAG_POWER)) {
    return ARM_DRIVER_ERROR;
  }

  ENET_SetMacAddr (ENET, (uint8_t *)ptr_addr);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t SetAddressFilter (const ARM_ETH_MAC_ADDR *ptr_addr,
                                               uint32_t          num_addr)
  \brief       Configure Address Filter.
  \param[in]   ptr_addr  Pointer to addresses
  \param[in]   num_addr  Number of addresses to configure
  \return      \ref execution_status
*/
static int32_t SetAddressFilter (const ARM_ETH_MAC_ADDR *ptr_addr, uint32_t num_addr) {
  (void)ptr_addr;
  (void)num_addr;

  /* Not supported by ENET module */
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          int32_t SendFrame (const uint8_t *frame, uint32_t len, uint32_t flags)
  \brief       Send Ethernet frame.
  \param[in]   frame  Pointer to frame buffer with data to send
  \param[in]   len    Frame buffer length in bytes
  \param[in]   flags  Frame transmit flags (see ARM_ETH_MAC_TX_FRAME_...)
  \return      \ref execution_status
*/
static int32_t SendFrame (const uint8_t *frame, uint32_t len, uint32_t flags) {

  if ((frame == NULL) || (len == 0U)) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (!(emac.flags & EMAC_FLAG_POWER)) {
    return ARM_DRIVER_ERROR;
  }

  if (emac.tx_len == 0U) {
    /* Start of a new transmit frame */
    if (ENET_IsTxDescriptorDmaOwn (&Tx_Desc[emac.tx_index])) {
      /* Transmitter is busy, wait */
      return ARM_DRIVER_ERROR_BUSY;
    }
  }
  
  /* Copy data fragments to EMAC-DMA buffer */
  memcpy ((uint8_t *)&Tx_Buf[emac.tx_index] + emac.tx_len, frame, len);
  emac.tx_len += len;

  if (flags & ARM_ETH_MAC_TX_FRAME_FRAGMENT) {
    /* More data to come */
    return ARM_DRIVER_OK;
  }

  /* Frame is now ready, pass it on to DMA */
  ENET_SetupTxDescriptor (&Tx_Desc[emac.tx_index], Tx_Buf[emac.tx_index], emac.tx_len, NULL, 0,
                          emac.tx_len, true, false, kENET_FirstLastFlag, 0);
#if (EMAC_CHECKSUM_OFFLOAD)
  Tx_Desc[emac.tx_index].tdes3 |= ENET_TXDESCRIP_RD_CIC(kENET_TxOffloadAll);
#endif
  /* Update the transmit tail address */
  ENET_UpdateTxDescriptorTail (ENET, 0, (uint32_t)&Tx_Desc[++emac.tx_index]);

  if (emac.tx_index >= EMAC_TX_BUF_CNT) {
    emac.tx_index = 0U;
  }
  emac.tx_len = 0U;

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t ReadFrame (uint8_t *frame, uint32_t len)
  \brief       Read data of received Ethernet frame.
  \param[in]   frame  Pointer to frame buffer for data to read into
  \param[in]   len    Frame buffer length in bytes
  \return      number of data bytes read or execution status
                 - value >= 0: number of data bytes read
                 - value < 0: error occurred, value is execution status as defined with \ref execution_status 
*/
static int32_t ReadFrame (uint8_t *frame, uint32_t len) {

  if ((frame == NULL) && (len != 0U)) {
    /* Invalid parameters */
    return ARM_DRIVER_ERROR_PARAMETER;
  }

  if (!(emac.flags & EMAC_FLAG_POWER)) {
    return ARM_DRIVER_ERROR;
  }

  memcpy (frame, Rx_Buf[emac.rx_index], len);

  /* Return this block back to ENET-DMA */
  ENET_UpdateRxDescriptor (&Rx_Desc[emac.rx_index], Rx_Buf[emac.rx_index], NULL, true, false);  

  /* Resume DMA if receive suspended */
  if (ENET_GetDmaInterruptStatus (ENET, 0) & kENET_DmaRxBuffUnavail) {
    ENET_UpdateRxDescriptorTail (ENET, 0, (uint32_t)&Rx_Desc[EMAC_RX_BUF_CNT]);
  }

  if (++emac.rx_index >= EMAC_RX_BUF_CNT) {
    emac.rx_index = 0U;
  }

  return (int32_t)len;
}

/**
  \fn          uint32_t GetRxFrameSize (void)
  \brief       Get size of received Ethernet frame.
  \return      number of bytes in received frame
*/
static uint32_t GetRxFrameSize (void) {
  uint32_t stat;

  if (!(emac.flags & EMAC_FLAG_POWER)) {
    return ARM_DRIVER_ERROR;
  }

  stat = ENET_GetRxDescriptor (&Rx_Desc[emac.rx_index]);
  if (stat & ENET_RXDESCRIP_RD_OWN_MASK) {
    /* Owned by DMA */
    return 0U;
  }
 
  if (!(stat & ENET_RXDESCRIP_WR_FD_MASK)    ||
      !(stat & ENET_RXDESCRIP_WR_LD_MASK)    ||
       (stat & ENET_RXDESCRIP_WR_ERRSUM_MASK)) {
    /* Error, this block is invalid */
    return 0xFFFFFFFFU;
  }

  return (stat & ENET_RXDESCRIP_WR_PACKETLEN_MASK) - ENET_FCS_LEN;
}

/**
  \fn          int32_t GetRxFrameTime (ARM_ETH_MAC_TIME *time)
  \brief       Get time of received Ethernet frame.
  \param[in]   time  Pointer to time structure for data to read into
  \return      \ref execution_status
*/
static int32_t GetRxFrameTime (ARM_ETH_MAC_TIME *time) {
  (void)time;

  /* Not implemented */
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          int32_t GetTxFrameTime (ARM_ETH_MAC_TIME *time)
  \brief       Get time of transmitted Ethernet frame.
  \param[in]   time  Pointer to time structure for data to read into
  \return      \ref execution_status
*/
static int32_t GetTxFrameTime (ARM_ETH_MAC_TIME *time) {
  (void)time;

  /* Not implemented */
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          int32_t Control (uint32_t control, uint32_t arg)
  \brief       Control Ethernet Interface.
  \param[in]   control  operation
  \param[in]   arg      argument of operation (optional)
  \return      \ref execution_status
*/
static int32_t Control (uint32_t control, uint32_t arg) {
  enet_vlan_ctrl_t vlan_ctrl = { 0 };
  uint32_t mac_filter, mac_config;

  if (!(emac.flags & EMAC_FLAG_POWER)) {
    return ARM_DRIVER_ERROR;
  }

  switch (control) {
    case ARM_ETH_MAC_CONFIGURE:
      mac_config = ENET->MAC_CONFIGURATION;
      /* Configure 100MBit/10MBit mode */
      switch (arg & ARM_ETH_MAC_SPEED_Msk) {
        case ARM_ETH_MAC_SPEED_10M:
          mac_config &= ~ENET_MAC_CONFIGURATION_FES_MASK;
          break;
        case ARM_ETH_SPEED_100M:
          mac_config |=  ENET_MAC_CONFIGURATION_FES_MASK;
          break;
        default:
          return ARM_DRIVER_ERROR_UNSUPPORTED;
      }

      /* Configure Half/Full duplex mode */
      switch (arg & ARM_ETH_MAC_DUPLEX_Msk) {
        case ARM_ETH_MAC_DUPLEX_HALF:
          mac_config &= ~ENET_MAC_CONFIGURATION_DM_MASK;
          break;
        case ARM_ETH_MAC_DUPLEX_FULL:
          mac_config |=  ENET_MAC_CONFIGURATION_DM_MASK;
          break;
      }

      /* Configure loopback mode */
      if (arg & ARM_ETH_MAC_LOOPBACK) {
        mac_config |=  ENET_MAC_CONFIGURATION_LM_MASK;
      }
      else {
        mac_config &= ~ENET_MAC_CONFIGURATION_LM_MASK;
      }
      ENET->MAC_CONFIGURATION = mac_config;

#if (EMAC_CHECKSUM_OFFLOAD)
      /* Enable rx checksum verification */
      if (arg & ARM_ETH_MAC_CHECKSUM_OFFLOAD_RX) {
        ENET->MTL_QUEUE[0].MTL_RXQX_OP_MODE |=  ENET_MTL_QUEUE_MTL_RXQX_OP_MODE_RSF_MASK; 
        ENET->MAC_CONFIGURATION             |=  ENET_MAC_CONFIGURATION_IPC_MASK;
      }
      else {
        ENET->MTL_QUEUE[0].MTL_RXQX_OP_MODE &= ~ENET_MTL_QUEUE_MTL_RXQX_OP_MODE_RSF_MASK; 
        ENET->MAC_CONFIGURATION             &= ~ENET_MAC_CONFIGURATION_IPC_MASK;
      }

      /* Enable tx checksum generation */
      if (arg & ARM_ETH_MAC_CHECKSUM_OFFLOAD_TX) {
        ENET->MTL_QUEUE[0].MTL_TXQX_OP_MODE |=  ENET_MTL_QUEUE_MTL_TXQX_OP_MODE_TSF_MASK;
      }
      else {
        ENET->MTL_QUEUE[0].MTL_TXQX_OP_MODE &= ~ENET_MTL_QUEUE_MTL_TXQX_OP_MODE_TSF_MASK;
      }
#else
      if ((arg & ARM_ETH_MAC_CHECKSUM_OFFLOAD_RX) ||
          (arg & ARM_ETH_MAC_CHECKSUM_OFFLOAD_TX)) {
        /* Checksum offload is disabled in the driver */
        return ARM_DRIVER_ERROR;
      }
#endif

      /* Keep VLAN tag filter setting */
      mac_filter = ENET->MAC_PACKET_FILTER & ENET_MAC_PACKET_FILTER_VTFE_MASK;

      /* Enable broadcast frame receive */
      if (!(arg & ARM_ETH_MAC_ADDRESS_BROADCAST)) {
        mac_filter |= ENET_MAC_PACKET_FILTER_DBF_MASK;
      }

      /* Enable all multicast frame receive */
      if (arg & ARM_ETH_MAC_ADDRESS_MULTICAST) {
        mac_filter |= ENET_MAC_PACKET_FILTER_PM_MASK;
      }

      /* Enable promiscuous mode (no filtering) */
      if (arg & ARM_ETH_MAC_ADDRESS_ALL) {
        mac_filter |= ENET_MAC_PACKET_FILTER_PR_MASK;
      }

      ENET->MAC_PACKET_FILTER = mac_filter;
      break;

    case ARM_ETH_MAC_CONTROL_TX:
      /* Enable/disable MAC transmitter */
      if (arg != 0U) {
        ENET->DMA_CH[0].DMA_CHX_TX_CTRL |= ENET_DMA_CH_DMA_CHX_TX_CTRL_ST_MASK;
        ENET->MAC_CONFIGURATION         |= ENET_MAC_CONFIGURATION_TE_MASK;
      }
      else {
        ENET->MAC_CONFIGURATION         &= ~ENET_MAC_CONFIGURATION_TE_MASK;
        ENET->DMA_CH[0].DMA_CHX_TX_CTRL &= ~ENET_DMA_CH_DMA_CHX_TX_CTRL_ST_MASK;
      }
      break;

    case ARM_ETH_MAC_CONTROL_RX:
      /* Enable/disable MAC receiver */
      if (arg != 0U) {
        ENET->DMA_CH[0].DMA_CHX_RX_CTRL |= ENET_DMA_CH_DMA_CHX_RX_CTRL_SR_MASK;
        ENET->MAC_CONFIGURATION         |= ENET_MAC_CONFIGURATION_RE_MASK;
      }
      else {
        ENET->MAC_CONFIGURATION         &= ~ENET_MAC_CONFIGURATION_RE_MASK;
        ENET->DMA_CH[0].DMA_CHX_RX_CTRL &= ~ENET_DMA_CH_DMA_CHX_RX_CTRL_SR_MASK;
      }
      break;

    case ARM_ETH_MAC_FLUSH:
      /* Flush Tx and Rx buffers */
      if (arg & ARM_ETH_MAC_FLUSH_RX) {
        uint32_t dma_rx_ctrl = ENET->DMA_CH[0].DMA_CHX_RX_CTRL;
        ENET->DMA_CH[0].DMA_CHX_RX_CTRL &= ~ENET_DMA_CH_DMA_CHX_RX_CTRL_SR_MASK;
        ENET->DMA_CH[0].DMA_CHX_RX_CTRL |= ENET_DMA_CH_DMA_CHX_RX_CTRL_RPF_MASK;
        ENET->DMA_CH[0].DMA_CHX_RX_CTRL = dma_rx_ctrl;
      }
      if (arg & ARM_ETH_MAC_FLUSH_TX) {

      }
      break;

    case ARM_ETH_MAC_SLEEP:
      /* Enable/disable Sleep mode */
     if (arg != 0U) {
        /* Enable Power Management interrupts */
        ENET_EnableInterrupts(ENET, kENET_MacPmt);
        NVIC_EnableIRQ (ETHERNET_PMT_IRQn);
        /* Enter Power-down, Magic packet enable */
        ENET_EnterPowerDown (ENET, NULL);
      }
      else {
        /* Disable Power Management interrupts */
        ENET_DisableInterrupts(ENET, kENET_MacPmt);
        NVIC_DisableIRQ (ETHERNET_PMT_IRQn);
        ENET->MAC_PMT_CONTROL_STATUS = 0x00000000U;
      }
      break;

    case ARM_ETH_MAC_VLAN_FILTER:
      /* Configure VLAN filter */
      vlan_ctrl.rxVlanTag.vid = arg & 0xFFFF;
      if (arg & ARM_ETH_MAC_VLAN_FILTER_ID_ONLY) {
        vlan_ctrl.vidComparison = true;
      }
      if (arg != 0U) {
        vlan_ctrl.disableVlanTypeCheck = true;
        vlan_ctrl.outerTagInRxStatus   = true;
      }  
      ENET_SetVlanCtrl(ENET, &vlan_ctrl);
      if (arg != 0U) {
        ENET->MAC_PACKET_FILTER |=  ENET_MAC_PACKET_FILTER_VTFE_MASK;
      } 
      else {
        ENET->MAC_PACKET_FILTER &= ~ENET_MAC_PACKET_FILTER_VTFE_MASK;
      } 
      break;

    default:
      return ARM_DRIVER_ERROR_UNSUPPORTED;
  }

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t ControlTimer (uint32_t control, ARM_ETH_MAC_TIME *time)
  \brief       Control Precision Timer.
  \param[in]   control  operation
  \param[in]   time     Pointer to time structure
  \return      \ref execution_status
*/
static int32_t ControlTimer (uint32_t control, ARM_ETH_MAC_TIME *time) {
  (void)control;
  (void)time;

  /* Not implemented */
  return ARM_DRIVER_ERROR_UNSUPPORTED;
}

/**
  \fn          int32_t PHY_Read (uint8_t phy_addr, uint8_t reg_addr, uint16_t *data)
  \brief       Read Ethernet PHY Register through Management Interface.
  \param[in]   phy_addr  5-bit device address
  \param[in]   reg_addr  5-bit register address
  \param[out]  data      Pointer where the result is written to
  \return      \ref execution_status
*/
static int32_t PHY_Read (uint8_t phy_addr, uint8_t reg_addr, uint16_t *data) {
  uint32_t loop;

  if (data == NULL) {
    return ARM_DRIVER_ERROR_PARAMETER;
  }
  if (!(emac.flags & EMAC_FLAG_POWER)) {
    return ARM_DRIVER_ERROR;
  }

  ENET_StartSMIRead (ENET, phy_addr, reg_addr);

  /* Wait until operation completed */
  loop = SystemCoreClock >> 12;
  while (ENET_IsSMIBusy (ENET)) {
    loop--;
    if (loop == 0) {
      /* Loop counter timeout */
      return ARM_DRIVER_ERROR_TIMEOUT;
    }
  }
  *data = (uint16_t)ENET_ReadSMIData (ENET);

  return ARM_DRIVER_OK;
}

/**
  \fn          int32_t PHY_Write (uint8_t phy_addr, uint8_t reg_addr, uint16_t data)
  \brief       Write Ethernet PHY Register through Management Interface.
  \param[in]   phy_addr  5-bit device address
  \param[in]   reg_addr  5-bit register address
  \param[in]   data      16-bit data to write
  \return      \ref execution_status
*/
static int32_t PHY_Write (uint8_t phy_addr, uint8_t reg_addr, uint16_t data) {
  uint32_t loop;

  if (!(emac.flags & EMAC_FLAG_POWER)) {
    return ARM_DRIVER_ERROR;
  }

  ENET_StartSMIWrite (ENET, phy_addr, reg_addr, data);

  /* Wait until operation completed */
  loop = SystemCoreClock >> 12;
  while (ENET_IsSMIBusy (ENET)) {
    loop--;
    if (loop == 0) {
      /* Loop counter timeout */
      return ARM_DRIVER_ERROR_TIMEOUT;
    }
  }
  return ARM_DRIVER_OK;
}

/**
  \fn          void ETHERNET_IRQHandler (void)
  \brief       Ethernet Interrupt handler.
*/
void ETHERNET_IRQHandler (void) {
  uint32_t stat, event = 0U;

  stat = ENET_GetDmaInterruptStatus(ENET, 0) & ~kENET_DmaRxBuffUnavail;
  if (stat & kENET_DmaTx) {
    /* Transmit interrupt */
    event |= ARM_ETH_MAC_EVENT_TX_FRAME;
  }
  if (stat & kENET_DmaRx) {
    /* Receive interrupt */
    event |= ARM_ETH_MAC_EVENT_RX_FRAME;
  }
  /* Callback event notification */
  if (event && emac.cb_event) {
    emac.cb_event (event);
  }
  /* Clear the interrupt */
  ENET_ClearDmaInterruptStatus(ENET, 0, stat);
  SDK_ISR_EXIT_BARRIER;
}

/**
  \fn          void ETHERNET_PMT_IRQHandler (void)
  \brief       Ethernet power management Interrupt handler.
*/
void ETHERNET_PMT_IRQHandler (void) {
  uint32_t stat, event = 0U;

  stat = ENET->MAC_PMT_CONTROL_STATUS;
  if (stat & ENET_MAC_PMT_CONTROL_STATUS_MGKPRCVD_MASK) {
      /* Magic packet received */
      event |= ARM_ETH_MAC_EVENT_WAKEUP;
  }
  /* Callback event notification */
  if (event && emac.cb_event) {
    emac.cb_event (event);
  }
  /* Clear the interrupt */
  ENET_ClearMacInterruptStatus(ENET, kENET_MacPmt);
  SDK_ISR_EXIT_BARRIER;
}

/* MAC Driver Control Block */
ARM_DRIVER_ETH_MAC Driver_ETH_MAC0 = {
  GetVersion,
  GetCapabilities,
  Initialize,
  Uninitialize,
  PowerControl,
  GetMacAddress,
  SetMacAddress,
  SetAddressFilter,
  SendFrame,
  ReadFrame,
  GetRxFrameSize,
  GetRxFrameTime,
  GetTxFrameTime,
  ControlTimer,
  Control,
  PHY_Read,
  PHY_Write
};
