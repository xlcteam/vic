#ifndef _VIC_SERIAL
#define _VIC_SERIAL

#include "vic.h"

#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>


static int vic_buffer_len = 0;

uint8_t vic_returned = 0;

char *vic_buffer;
char *vic_buff;
char *vic_func;

void vic_buffer_append(char i)
{
        vic_buffer = (char *)
                realloc(vic_buffer,
                        (vic_buffer_len + 1) * sizeof(char)
                );
        vic_buffer[vic_buffer_len++] = i;
}

inline void vic_buffer_free(void)
{
	if(vic_buffer != NULL){
		free(vic_buffer);
        	vic_buffer = NULL;
	}

        if(vic_func != NULL){
		free(vic_func);
		vic_func = NULL;
	}

        vic_buffer_len = 0;
}


char* vic_replace_evals(char* input)
{

        char* output;
        output = NULL;

        output = (char*) malloc(sizeof(char));
        uint8_t output_len = 0;

        uint8_t in_eval = 0;

        char* tmp;
        tmp = NULL;
        tmp = (char*) malloc(sizeof(char));
        uint8_t tmp_len = 0;

        do {
                if (*input == '`' && in_eval == 0) {
                        in_eval = 1;
                } else if (*input == '`' && in_eval == 1) {
                        in_eval = 0;

                        tmp = (char *) realloc(tmp, (tmp_len+2) * sizeof(char));
                        tmp[tmp_len++] = ';';
                        tmp[tmp_len++] = '\0';

                        char *out = vic_exec(tmp);

                        output = (char *) realloc(output,
                                        (output_len + strlen(out)) * sizeof(char));

                        memcpy(output + output_len, out, strlen(out));
                        output_len += strlen(out);

                        free(out);
                        free(tmp);
                        tmp_len = 0;

                } else {
                        if (in_eval) {
                                tmp = (char *) realloc(tmp,
                                                (tmp_len+1) * sizeof(char));
                                tmp[tmp_len++] = *input;

                        } else {
                                output = (char *) realloc(output,
                                        (output_len + 1) * sizeof(char));
                                output[output_len++] = *input;
                        }
                }

        } while(*(++input) != '\0');

        output = (char *) realloc(output, (output_len + 1) * sizeof(char));
        output[output_len++] = '\0';

        return output;

}

/*
  Processes input stream.
*/
void vic_process(char input)
{
	if (vic_buffer == NULL)
		vic_buffer = (char *) malloc(sizeof(char));
#ifndef ARDUINO
	if (input == '\n'){
#else
	if (input == 13) {
#endif
		vic_buffer_append(';');
		vic_buffer_append('\0');
#ifdef SHELL
		vic_println(" ");
#endif
		char* output = vic_exec(vic_buffer);
                vic_sys_print(output);
                free(output);

		vic_buffer_free();

#ifdef SHELL
		vic_print(VIC_PS1);
#endif
	} else if (input == 3) { /* reseting Arduino */
		void (*rst)() = 0;
		rst();

	} else if ((input == 8) || (input == 0x7f)) { /* BACKPSACE and DELETE */
		if (vic_buffer_len > 0){
			vic_buffer_len--;

			vic_out((char) 8);
			vic_out(' ');
			vic_out((char) 8);
		}

	} else {
#ifdef SHELL
		vic_out(input);
#endif
		vic_buffer_append(input);
	}

}

char* vic_exec(char *input)
{
        char *buffer = NULL;
        char *func = NULL;
        buffer = (char *) malloc(sizeof(char));
        int len = 0;
        do {
                if(*input == ';') {
                        if (len == 0)
                                continue;

                        // finish the string with '\0'
                        buffer = (char *) realloc(buffer,
                                        (len + 1) * sizeof(char));
                        buffer[len] = '\0';

                        char *replaced_buffer;
                        replaced_buffer = NULL;
                        replaced_buffer = vic_var_replace(buffer);
                        free(buffer);
                        buffer = replaced_buffer;

                        /* in case of calling a procedure */
                        if(func == NULL) {
                                char *tmp;

                                // gives pointer to aliased string
                                if ((tmp = vic_alias(buffer)) != 0){
                                        vic_exec(tmp);
                                        // free(buffer);

                                } else {
                                        vic_func = buffer;
                                        vic_fn_call(vic_func);
                                }


                        } else {
                                vic_func = func;
                                vic_buff = buffer;
#ifdef DEBUG
                                vic_sys_print("buf: '");
                                vic_sys_print(vic_buff);
                                vic_sys_println("'");

                                vic_sys_print("func: ");
                                vic_sys_println(vic_func);
#endif
                                vic_fn_call(vic_func);

                        }

                        if (vic_returned == 0 && (vic_config & VIC_RPC)){
                                vic_out('%');
                                vic_print(vic_func);
                                vic_out('$');

                        }

                        if (vic_returned == 1)
                                vic_returned = 0;


                        len = 0;
                        free(func);
                        func = vic_func = NULL;

                        free(buffer);
                        buffer = NULL;

                        /* ignoring whitespace */
                } else if (isspace(*input) && len == 0) {
                        continue;

                        /* separate the function */
                } else if (*input == ' ' && func == NULL) {

                        buffer = (char *) realloc(buffer,
                                        (len + 1) * sizeof(char));
                        buffer[len++] = '\0';

                        func = buffer;
                        buffer = (char *) malloc(sizeof(char));
                        len = 0;

                } else {

                        buffer = (char *) realloc(buffer,
                                        (len + 1) * sizeof(char));
                        buffer[len++] = *input;

                }

        } while(*(++input) != '\0');

        char* out = vic_io_return();

        out = strdup(out);
        vic_io_clean();

        return out;
}

// from bitlash/src/bitlast-serial.c
void vic_print_int_base(int i, uint8_t n)
{
        char buf[8 * sizeof(uint8_t)];           // stack for the digits
        char *ptr = buf;
        if (i == 0) {
                vic_out('0');
                return;
        }

        while (i > 0) {
                *ptr++ = i % n;
                i /= n;
        }
        while (--ptr >= buf) vic_out((*ptr < 10) ? (*ptr + '0') : (*ptr - 10 + 'A'));
}

void vic_print_int(int n)
{
        if (n < 0) {
                vic_out('-');
                n = -n;
        }

        vic_print_int_base(n, 10);
}

void vic_print_hex(int n)
{
        vic_print("0x");
        vic_print_int_base(n, 16);
}


char* vic_rstrip(char *s)
{
        uint8_t len = strlen(s);
        while(isspace(s[len-1])){
                len--;
        }

        char * out;
        out = malloc((len+1) * sizeof(char));

        memcpy(out, s, len);
        out[len] = '\0';
        return out;
}

#endif
