/*
 * NandExample.cpp
 *
 *  Created on: 8 янв. 2022 г.
 *      Author: Acer
 */

#include "NandExample.hpp"
#include "tc58cvg1_qspi.h"
#include <cstdio>
#include <cstring>

int NandExample::SectorIsBad(dhara_block_t bno)
{
	if (bno >= GetNumBlocks())
	{
		return -1;
	}

	uint8_t Probe = 0xFF;
	uint32_t row = bno << log2_ppb;

	if (TC58CVG1_QSPI_CheckReadPage(QSPI_Ref, row))
	{
		return true;
	}

	if (TC58CVG1_QSPI_ReadFromBuf(QSPI_Ref, &Probe, 2049, 1))
	{
		//printf("Sector is bad!\r\n");
		return true;
	}


	if (!Probe)
	{
		//printf("Sector is bad!\r\n");
		return true;
	}

	//printf("Sector is good!\r\n");

	return false;
}


void NandExample::MarkBadSector(dhara_block_t bno)
{
	if (bno >= GetNumBlocks())
	{
		return;
	}
}


int NandExample::EraseBlock(dhara_block_t bno, dhara_error_t *err)
{

	uint32_t row = bno << (log2_ppb);
	if (bno >= GetNumBlocks())
	{
		return -1;
	}
	//printf("Erasing page: %d!\r\n", (int) (row));
	return TC58CVG1_QSPI_EraseBlock(QSPI_Ref, row);
}


int NandExample::Prog(dhara_page_t p, const uint8_t *data, dhara_error_t *err)
{
	uint32_t bno = p >> log2_ppb;

	if ((bno < 0) || (bno >= GetNumBlocks())) 
	{
		return -1;
	}


	if (TC58CVG1_QSPI_WriteToBuf(QSPI_Ref, (uint8_t *) data, 0, GetPageSize()))
	{
		return -1;
	}

	//printf("Write adr: %d\r\n", (int) p);


	return TC58CVG1_QSPI_ProgramExecute(QSPI_Ref, p);
}


int NandExample::BlockIsFree(dhara_page_t p)
{
	uint32_t bno = p >> log2_ppb;


	if ((bno < 0) || (bno >= GetNumBlocks()))
	{
		return false;
	}

	//uint32_t adr = (bno << GetLog2BlockSize());

	uint32_t *buf = new uint32_t [GetPageSize() / sizeof(uint32_t)];

	int result = true;


	for (uint8_t i = 3; i > 0; i--)
	{
		if (TC58CVG1_QSPI_ReadPage(QSPI_Ref, p))
		{
			//printf("Sector is bad!\r\n");
			if (!i)
			{
				result = false;
			}
		}
		else
		{
			break;
		}
	}



	if (TC58CVG1_QSPI_ReadFromBuf(QSPI_Ref, (uint8_t *) buf, 0, GetPageSize()))
	{
		result = false;
	}

	for (uint32_t k = 0; k < GetPageSize() / sizeof(uint32_t); k++)
	{
		if (buf[k] != 0xFFFFFFFF)
		{
			result = false;
			break;
		}
	}



//	if (result)
//	{
//		printf("Page is erased!\r\n");
//	}
//	else
//	{
//		printf("Page is not erased!\r\n");
//	}

	delete[] buf;


	return result;
}


int NandExample::Read(dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err)
{
	uint32_t bno = p >> log2_ppb;
	//uint32_t addr = (p << log2_page_size);

	if ((bno < 0) || (bno >= GetNumBlocks()))
	{
		return -1;
	}

	if ((offset > GetPageSize()) || (length > GetPageSize()) || (offset + length > GetPageSize()))
	{
		return -1;
	}



	if (TC58CVG1_QSPI_ReadPage(QSPI_Ref, p))
	{
		//printf("Sector is bad!\r\n");
		return -1;
	}


	//printf("Read adr: %d\r\n", (int) (p));


	return TC58CVG1_QSPI_ReadFromBuf(QSPI_Ref, data, offset, length);;
}


int NandExample::CopyPage(dhara_page_t src, dhara_page_t dst, dhara_error_t *err)
{
    uint8_t *buf = new uint8_t[GetPageSize()];

    int status = 0;

	if ((Read(src, 0, GetPageSize(), buf, err) < 0) || (Prog(dst, buf, err) < 0))
    {
        status = -1;
    }

    delete[] buf;
		
	return status;
}

