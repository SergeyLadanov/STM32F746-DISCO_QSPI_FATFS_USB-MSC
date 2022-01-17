/* Copyright (c) 2010-2011 mbed.org, MIT License
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software
* and associated documentation files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
* BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "QSPI_DISCO_F746NG.h"


//=================================================================================================================
// Public methods
//=================================================================================================================

uint8_t QSPI_DISCO_F746NG::Init(void)
{
  if (!BSP_QSPI_Init())
  {
	  QSPI_Info Info;

	  if (!GetInfo(&Info))
	  {
		  Capacity = Info.FlashSize;
		  SectorSize = Info.EraseSectorSize;
	  }
	  else
	  {
		  return QSPI_ERROR;
	  }
  }
  else
  {
	  return QSPI_ERROR;
  }

  return QSPI_OK;
}

//uint8_t QSPI_DISCO_F746NG::DeInit(void)
//{
//  return BSP_QSPI_DeInit();
//}
//
//
//uint8_t QSPI_DISCO_F746NG::Write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
//{
//  return BSP_QSPI_Write(pData, WriteAddr, Size);
//}
//
//uint8_t QSPI_DISCO_F746NG::Erase_Block(uint32_t BlockAddress)
//{
//  return BSP_QSPI_EraseBlock(BlockAddress);
//}
//
//uint8_t QSPI_DISCO_F746NG::GetStatus(void)
//{
//  return BSP_QSPI_GetStatus();
//}
//
uint8_t QSPI_DISCO_F746NG::GetInfo(QSPI_Info* pInfo)
{
  return BSP_QSPI_GetInfo(pInfo);
}


//=================================================================================================================
// Private methods
//=================================================================================================================
