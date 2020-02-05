/*
 * File      : fal_cfg.h
 * COPYRIGHT (C) 2012-2018, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-17     armink       the first version
 */
#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#include <board.h>

/* ===================== Flash device Configuration ========================= */
/* enable SFUD flash driver sample */
#define FAL_FLASH_PORT_DRIVER_SFUD

extern const struct fal_flash_dev flash_sim_dev;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &flash_sim_dev,                                                  \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                          \
{                                                                               \
    {FAL_PART_MAGIC_WROD,"easyflash",    "flash_sim",         0,  512*1024, 0}, \
    {FAL_PART_MAGIC_WROD, "download",    "flash_sim",  512*1024, 1024*1024, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */

#endif /* _FAL_CFG_H_ */
