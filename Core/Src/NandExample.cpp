/*
 * NandExample.cpp
 *
 *  Created on: 8 янв. 2022 г.
 *      Author: Acer
 */

#include "NandExample.hpp"
#include <cstdio>
#include <cstring>

int NandExample::SectorIsBad(dhara_block_t bno)
{
	if (bno >= GetNumBlocks()) {
		return -1;
	}

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
	uint32_t addr = (bno << GetLog2BlockSize());

	if (bno >= GetNumBlocks())
	{
		return -1;
	}
	//printf("Erasing page: %d!\r\n", (int) addr);
	return Flash->Erase_Block(addr);
}


int NandExample::Prog(dhara_page_t p, const uint8_t *data, dhara_error_t *err)
{
	uint32_t bno = p >> log2_ppb;
	uint32_t addr = (p << log2_page_size);

	if ((bno < 0) || (bno >= GetNumBlocks())) 
	{
		return -1;
	}

	//printf("Write adr: %d\r\n", (int) addr);
	return Flash->Write((uint8_t *) data, addr, GetPageSize());
}


int NandExample::BlockIsFree(dhara_page_t p)
{
	uint32_t bno = p >> log2_ppb;

	uint32_t probe = 0x00000000;

	uint32_t size_block = GetPageSize() / 4;

	if ((bno < 0) || (bno >= GetNumBlocks()))
	{
		return false;
	}

	uint32_t adr = (bno << GetLog2BlockSize());


	for (uint32_t i = 0; i < size_block; i += sizeof(probe))
	{
		if (Flash->Read((uint8_t *) &probe, adr + i, sizeof(probe)))
		{
			return false;
		}

		if (probe != 0xFFFFFFFF)
		{
			//printf("Page is not erased!\r\n");
			return false;
		}
	}

	//printf("Page is erased!\r\n");
	return true;
}


int NandExample::Read(dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err)
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


	//printf("Read adr: %d\r\n", (int) (addr + offset));

	return Flash->Read(data, addr + offset, length);
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

