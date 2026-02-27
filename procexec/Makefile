FLAGS = -Wall -g 
LIBS = ../lib/error_functions.c ../lib/get_num.c
.PHONY: clean all

%: %.c
	gcc ${FLAGS} $< ${LIBS} -o $@.o

clean:
	rm -f *.o 