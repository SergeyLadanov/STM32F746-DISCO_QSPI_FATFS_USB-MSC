/*
 * dhara_nandio.cpp
 *
 *  Created on: 8 янв. 2022 г.
 *      Author: Sergey Ladanov
 */

#include "DharaNandDriver.hpp"

#if DHARA_USE_CPP_LAYER != 0
//--------------------------------------------
int dhara_nand_is_bad(const struct dhara_nand *n, dhara_block_t bno)
{
    DharaNandDriver *drv = (DharaNandDriver *) n;
    return drv->SectorIsBad(bno);
}

//--------------------------------------------
void dhara_nand_mark_bad(const struct dhara_nand *n, dhara_block_t bno)
{
    DharaNandDriver *drv = (DharaNandDriver *) n;
    drv->MarkBadSector(bno);
}

//--------------------------------------------
int dhara_nand_erase(const struct dhara_nand *n, dhara_block_t bno,
		     dhara_error_t *err)
{
    DharaNandDriver *drv = (DharaNandDriver *) n;
	return drv->EraseBlock(bno, err);
}

//--------------------------------------------
int dhara_nand_prog(const struct dhara_nand *n, dhara_page_t p,
		    const uint8_t *data, dhara_error_t *err)
{
    DharaNandDriver *drv = (DharaNandDriver *) n;
    return drv->Prog(p, data, err);
}

//--------------------------------------------
int dhara_nand_is_free(const struct dhara_nand *n, dhara_page_t p)
{
    DharaNandDriver *drv = (DharaNandDriver *) n;
    return drv->BlockIsFree(p);
}

//--------------------------------------------
int dhara_nand_read(const struct dhara_nand *n, dhara_page_t p,
		    size_t offset, size_t length,
		    uint8_t *data, dhara_error_t *err)
{
    DharaNandDriver *drv = (DharaNandDriver *) n;
    return drv->Read(p, offset, length, data, err);
}


//--------------------------------------------
int dhara_nand_copy(const struct dhara_nand *n,
		    dhara_page_t src, dhara_page_t dst,
		    dhara_error_t *err)
{
    DharaNandDriver *drv = (DharaNandDriver *) n;
    return drv->CopyPage(src, dst, err);
}


#endif

