#ifndef _VIC_H
#define _VIC_H 1

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>

#include "vic-funcs.h"

#define vic_sys_print(x) printf("%s", x)
#define vic_sys_out(x) printf("%c", x)
#define vic_sys_println(x) printf("%s\n", x)

#define VIC_PS1 "+> "
#define VIC_BUFFER_SIZE 128
#define VIC_FUNCS_COUNT 64

#define VIC_NO_ERR 0
#define VIC_FUNC_INSUFFICIENT_SPACE 1

/* vic-serial.c */
extern char vic_buffer[VIC_BUFFER_SIZE];
extern int vic_buffer_len;

void vic_buffer_append(char i);
void vic_process(char input);

/* vic-funcs.c */
extern VIC_FUNC vic_funcs[VIC_FUNCS_COUNT];
extern int vic_funcs_len;

int vic_fn_add(const char *raw_name, void (*p_func)());
void vic_funcs_clear(void);

/* vic.c */
void vic_init();

#endif

/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:expandtabs */
