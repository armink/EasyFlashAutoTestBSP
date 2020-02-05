/*
 * Copyright (c) 2020, Armink, <armink.ztl@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-25     armink       the first version
 */

#include "utest.h"
#include <easyflash.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ENV_BLOB_NAME "env_blob_test"
#define TEST_ENV_NAME      "env_test"

static void test_easyflash_init(void)
{
    uassert_true(easyflash_init() == EF_NO_ERR);
}

static void test_ef_load_env(void)
{
    uassert_true(ef_load_env() == EF_NO_ERR);
}

static void test_ef_create_env_blob(void)
{
    EfErrCode result = EF_NO_ERR;
    rt_tick_t tick = rt_tick_get(), read_tick;
    size_t save_len, read_len;
    bool find_ok;
    struct env_node_obj env_obj;
    uint8_t value_buf[sizeof(tick)];

    result = ef_set_env_blob(TEST_ENV_BLOB_NAME, &tick, sizeof(tick));
    uassert_true(result == EF_NO_ERR);

    read_len = ef_get_env_blob(TEST_ENV_BLOB_NAME, &read_tick, sizeof(read_tick), &save_len);
    uassert_int_equal(save_len, sizeof(read_tick));
    uassert_int_equal(save_len, read_len);
    uassert_int_equal(tick, read_tick);

    find_ok = ef_get_env_obj(TEST_ENV_BLOB_NAME, &env_obj);
    uassert_true(find_ok);

    read_len = ef_read_env_value(&env_obj, value_buf, sizeof(value_buf));
    uassert_int_equal(read_len, sizeof(value_buf));
    uassert_buf_equal(&tick, value_buf, sizeof(value_buf));
}

static void test_ef_change_env_blob(void)
{
    EfErrCode result = EF_NO_ERR;
    rt_tick_t tick = rt_tick_get(), read_tick;
    size_t save_len, read_len;

    read_len = ef_get_env_blob(TEST_ENV_BLOB_NAME, &read_tick, sizeof(read_tick), &save_len);
    uassert_int_equal(save_len, sizeof(read_tick));
    uassert_int_equal(save_len, read_len);
    uassert_int_not_equal(tick, read_tick);

    result = ef_set_env_blob(TEST_ENV_BLOB_NAME, &tick, sizeof(tick));
    uassert_true(result == EF_NO_ERR);

    read_len = ef_get_env_blob(TEST_ENV_BLOB_NAME, &read_tick, sizeof(read_tick), &save_len);
    uassert_int_equal(save_len, sizeof(read_tick));
    uassert_int_equal(save_len, read_len);
    uassert_int_equal(tick, read_tick);
}

static void test_ef_del_env_blob(void)
{
    EfErrCode result = EF_NO_ERR;
    rt_tick_t tick = rt_tick_get(), read_tick;
    size_t save_len, read_len;

    read_len = ef_get_env_blob(TEST_ENV_BLOB_NAME, &read_tick, sizeof(read_tick), &save_len);
    uassert_int_equal(save_len, sizeof(read_tick));
    uassert_int_equal(save_len, read_len);
    uassert_int_not_equal(tick, read_tick);

    result = ef_set_env_blob(TEST_ENV_BLOB_NAME, NULL, 0);
    uassert_true(result == EF_NO_ERR);

    read_len = ef_get_env_blob(TEST_ENV_BLOB_NAME, &read_tick, sizeof(read_tick), &save_len);
    uassert_int_equal(save_len, 0);
    uassert_int_equal(read_len, 0);
}

static void test_ef_create_env(void)
{
    EfErrCode result = EF_NO_ERR;
    rt_tick_t tick = rt_tick_get(), read_tick;
    char value_buf[14], *read_value;

    snprintf(value_buf, sizeof(value_buf), "%d", tick);
    result = ef_set_env(TEST_ENV_NAME, value_buf);
    uassert_true(result == EF_NO_ERR);

    read_value = ef_get_env(TEST_ENV_NAME);
    uassert_not_null(read_value);
    read_tick = atoi(read_value);
    uassert_int_equal(tick, read_tick);
}

static void test_ef_change_env(void)
{
    EfErrCode result = EF_NO_ERR;
    rt_tick_t tick = rt_tick_get(), read_tick;
    char value_buf[14], *read_value;

    read_value = ef_get_env(TEST_ENV_NAME);
    uassert_not_null(read_value);
    read_tick = atoi(read_value);
    uassert_int_not_equal(tick, read_tick);

    snprintf(value_buf, sizeof(value_buf), "%d", tick);
    result = ef_set_env(TEST_ENV_NAME, value_buf);
    uassert_true(result == EF_NO_ERR);

    read_value = ef_get_env(TEST_ENV_NAME);
    uassert_not_null(read_value);
    read_tick = atoi(read_value);
    uassert_int_equal(tick, read_tick);
}

static void test_ef_del_env(void)
{
    EfErrCode result = EF_NO_ERR;
    rt_tick_t tick = rt_tick_get(), read_tick;
    char *read_value;

    read_value = ef_get_env(TEST_ENV_NAME);
    uassert_not_null(read_value);
    read_tick = atoi(read_value);
    uassert_int_not_equal(tick, read_tick);

    result = ef_del_env(TEST_ENV_NAME);
    uassert_true(result == EF_NO_ERR);

    read_value = ef_get_env(TEST_ENV_NAME);
    uassert_null(read_value);
}

static rt_err_t utest_tc_init(void)
{
    return RT_EOK;
}

static rt_err_t utest_tc_cleanup(void)
{
    return RT_EOK;
}

static void testcase(void)
{
    UTEST_UNIT_RUN(test_easyflash_init);
    UTEST_UNIT_RUN(test_ef_load_env);
    UTEST_UNIT_RUN(test_ef_create_env_blob);
    UTEST_UNIT_RUN(test_ef_change_env_blob);
    UTEST_UNIT_RUN(test_ef_del_env_blob);
    UTEST_UNIT_RUN(test_ef_create_env);
    UTEST_UNIT_RUN(test_ef_change_env);
    UTEST_UNIT_RUN(test_ef_del_env);
}
UTEST_TC_EXPORT(testcase, "packages.tools.easyflash", utest_tc_init, utest_tc_cleanup, 20);
