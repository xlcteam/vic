C=gcc -g -I../src -lm
C_LIBS=-L../ -lvic  

test-vic-funcs: test-vic-funcs.c
	$(C) test-vic-funcs.c -o test-vic-funcs $(C_LIBS)
	./test-vic-funcs

test-vic-serial: test-vic-serial.c
	$(C) test-vic-serial.c -o test-vic-serial $(C_LIBS)
	./test-vic-serial

test-vic-var: test-vic-var.c
	$(C) test-vic-var.c -o test-vic-var $(C_LIBS)
	./test-vic-var

test-vic-io: test-vic-io.c
	$(C) test-vic-io.c -o test-vic-io $(C_LIBS)
	./test-vic-io

test-vic-exec: test-vic-exec.c
	$(C) test-vic-exec.c -o test-vic-exec $(C_LIBS)
	./test-vic-exec

all-tests: test-vic-serial test-vic-funcs test-vic-exec test-vic-var
	@echo "All passed!"

clean: 
	rm -f test-serial test-funcs test-var test-exec
	touch test-vic-var.c
	touch test-vic-serial.c
	touch test-vic-funcs.c
	touch test-vic-exec.c
