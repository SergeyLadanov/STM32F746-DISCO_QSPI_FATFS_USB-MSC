/*
 * dhara.c
 *
 *  Created on: 8 jan 2022 г.
 *      Author: Sergey Ladanov
 */

#include "dhara.h"


int dhara_read_blocks(struct dhara_map *m, uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
{
	dhara_error_t err;

	for (uint32_t i = 0; i < blk_len; i++)
	{
		if (dhara_map_read(m, blk_addr + i, &buf[i * dhara_map_blocksize(m)], &err) < 0)
		{
			return -1;
		}
	}

	return 0;
}


int dhara_write_blocks(struct dhara_map *m, uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
{
	dhara_error_t err;

	for (uint32_t i = 0; i < blk_len; i++)
	{
		if (dhara_map_write(m, blk_addr + i, (const uint8_t *) &buf[i * dhara_map_blocksize(m)], &err) < 0)
		{
			return -1;
		}
	}

	return 0;
}


