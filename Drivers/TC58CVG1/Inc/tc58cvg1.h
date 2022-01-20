/**
  ******************************************************************************
  * @file    TC58CVG1.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    29-May-2015
  * @brief   This file contains all the description of the N25Q128A QSPI memory.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TC58CVG1_H
#define __TC58CVG1_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup n25q128a
  * @{
  */

/** @defgroup N25Q128A_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup TC58CVG1_Exported_Constants
  * @{
  */
   
/** 
  * @brief  TC58CVG1 Configuration
  */  
#define TC58CVG1_FLASH_SIZE                  0x10000000 /* 2 GBits => 256MBytes */
#define TC58CVG1_SECTOR_SIZE                 0x200   /* 512 Bytes */
#define TC58CVG1_PAGE_SIZE                   0x800    /* 2048 Bytes */
#define TC58CVG1_BLOCK_SIZE                  0x20000  /* 128 kBytes */

#define TC58CVG1_DUMMY_CYCLES_READ           8

#define TC58CVG1_BLOCK_ERASE_MAX_TIME        500
#define TC58CVG1_READ_PAGE_MAX_TIME          500
#define TC58CVG1_PROG_MAX_TIME               500

/** 
  * @brief  N25Q128A Commands  
  */  
/* Reset Operations */
#define TC58CVG1_RESET_CMD                   0xFF


#define TC58CVG1_GET_FEATURES_CMD            0x0F
#define TC58CVG1_SET_FEATURES_CMD            0x1F
#define TC58CVG1_READ_PAGE_CMD               0x13
#define TC58CVG1_PROGRAM_EXECUTE_CMD         0x10
#define TC58CVG1_ERASE_BLOCK_CMD             0xD8

#define TC58CVG1_PROGRAM_LOADx4_CMD          0x32
#define TC58CVG1_PROGRAM_LOAD_RANDOMx4_CMD   0xC4
#define TC58CVG1_READ_BUFFERx4_CMD           0x6B

/* Write Operations */
#define TC58CVG1_WRITE_ENABLE_CMD                     0x06
#define TC58CVG1_WRITE_DISABLE_CMD                    0x04






#define TC58CVG1_FT_A0_ADR                   0xA0


#define TC58CVG1_FT_B0_ADR                   0xB0
#define TC58CVG1_FT_B0_HOLD_D_BIT            (1 << 0)


#define TC58CVG1_FT_C0_ADR                   0xC0
#define TC58CVG1_FT_C0_OIP_BIT               (1 << 0)
#define TC58CVG1_FT_C0_WEL_BIT               (1 << 1)
#define TC58CVG1_FT_C0_ERS_F_BIT             (1 << 2)
#define TC58CVG1_FT_C0_PRG_F_BIT             (1 << 3)
#define TC58CVG1_FT_C0_ECCS0_BIT             (1 << 4)
#define TC58CVG1_FT_C0_ECCS1_BIT             (1 << 5)




/**
  * @}
  */
  
/** @defgroup N25Q128A_Exported_Functions
  * @{
  */ 
/**
  * @}
  */ 
      
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __N25Q128A_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
