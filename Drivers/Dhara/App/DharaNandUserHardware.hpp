/*
 * DharaNandUserHardware.hpp
 *
 *  Created on: 8 янв. 2022 г.
 *      Author: Sergey Ladanov
 */

#ifndef DHARA_APP_DHARANANDUSERHARDWARE_HPP_
#define DHARA_APP_DHARANANDUSERHARDWARE_HPP_

#include "DharaFTL.hpp"

#if DHARA_USE_CPP_LAYER != 0

class DharaNandUserHardware : public DharaNandDriver
{
public:

	DharaNandUserHardware(uint8_t Log2PageSize, uint8_t Log2Ppb, unsigned int NumBlocks)
        : DharaNandDriver(Log2PageSize, Log2Ppb, NumBlocks)
    {

    }
private:
    // Nand IO methods
    virtual int SectorIsBad(dhara_block_t bno) override;
    virtual void MarkBadSector(dhara_block_t bno) override;
    virtual int EraseBlock(dhara_block_t bno, dhara_error_t *err = nullptr) override;
    virtual int Prog(dhara_page_t p, const uint8_t *data, dhara_error_t *err = nullptr) override;
    virtual int BlockIsFree(dhara_page_t p) override;
    virtual int Read(dhara_page_t p, size_t offset, size_t length, uint8_t *data, dhara_error_t *err = nullptr) override;
    virtual int CopyPage(dhara_page_t src, dhara_page_t dst, dhara_error_t *err = nullptr) override;
};

#endif

#endif /* DHARA_APP_DHARANANDUSERHARDWARE_HPP_ */
