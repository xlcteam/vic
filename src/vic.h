#ifndef _VIC_H
#define _VIC_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>


#define VIC_XSTR(s) VIC_STR(s)
#define VIC_STR(s) #s

#ifdef ARDUINO

#include <avr/pgmspace.h>

#else

#define PROGMEM
#define pgm_read_word *
#define strncpy_P strncpy

#endif /* arduino */

#if defined(__AVR_ATmega168__) /* 1 kB */

#define VIC_FUNCS_COUNT 8
#define VIC_VARS_COUNT 8

#elif defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)/* 2, 2.5 kB */

#define VIC_FUNCS_COUNT 16
#define VIC_VARS_COUNT 16

#elif defined(__AVR_ATmega1280__) /* 4 kB */

#define VIC_FUNCS_COUNT 32
#define VIC_VARS_COUNT 32

#elif defined(__AVR_ATmega2560__) /* 8 kB */

#define VIC_FUNCS_COUNT 64
#define VIC_VARS_COUNT 64

#else

#define VIC_FUNCS_COUNT 64
#define VIC_VARS_COUNT 64

#endif

#define VIC_BUFFER_IN_SIZE 64
#define VIC_BUFFER_OUT_SIZE 128
#define VIC_FUNC_NAME_LEN 8
#define VIC_VAR_NAME_LEN 8
#define VIC_VAR_VAL_LEN 16

#define VIC_VAR_NONE 0x00
#define VIC_VAR_INT8 0x01
#define VIC_VAR_UINT8 0x02
#define VIC_VAR_INT16 0x03
#define VIC_VAR_UINT16 0x04
/*
#define VIC_VAR_INT32 0x05
#define VIC_VAR_UINT32 0x06
*/
#define VIC_VAR_FLOAT 0x07

#define VIC_ERR_NO 0
#define VIC_ERR_INSUFFICIENT_SPACE 1
#define VIC_ERR_INVALID_NAME 2
#define VIC_ERR_INVALID_ARGS 3

#define VIC_PS1 "+> "
#define VIC_INIT_MESS "vic initialized"

/* vic-serial.c */
extern uint8_t vic_buffer_in_len;
extern char vic_buffer_in[VIC_BUFFER_IN_SIZE + 1];
extern char vic_buffer_out[VIC_BUFFER_OUT_SIZE + 1];

#define vic_printf(format, ...) do {\
    snprintf(vic_buffer_out, VIC_BUFFER_OUT_SIZE + 1, (format), __VA_ARGS__);\
    vic_print(vic_buffer_out); } while(0)
#define vic_println(s) do { vic_print(s); vic_out('\n'); } while(0)

void vic_out(char c);
void vic_print(const char *s);
void vic_print_err(uint8_t id);

void vic_buffer_append(char i);
void vic_process(char input);
void vic_output_set(void (*output_func)(char));

/* vic-funcs.c */
uint8_t vic_fn_add(const char *raw_name, void (*p_func)(void));
uint8_t vic_fn_call(const char *raw_name);
uint8_t vic_fn_rm(const char *raw_name);
void vic_funcs_clear(void);

/* vic-var.c */
uint8_t vic_var_set(const char *raw_name, const char *value);
uint8_t vic_var_get(const char *raw_name, char **value_out);
uint8_t vic_var_bind(const char *raw_name, void *bind_val, const uint8_t type);
void vic_vars_clear(void);

/* vic-exec.c */
#define vic_args(format, ...) (sscanf(vic_args_s, (format), __VA_ARGS__))

extern char *vic_args_s;

uint8_t vic_exec(char *line);

/* vic.c */
extern const char * const vic_err_msg[] PROGMEM;

void vic_init(void (*output_func)(char));

#endif

/* vim: set tabstop=4:softtabstop=4:shiftwidth=4:expandtabs */
