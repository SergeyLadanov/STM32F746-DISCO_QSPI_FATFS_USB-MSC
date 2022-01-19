/**
  ******************************************************************************
  * @file    stm32f746g_discovery_qspi.c
  * @author  Florian SAVARD
  * @version V1.0.0
  * @date    15-november-2015
  * @brief   This file includes a standard driver for the N25Q128A QSPI 
  *          memory mounted on STM32F746G-Discovery board.
  @verbatim
  ==============================================================================
                     ##### How to use this driver #####
  ==============================================================================  
  [..] 
   (#) This driver is used to drive the N25Q128A QSPI external 
       memory mounted on STM32F746G-DISCO evaluation board.
       
   (#) This driver need a specific component driver (N25Q128A) to be included with.

   (#) Initialization steps:
       (++) Initialize the QPSI external memory using the TC58CVG1_QSPI_Init() function. This
            function includes the MSP layer hardware resources initialization and the
            QSPI interface with the external memory.
  
   (#) QSPI memory operations
       (++) QSPI memory can be accessed with read/write operations once it is
            initialized.
            Read/write operation can be performed with AHB access using the functions
            TC58CVG1_QSPI_Read()/TC58CVG1_QSPI_Write().
       (++) The function TC58CVG1_QSPI_GetInfo() returns the configuration of the QSPI memory.
            (see the QSPI memory data sheet)
       (++) Perform erase block operation using the function TC58CVG1_QSPI_Erase_Block() and by
            specifying the block address. You can perform an erase operation of the whole 
            chip by calling the function TC58CVG1_QSPI_Erase_Chip().
       (++) The function TC58CVG1_QSPI_GetStatus() returns the current status of the QSPI memory.
            (see the QSPI memory data sheet)
  @endverbatim
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

/* Includes ------------------------------------------------------------------*/
#include "tc58cvg1_qspi.h"

/** @addtogroup TC58CVG1
  * @{
  */

/** @addtogroup STM32F746G_DISCOVERY
  * @{
  */

/** @defgroup STM32F746G_DISCOVERY_QSPI STM32F746G-DISCOVERY QSPI
  * @{
  */

/* Private variables ---------------------------------------------------------*/

/** @defgroup STM32F746G_DISCOVERY_QSPI_Private_Variables Private Variables
  * @{
  */

/**
  * @}
  */


/* Private functions ---------------------------------------------------------*/

/** @defgroup STM32F746G_DISCOVERY_QSPI_Private_Functions Private Functions
  * @{
  */
static uint8_t QSPI_WriteEnable        (QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout);

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/





/**
  * @brief  Return the configuration of the QSPI memory.
  * @param  pInfo: pointer on the configuration structure  
  * @retval QSPI memory status
  */
uint8_t TC58CVG1_QSPI_GetInfo(QSPI_Info* pInfo)
{
  /* Configure the structure with the memory configuration */
  pInfo->FlashSize          = TC58CVG1_FLASH_SIZE;
  pInfo->EraseSectorSize    = TC58CVG1_PAGE_SIZE;
  pInfo->EraseSectorsNumber = (TC58CVG1_FLASH_SIZE/TC58CVG1_PAGE_SIZE);
  pInfo->ProgPageSize       = TC58CVG1_PAGE_SIZE;
  pInfo->ProgPagesNumber    = (TC58CVG1_FLASH_SIZE/TC58CVG1_PAGE_SIZE);
  
  return QSPI_OK;
}


/**
  * @brief  This function reset the QSPI memory.
  * @param  hqspi: QSPI handle
  * @retval None
  */
uint8_t TC58CVG1_QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef sCommand;

  /* Initialize the reset enable command */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = TC58CVG1_RESET_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Configure automatic polling mode to wait the memory is ready */  
  if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}


/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     sCommand;
  QSPI_AutoPollingTypeDef sConfig;

  /* Enable write operations */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = TC58CVG1_WRITE_ENABLE_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }
  
  /* Configure automatic polling mode to wait for write enabling */  
  sConfig.Match           = TC58CVG1_FT_C0_WEL_BIT;
  sConfig.Mask            = TC58CVG1_FT_C0_WEL_BIT;
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 1;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  sCommand.Instruction    = TC58CVG1_GET_FEATURES_CMD;
  sCommand.DataMode       = QSPI_DATA_1_LINE;
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       = QSPI_ADDRESS_8_BITS;
  sCommand.Address           = TC58CVG1_FT_C0_ADR;

  if (HAL_QSPI_AutoPolling(hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}





/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_WriteDisable(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     sCommand;
  QSPI_AutoPollingTypeDef sConfig;

  /* Enable write operations */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = TC58CVG1_WRITE_DISABLE_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_NONE;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  sConfig.Match           = 0;
  sConfig.Mask            = TC58CVG1_FT_C0_WEL_BIT;
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 1;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  sCommand.Instruction    = TC58CVG1_GET_FEATURES_CMD;
  sCommand.DataMode       = QSPI_DATA_1_LINE;
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       = QSPI_ADDRESS_8_BITS;
  sCommand.Address           = TC58CVG1_FT_C0_ADR;

  if (HAL_QSPI_AutoPolling(hqspi, &sCommand, &sConfig, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hqspi: QSPI handle
  * @param  Timeout: Timeout for auto-polling
  * @retval None
  */
static uint8_t QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout)
{
  QSPI_CommandTypeDef     sCommand;
  QSPI_AutoPollingTypeDef sConfig;

  /* Configure automatic polling mode to wait for memory ready */  
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = TC58CVG1_GET_FEATURES_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       = QSPI_ADDRESS_8_BITS;
  sCommand.Address           = TC58CVG1_FT_C0_ADR;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_1_LINE;
  sCommand.DummyCycles       = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;


  sConfig.Match           = 0;
  sConfig.Mask            = TC58CVG1_FT_C0_OIP_BIT;
  sConfig.MatchMode       = QSPI_MATCH_MODE_AND;
  sConfig.StatusBytesSize = 1;
  sConfig.Interval        = 0x10;
  sConfig.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_QSPI_AutoPolling(hqspi, &sCommand, &sConfig, Timeout) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}


// For kioxia nand



uint8_t TC58CVG1_QSPI_GetFeature(QSPI_HandleTypeDef *hqspi, uint8_t address, uint8_t *pData)
{
	QSPI_CommandTypeDef sCommand;

	/* Initialize the read command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = TC58CVG1_GET_FEATURES_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_8_BITS;
	sCommand.Address           = address;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the command */
	if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Reception of the data */
	if (HAL_QSPI_Receive(hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

  return QSPI_OK;
}


uint8_t TC58CVG1_QSPI_SetFeature(QSPI_HandleTypeDef *hqspi, uint8_t address, uint8_t Data)
{
	QSPI_CommandTypeDef sCommand;

	/* Initialize the read command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = TC58CVG1_SET_FEATURES_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_8_BITS;
	sCommand.Address           = address;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_1_LINE;
	sCommand.DummyCycles       = 0;
	sCommand.NbData            = 1;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	/* Configure the command */
	if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Reception of the data */
	if (HAL_QSPI_Transmit(hqspi, &Data, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

  return QSPI_OK;
}




uint8_t TC58CVG1_QSPI_ReadPage(QSPI_HandleTypeDef *hqspi, uint32_t RowAddr)
{
  QSPI_CommandTypeDef sCommand;
  uint8_t Status;

  /* Initialize the read command */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = TC58CVG1_READ_PAGE_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
  sCommand.Address           = RowAddr;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.NbData            = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }


  if (QSPI_AutoPollingMemReady(hqspi, TC58CVG1_READ_PAGE_MAX_TIME) != QSPI_OK)
  {
	  return QSPI_ERROR;
  }


  if (TC58CVG1_QSPI_GetFeature(hqspi, TC58CVG1_FT_C0_ADR, &Status) != QSPI_OK)
  {
	  return QSPI_ERROR;
  }

  if (Status & TC58CVG1_FT_C0_ECCS1_BIT)
  {
	  return QSPI_ERROR;
  }




  return QSPI_OK;
}



uint8_t TC58CVG1_QSPI_CheckReadPage(QSPI_HandleTypeDef *hqspi, uint32_t RowAddr)
{
  QSPI_CommandTypeDef sCommand;

  /* Initialize the read command */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = TC58CVG1_READ_PAGE_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
  sCommand.Address           = RowAddr;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_NONE;
  sCommand.DummyCycles       = 0;
  sCommand.NbData            = 0;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }


  if (QSPI_AutoPollingMemReady(hqspi, TC58CVG1_READ_PAGE_MAX_TIME) != QSPI_OK)
  {
	  return QSPI_ERROR;
  }

  return QSPI_OK;
}





uint8_t TC58CVG1_QSPI_ReadFromBuf(QSPI_HandleTypeDef *hqspi, uint8_t* pData, uint32_t ColAddr, uint32_t Size)
{
  QSPI_CommandTypeDef sCommand;


  /* Initialize the read command */
  sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  sCommand.Instruction       = TC58CVG1_READ_BUFFERx4_CMD;
  sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
  sCommand.AddressSize       = QSPI_ADDRESS_16_BITS;
  sCommand.Address           = ColAddr;
  sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  sCommand.DataMode          = QSPI_DATA_4_LINES;
  sCommand.DummyCycles       = TC58CVG1_DUMMY_CYCLES_READ;
  sCommand.NbData            = Size;
  sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
  sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}



uint8_t TC58CVG1_QSPI_ProgramExecute(QSPI_HandleTypeDef *hqspi, uint32_t RowAddr)
{
	QSPI_CommandTypeDef     sCommand;
	uint8_t Status;

	/* Enable write operations */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = TC58CVG1_PROGRAM_EXECUTE_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.Address           = RowAddr;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;


	/* Enable write operations */
	if (QSPI_WriteEnable(hqspi) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Configure automatic polling mode to wait for end of program */
	if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if (TC58CVG1_QSPI_GetFeature(hqspi, TC58CVG1_FT_C0_ADR, &Status) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if (Status & TC58CVG1_FT_C0_PRG_F_BIT)
	{
		return QSPI_ERROR;
	}

	QSPI_WriteDisable(hqspi);

  return QSPI_OK;
}



uint8_t TC58CVG1_QSPI_EraseBlock(QSPI_HandleTypeDef *hqspi, uint32_t BlockAddress)
{
	QSPI_CommandTypeDef     sCommand;
	uint8_t Status;

	/* Enable write operations */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = TC58CVG1_ERASE_BLOCK_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_24_BITS;
	sCommand.Address           = BlockAddress;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_NONE;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;


	/* Enable write operations */
	if (QSPI_WriteEnable(hqspi) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Configure automatic polling mode to wait for end of program */
	if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if (TC58CVG1_QSPI_GetFeature(hqspi, TC58CVG1_FT_C0_ADR, &Status) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if (Status & TC58CVG1_FT_C0_ERS_F_BIT)
	{
		return QSPI_ERROR;
	}

	QSPI_WriteDisable(hqspi);

  return QSPI_OK;
}


uint8_t TC58CVG1_QSPI_ProgramLoad(QSPI_HandleTypeDef *hqspi, uint8_t* pData, uint32_t ColAddr, uint32_t Size)
{
	QSPI_CommandTypeDef sCommand;


	/* Initialize the program command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = TC58CVG1_PROGRAM_LOADx4_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_16_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	sCommand.Address = ColAddr;
	sCommand.NbData  = Size;


	/* Configure the command */
	if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Transmission of the data */
	if (HAL_QSPI_Transmit(hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}




	return QSPI_OK;
}


uint8_t TC58CVG1_QSPI_ClearBuffer(QSPI_HandleTypeDef *hqspi)
{
	uint8_t clear_byte = 0xFF;

	if (TC58CVG1_QSPI_ProgramLoad(hqspi, &clear_byte, 0, 1))
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}


uint8_t TC58CVG1_QSPI_HoldDisable(QSPI_HandleTypeDef *hqspi)
{
	uint8_t Status = 0;

	/* Disable Hold function */
	if (TC58CVG1_QSPI_GetFeature(hqspi, TC58CVG1_FT_B0_ADR, &Status) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	Status |= TC58CVG1_FT_B0_HOLD_D_BIT;


	if (TC58CVG1_QSPI_SetFeature(hqspi, TC58CVG1_FT_B0_ADR, Status) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}


uint8_t TC58CVG1_QSPI_HoldEnable(QSPI_HandleTypeDef *hqspi)
{
	uint8_t Status = 0;

	/* Disable Hold function */
	if (TC58CVG1_QSPI_GetFeature(hqspi, TC58CVG1_FT_B0_ADR, &Status) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	Status &= ~TC58CVG1_FT_B0_HOLD_D_BIT;


	if (TC58CVG1_QSPI_SetFeature(hqspi, TC58CVG1_FT_B0_ADR, Status) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}



uint8_t TC58CVG1_QSPI_ProgramLoadRandom(QSPI_HandleTypeDef *hqspi, uint8_t* pData, uint32_t ColAddr, uint32_t Size)
{
	QSPI_CommandTypeDef sCommand;


	/* Initialize the program command */
	sCommand.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	sCommand.Instruction       = TC58CVG1_PROGRAM_LOAD_RANDOMx4_CMD;
	sCommand.AddressMode       = QSPI_ADDRESS_1_LINE;
	sCommand.AddressSize       = QSPI_ADDRESS_16_BITS;
	sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	sCommand.DataMode          = QSPI_DATA_4_LINES;
	sCommand.DummyCycles       = 0;
	sCommand.DdrMode           = QSPI_DDR_MODE_DISABLE;
	sCommand.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	sCommand.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

	sCommand.Address = ColAddr;
	sCommand.NbData  = Size;


	/* Configure the command */
	if (HAL_QSPI_Command(hqspi, &sCommand, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	/* Transmission of the data */
	if (HAL_QSPI_Transmit(hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}


	return QSPI_OK;
}


uint8_t TC58CVG1_QSPI_UnlockAllBlocks(QSPI_HandleTypeDef *hqspi)
{
	return TC58CVG1_QSPI_SetFeature(hqspi, TC58CVG1_FT_A0_ADR, 0);
}


void TC58CVG1_QSPI_EraseChip(QSPI_HandleTypeDef *hqspi)
{
	uint32_t offset = 0;
	for(uint32_t i = 0; i < (TC58CVG1_FLASH_SIZE / TC58CVG1_BLOCK_SIZE); i++)
	{
		TC58CVG1_QSPI_EraseBlock(hqspi, offset);
		offset += (TC58CVG1_BLOCK_SIZE / TC58CVG1_PAGE_SIZE);
	}
}







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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

