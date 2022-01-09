/*
 * dhara.h
 *
 *  Created on: 8 jan 2022 г.
 *      Author: Sergey Ladanov
 */

#ifndef DHARA_DHARA_H_
#define DHARA_DHARA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define DHARA_USE_CPP_LAYER 1

#include "map.h"
#include "nand.h"
#include <stdint.h>



static inline uint32_t dhara_nand_pagesize(const struct dhara_nand *n)
{
	return (1 << n->log2_page_size);
}

static inline uint32_t dhara_nand_blocksize(const struct dhara_nand *n)
{
	return (1 << (n->log2_page_size + n->log2_ppb));
}

static inline uint32_t dhara_nand_log2blocksize(const struct dhara_nand *n)
{
	return (n->log2_page_size + n->log2_ppb);
}


static inline uint32_t dhara_nand_numblocks(const struct dhara_nand *n)
{
	return n->num_blocks;
}


static inline uint32_t dhara_map_memsize(const struct dhara_map *m)
{
	return dhara_map_capacity(m) * dhara_nand_pagesize(m->journal.nand);
}


static inline uint32_t dhara_map_blocksize(const struct dhara_map *m)
{
	return dhara_nand_pagesize(m->journal.nand);
}

int dhara_read_blocks(struct dhara_map *m, uint8_t *buf, uint32_t blk_addr, uint32_t blk_len);

int dhara_write_blocks(struct dhara_map *m, uint8_t *buf, uint32_t blk_addr, uint32_t blk_len);

#ifdef __cplusplus
}
#endif



#endif /* DHARA_DHARA_H_ */
