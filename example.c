#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define SHELL 1
#define DEBUG 1
#include <vic.h>

void test()
{
        int t;
        vic_args("%d", &t);
        printf("test, args: %s, t = %d\n", vic_buff, t);
        vic_return("%d", t+1);
}

void whl()
{
        char inp[65];
        vic_args("%s", &inp);


}


void ls()
{
        vic_print("listing");
}

void rm()
{
        char i[20];
        vic_args("%s", &i);
        printf("Removing %s\n", i);
        vic_exec("ls;");

}

void milis()
{
        vic_func();

        vic_return("%d", rand());
}

int main(void)
{

        //vic_fn_add("A", &test);
        vic_init();

        vic_fn_add("l", &ls);
        vic_fn_add("rm", &rm);
        vic_fn_add("test", &test);
        vic_fn_add("m", &milis);

        vic_alias_add("time", "rpc;m;shell;");
        vic_exec("ls;");
        while(1){
                char input[200];
                vic_print("+> ");
                gets(input);
                int i;
                for(i = 0; i < strlen(input); i++){
                        vic_process(input[i]);
                }
                vic_process('\n');
        }

        return 0;
}
