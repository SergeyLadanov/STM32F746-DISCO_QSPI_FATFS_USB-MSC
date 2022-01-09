/*
 * DharaNandDriver.hpp
 *
 *  Created on: 8 янв. 2022 г.
 *      Author: Sergey Ladanov
 */

#ifndef DHARA_APP_DHARANANDDRIVER_HPP_
#define DHARA_APP_DHARANANDDRIVER_HPP_

#include "dhara.h"

#if DHARA_USE_CPP_LAYER != 0

class DharaNandDriver : public dhara_nand
{
public:
    DharaNandDriver(uint8_t Log2PageSize, uint8_t Log2Ppb, unsigned int NumBlocks)
    {
        log2_page_size = Log2PageSize;
        log2_ppb = Log2Ppb;
        num_blocks = NumBlocks;
    }

    virtual int SectorIsBad(dhara_block_t bno) = 0;
    virtual void MarkBadSector(dhara_block_t bno) = 0;
    virtual int EraseBlock(dhara_block_t bno, dhara_error_t *err = nullptr) = 0;
    virtual int Prog(dhara_page_t p, const uint8_t *data, dhara_error_t *err = nullptr) = 0;
    virtual int BlockIsFree(dhara_page_t p) = 0;
    virtual int Read(dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err = nullptr) = 0;
    virtual int CopyPage(dhara_page_t src, dhara_page_t dst, dhara_error_t *err = nullptr) = 0;

    inline uint32_t GetPageSize(void)
    {
        return dhara_nand_pagesize(this);
    }

    inline uint32_t GetBlockSize(void)
    {
        return dhara_nand_blocksize(this);
    }

    inline uint32_t GetLog2BlockSize(void)
    {
        return dhara_nand_log2blocksize(this);
    }

    inline uint32_t GetNumBlocks(void)
    {
        return dhara_nand_numblocks(this);
    }
};

#endif

#endif /* DHARA_APP_DHARANANDDRIVER_HPP_ */
