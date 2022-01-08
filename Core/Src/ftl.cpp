/*
 * ftl.cpp
 *
 *  Created on: 7 янв. 2022 г.
 *      Author: Acer
 */
#include "nand.h"
#include "QSPI_DISCO_F746NG.h"
#include <cstdio>
#include <cstring>

extern QSPI_DISCO_F746NG qspi;

#define LOG2_PAGE_SIZE		9
#define LOG2_PAGES_PER_BLOCK	3
#define LOG2_BLOCK_SIZE		(LOG2_PAGE_SIZE + LOG2_PAGES_PER_BLOCK)
#define NUM_BLOCKS		4096

#define PAGE_SIZE		(1 << LOG2_PAGE_SIZE)
#define PAGES_PER_BLOCK		(1 << LOG2_PAGES_PER_BLOCK)
#define BLOCK_SIZE		(1 << LOG2_BLOCK_SIZE)
#define MEM_SIZE		(NUM_BLOCKS * BLOCK_SIZE)


struct dhara_nand sim_nand = {
	.log2_page_size		= LOG2_PAGE_SIZE,
	.log2_ppb		= LOG2_PAGES_PER_BLOCK,
	.num_blocks		= NUM_BLOCKS
};



//--------------------------------------------
int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t bno)
{
	if (bno >= NUM_BLOCKS) {
		return -1;
	}


	return false;
}

//--------------------------------------------
void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t bno)
{
	if (bno >= NUM_BLOCKS) {
		return;
	}

}

//--------------------------------------------
int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t bno,
		     dhara_error_t *err)
{
	uint32_t addr = (bno << LOG2_BLOCK_SIZE);

	if (bno >= NUM_BLOCKS)
	{
		return -1;
	}

	//printf("Erasing page: %d!\r\n", (int) addr);

	return qspi.Erase_Block(addr);
}

//--------------------------------------------
int dhara_nand_prog(const struct dhara_nand *n, dhara_page_t p,
		    const uint8_t *data, dhara_error_t *err)
{
	const int bno = p >> LOG2_PAGES_PER_BLOCK;
	uint32_t addr = (p << LOG2_PAGE_SIZE);

	if ((bno < 0) || (bno >= NUM_BLOCKS)) {
		return -1;
	}


	//printf("Write adr: %d\r\n", (int) addr);

	return qspi.Write((uint8_t *) data, addr, PAGE_SIZE);

}

//--------------------------------------------
int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p)
{
	const int bno = p >> LOG2_PAGES_PER_BLOCK;

	uint32_t probe = 0x00000000;

	uint32_t size_block = (1 << LOG2_BLOCK_SIZE);

	if ((bno < 0) || (bno >= NUM_BLOCKS))
	{
		return false;
	}

	uint32_t adr = (bno << LOG2_BLOCK_SIZE);


	for (uint32_t i = 0; i < size_block; i += sizeof(probe))
	{
		if (qspi.Read((uint8_t *) &probe, adr + i, sizeof(probe)))
		{
			return false;
		}

		if (probe != 0xFFFFFFFF)
		{
			printf("Page is not erased!\r\n");
			return false;
		}
	}


	printf("Page is erased!\r\n");

	return true;
}

//--------------------------------------------
int dhara_nand_read(const struct dhara_nand *n, dhara_page_t p,
		    size_t offset, size_t length,
		    uint8_t *data, dhara_error_t *err)
{
	const int bno = p >> LOG2_PAGES_PER_BLOCK;
	uint32_t addr = (p << LOG2_PAGE_SIZE);

	if ((bno < 0) || (bno >= NUM_BLOCKS))
	{
		return -1;
	}

	if ((offset > PAGE_SIZE) || (length > PAGE_SIZE) ||
	    (offset + length > PAGE_SIZE))
	{
		return -1;
	}


	//printf("Read adr: %d\r\n", (int) (addr + offset));

	return qspi.Read(data, addr + offset, length);
}


//--------------------------------------------
int dhara_nand_copy(const struct dhara_nand *n,
		    dhara_page_t src, dhara_page_t dst,
		    dhara_error_t *err)
{
	uint8_t buf[PAGE_SIZE];

	if ((dhara_nand_read(n, src, 0, PAGE_SIZE, buf, err) < 0) ||
	    (dhara_nand_prog(n, dst, buf, err) < 0))
		return -1;

	return 0;
}



