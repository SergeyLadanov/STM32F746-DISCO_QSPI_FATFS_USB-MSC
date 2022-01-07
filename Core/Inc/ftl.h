/*
 * ftl.h
 *
 *  Created on: 7 янв. 2022 г.
 *      Author: Acer
 */

#ifndef INC_FTL_H_
#define INC_FTL_H_

#include "nand.h"

/* Simulated NAND layer. This layer reads and writes to an in-memory
 * buffer.
 */
extern struct dhara_nand sim_nand;

/* Reset to start-up defaults */
void sim_reset(void);

/* Dump statistics and status */
void sim_dump(void);

/* Halt/resume counting of statistics */
void sim_freeze(void);
void sim_thaw(void);

/* Set faults on individual blocks */
void sim_set_failed(dhara_block_t blk);
void sim_set_timebomb(dhara_block_t blk, int ttl);

/* Create some factory-marked bad blocks */
void sim_inject_bad(int count);

/* Create some unmarked bad blocks */
void sim_inject_failed(int count);

/* Create a timebomb on the given block */
void sim_inject_timebombs(int count, int max_ttl);



#endif /* INC_FTL_H_ */
