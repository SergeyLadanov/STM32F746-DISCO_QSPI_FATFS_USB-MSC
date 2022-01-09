/*
 * DharaFTL.hpp
 *
 *  Created on: 8 янв. 2022 г.
 *      Author: Sergey Ladanov
 */

#ifndef DHARA_APP_DHARAFTL_HPP_
#define DHARA_APP_DHARAFTL_HPP_

#include "dhara.h"
#include "DharaNandDriver.hpp"

#if DHARA_USE_CPP_LAYER != 0

class DharaFTL
{
private:
    struct dhara_map Map;

public:
    int Init(DharaNandDriver *nand_driver, uint8_t *page_buf, uint8_t gc_ratio, dhara_error_t *err = nullptr)
    {
        dhara_map_init(&Map, nand_driver, page_buf, gc_ratio);
        return dhara_map_resume(&Map, err);
    }

    inline int Read(dhara_sector_t s, uint8_t *data, dhara_error_t *err = nullptr)
    {
        return dhara_map_read(&Map, s, data, err);
    }

    inline int Write(dhara_sector_t s, const uint8_t *data, dhara_error_t *err = nullptr)
    {
        return dhara_map_write(&Map, s, data, err);
    }

    inline int Sync(dhara_error_t *err = nullptr)
    {
        return dhara_map_sync(&Map, err);
    }

    inline uint32_t GetMemSize(void)
    {
        return dhara_map_memsize(&Map);
    }

    inline uint32_t GetBlockSize(void)
    {
        return dhara_map_blocksize(&Map);
    }

    inline uint32_t GetBlockNum(void)
    {
        return dhara_map_capacity(&Map);
    }

    inline int ReadBlocks(uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
    {
        return dhara_read_blocks(&Map, buf, blk_addr, blk_len);
    }

    inline int WriteBlocks(uint8_t *buf, uint32_t blk_addr, uint32_t blk_len)
    {
        return dhara_write_blocks(&Map, buf, blk_addr, blk_len);
    }
};

#endif

#endif /* DHARA_APP_DHARAFTL_HPP_ */
