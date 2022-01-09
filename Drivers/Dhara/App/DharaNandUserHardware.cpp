/*
 * DharaNandUserHardware.cpp
 *
 *  Created on: 8 янв. 2022 г.
 *      Author: Sergey Ladanov
 */

#include "DharaNandUserHardware.hpp"
#include <cstdio>
#include <cstring>

/***CPP INITIALIZATION EXAMPLE***/
/*
DharaFTL Map;

static void Dhara_Init(void)
{
	static NandExample NandFtlDriver(9, 3, 4096, &qspi);
	static uint8_t page_buf[512];


	if (Map.Init(&NandFtlDriver, page_buf, 4))
	{
		Error_Handler();
	}
}
*/

/*****CPP WRITE EXAMPLE*****/
/*
dhara_error_t err;

if (Map.WriteBlocks(buf, blk_addr, blk_len))
{
	return (USBD_FAIL);
}

if (Map.Sync(&err) < 0)
{
	return (USBD_FAIL);
}
*/


/*****CPP READ EXAMPLE*****/
/*
if (Map.ReadBlocks(buf, blk_addr, blk_len))
{
	return (USBD_FAIL);
}
*/



#if DHARA_USE_CPP_LAYER != 0

int DharaNandUserHardware::SectorIsBad(dhara_block_t bno)
{
	if (bno >= GetNumBlocks()) {
		return -1;
	}

	return false;
}


void DharaNandUserHardware::MarkBadSector(dhara_block_t bno)
{
	if (bno >= GetNumBlocks())
	{
		return;
	}
}


int DharaNandUserHardware::EraseBlock(dhara_block_t bno, dhara_error_t *err)
{
	uint32_t addr = (bno << GetLog2BlockSize());

	if (bno >= GetNumBlocks())
	{
		return -1;
	}
	printf("Erasing page: %lu!\r\n", addr);
	return 0;
}


int DharaNandUserHardware::Prog(dhara_page_t p, const uint8_t *data, dhara_error_t *err)
{
	uint32_t bno = p >> log2_ppb;
	uint32_t addr = (p << log2_page_size);

	if ((bno < 0) || (bno >= GetNumBlocks())) 
	{
		return -1;
	}

	printf("Write adr: %lu\r\n", addr);
	return 0;
}


int DharaNandUserHardware::BlockIsFree(dhara_page_t p)
{
	uint32_t bno = p >> log2_ppb;

	uint32_t probe = 0x00000000;

	uint32_t size_block = GetPageSize() / 4;

	(void) size_block;

	if ((bno < 0) || (bno >= GetNumBlocks()))
	{
		return false;
	}

	uint32_t addr = (bno << GetLog2BlockSize());

	(void) addr;


	if (probe != 0xFFFFFFFF)
	{
		printf("Page is not erased!\r\n");
		return false;
	}

	printf("Page is erased!\r\n");
	return true;
}


int DharaNandUserHardware::Read(dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err)
{
	uint32_t bno = p >> log2_ppb;
	uint32_t addr = (p << log2_page_size);

	if ((bno < 0) || (bno >= GetNumBlocks()))
	{
		return -1;
	}

	if ((offset > GetPageSize()) || (length > GetPageSize()) || (offset + length > GetPageSize()))
	{
		return -1;
	}


	printf("Read adr: %lu\r\n", (addr + offset));

	return 0;
}


int DharaNandUserHardware::CopyPage(dhara_page_t src, dhara_page_t dst, dhara_error_t *err)
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

#endif
