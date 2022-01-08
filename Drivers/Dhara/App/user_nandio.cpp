/*
 * ftl.cpp
 *
 *  Created on: 7 янв. 2022 г.
 *      Author: Acer
 */
#include "dhara.h"
#include "QSPI_DISCO_F746NG.h"
#include <cstdio>
#include <cstring>

extern QSPI_DISCO_F746NG qspi;


//--------------------------------------------
int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t bno)
{
	if (bno >= dhara_nand_numblocks(n)) {
		return -1;
	}

	return false;
}

//--------------------------------------------
void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t bno)
{
	if (bno >= dhara_nand_numblocks(n)) {
		return;
	}

}

//--------------------------------------------
int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t bno,
		     dhara_error_t *err)
{
	uint32_t addr = (bno << dhara_nand_log2blocksize(n));

	if (bno >= dhara_nand_numblocks(n))
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
	uint32_t bno = p >> n->log2_ppb;
	uint32_t addr = (p << n->log2_page_size);

	if ((bno < 0) || (bno >= dhara_nand_numblocks(n))) {
		return -1;
	}


	//printf("Write adr: %d\r\n", (int) addr);

	return qspi.Write((uint8_t *) data, addr, dhara_nand_pagesize(n));

}

//--------------------------------------------
int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p)
{
	uint32_t bno = p >> n->log2_ppb;

	uint32_t probe = 0x00000000;

	uint32_t size_block = dhara_nand_pagesize(n) / 4;

	if ((bno < 0) || (bno >= dhara_nand_numblocks(n)))
	{
		return false;
	}

	uint32_t adr = (bno << dhara_nand_log2blocksize(n));


	for (uint32_t i = 0; i < size_block; i += sizeof(probe))
	{
		if (qspi.Read((uint8_t *) &probe, adr + i, sizeof(probe)))
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

//--------------------------------------------
int dhara_nand_read(const struct dhara_nand *n, dhara_page_t p,
		    size_t offset, size_t length,
		    uint8_t *data, dhara_error_t *err)
{
	uint32_t bno = p >> n->log2_ppb;
	uint32_t addr = (p << n->log2_page_size);

	if ((bno < 0) || (bno >= dhara_nand_numblocks(n)))
	{
		return -1;
	}

	if ((offset > dhara_nand_pagesize(n)) || (length > dhara_nand_pagesize(n)) ||
	    (offset + length > dhara_nand_pagesize(n)))
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
	uint8_t *buf = new uint8_t [dhara_nand_pagesize(n)];

	if ((dhara_nand_read(n, src, 0, dhara_nand_pagesize(n), buf, err) < 0) ||
	    (dhara_nand_prog(n, dst, buf, err) < 0))
		return -1;


	delete[] buf;

	return 0;
}



