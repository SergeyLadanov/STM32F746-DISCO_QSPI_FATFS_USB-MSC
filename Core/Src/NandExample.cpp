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

	if ((bno < 0) || (bno >= GetNumBlocks()))
	{
		return false;
	}

	uint32_t adr = (bno << GetLog2BlockSize());

	uint32_t *buf = new uint32_t [GetPageSize() / sizeof(uint32_t)];

	uint32_t offset = 0;

	int result = true;



//	for (int i = 0; i < (1 << log2_ppb); i ++)
//	{
//		if (Flash->Read((uint8_t *) buf, adr + offset, GetPageSize()))
//		{
//			result = false;
//			break;
//		}
//
//		for (uint32_t k = 0; k < (GetPageSize() / sizeof(uint32_t)); k++)
//		{
//			if (buf[k] != 0xFFFFFFFF)
//			{
//				result = false;
//				break;
//			}
//		}
//
//		offset += GetPageSize();
//	}

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

	//return Flash->Read(data, addr + offset, length);
	return 0;
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

