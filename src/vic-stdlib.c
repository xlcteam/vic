#ifndef _VIC_STDLIB
#define _VIC_STDLIB

#include "vic.h"
#include "vic-funcs.h"
#include "vic-var.h"
#include "stdlib.h"

void vic_set(void)
{
    char var_name[VIC_VAR_NAME_LEN+1] = {'\0'};
    char var_val[VIC_VAR_VAL_LEN+1] = {'\0'};

    const char *args_format = "%"VIC_XSTR(VIC_VAR_NAME_LEN)"s";
    if (vic_args(args_format, var_name) != 1) {
        vic_print_err(VIC_ERR_INVALID_ARGS);
        return;
    }
    args_format = "%*s %"VIC_XSTR(VIC_VAR_VAL_LEN)"s";
    if (vic_args(args_format, var_val) != 1) {
        vic_print_err(VIC_ERR_INVALID_ARGS);
        return;
    }

    uint8_t error = vic_var_set(var_name, var_val);

    vic_print_err(error);
}

void vic_get(void)
{
    char var_name[VIC_VAR_NAME_LEN+1] = {'\0'};
    const char *args_format = "%"VIC_XSTR(VIC_VAR_NAME_LEN)"s";

    if (vic_args(args_format, var_name) != 1) {
        vic_print_err(VIC_ERR_INVALID_ARGS);
        return;
    }

    char *value = NULL;
    uint8_t error = vic_var_get(var_name, &value);

    vic_print_err(error);
    if (value != NULL) {
        vic_println(value);
    }
}

void vic_list(void)
{
    uint8_t i;
    for (i = 0; i < vic_funcs_len; i++) {
        if (vic_funcs[i].p_func != NULL) {
            vic_println(vic_funcs[i].name);
        }
    }
}

void vic_list_vars(void)
{
    uint8_t i;
    for (i = 0; i < vic_vars_len; i++) {
        vic_println(vic_vars[i].name);
    }
}

#endif

/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:expandtabs */
