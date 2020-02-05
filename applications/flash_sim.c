/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-24     armink       the first version
 */

#include <stdio.h>
#include <fal.h>
#include <string.h>

#define FLASH_FILE_PATH       "/flash_sim.bin"
#define FLASH_FILE_SIZE       (1*1024*1024)
#define FLASH_FILE_PAGE_SIZE  (4*1024)

static FILE *flash_fp = NULL;

static int init(void)
{
    flash_fp = fopen(FLASH_FILE_PATH, "ab+");
    if (flash_fp) {
        fclose(flash_fp);
    }

    return 0;
}

static int read(long offset, uint8_t *buf, size_t size)
{
    flash_fp = fopen(FLASH_FILE_PATH, "rb+");
    if (flash_fp) {
        fseek(flash_fp, offset, SEEK_SET);
        fread(buf, size, 1, flash_fp);
        fclose(flash_fp);
        return size;
    } else {
        return -1;
    }
}

static int write(long offset, const uint8_t *buf, size_t size)
{
    flash_fp = fopen(FLASH_FILE_PATH, "rb+");

    if (flash_fp){
        fseek(flash_fp, offset, SEEK_SET);
        fwrite(buf, size, 1, flash_fp);
        fclose(flash_fp);
        return size;
    } else {
        return -1;
    }
}

static int erase(long offset, size_t size)
{
#define BUF_SIZE 32

    uint8_t buf[BUF_SIZE];
    size_t i;

    flash_fp = fopen(FLASH_FILE_PATH, "rb+");

    if (flash_fp) {
        fseek(flash_fp, offset, SEEK_SET);
        for (i = 0; i * BUF_SIZE < size; i++)
        {
            memset(buf, 0xFF, BUF_SIZE);
            fwrite(buf, BUF_SIZE, 1, flash_fp);
        }

        memset(buf, 0xFF, BUF_SIZE);
        fwrite(buf, size - i * BUF_SIZE, 1, flash_fp);
        fclose(flash_fp);

        return size;
    } else {
        return -1;
    }
}
const struct fal_flash_dev flash_sim_dev = { "flash_sim", 0, FLASH_FILE_SIZE, FLASH_FILE_PAGE_SIZE, {init, read, write, erase} };
