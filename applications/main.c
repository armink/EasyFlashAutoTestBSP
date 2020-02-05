#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <rtthread.h>
#include <fal.h>
#include <easyflash.h>

int main(void)
{
    printf("hello rt-thread\n");

    fal_init();
    easyflash_init();

    return 0;
}


void file_test(void)
{
    FILE *flash_fp = fopen("/flash_sim.bin", "rb+");

    uint8_t buf[] = {1,2,3,4 };

    fseek(flash_fp, 0, SEEK_SET);
    fwrite(buf, sizeof(buf), 1, flash_fp);
    fflush(flash_fp);
    fclose(flash_fp);
}
MSH_CMD_EXPORT(file_test, file test)

