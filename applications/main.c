#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>
#include <fal.h>
#include <easyflash.h>

static void entry(void *para)
{
    rt_tick_t tick;

    while(1)
    {
        tick = rt_tick_get() + rt_tick_from_millisecond(rand()%10);
        while(rt_tick_get() < tick)
        {
        }
        rt_thread_delay(10);
    }
}

void sys_load_test_start(void)
{
    rt_thread_t thread;
    int i;
    char name[] = "thread-a";

#define MAX_THREAD 15

    for (i = 0; i < MAX_THREAD; i++)
    {
        name[7] = 'a' + i;
        thread = rt_thread_create(name, entry, RT_NULL, 512, i * 2, 10);
        if (thread)
            rt_thread_startup(thread);
    }
}

int main(void)
{
    printf("hello rt-thread\n");

    fal_init();
    easyflash_init();

    sys_load_test_start();

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

void big_env_blob_test()
{
#define ENV_SIZE  512

    uint8_t value[ENV_SIZE];
    uint8_t read_value[ENV_SIZE];
    size_t return_len, saved_value_len;
    EfErrCode result;


    for (size_t i = 0; i < ENV_SIZE; i++)
    {
        value[i] = i % 256;
    }

    result =  ef_set_env_blob("big_env", value, ENV_SIZE);
    rt_kprintf("ef_set_env_blob return: %d, size: %d\n", result, ENV_SIZE);

    return_len = ef_get_env_blob("big_env", read_value, ENV_SIZE, &saved_value_len);
    rt_kprintf("ef_get_env_blob return: %d, saved_value_len: %d\n", return_len, saved_value_len);

    rt_kprintf("memcmp result: %d\n", memcmp(value, read_value, ENV_SIZE));
}
MSH_CMD_EXPORT(big_env_blob_test, big env blob test)
