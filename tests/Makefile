NAME 	:= mini_tests

TGT 	:= $(NAME)

#CFLAGS 	:= -g03 -Wall -DDEBUG -rdynamic -DCOMPILE_WITH_PARAM_CHECKING -DCOMPILE_WITH_FULL_SHIELD -I..
CFLAGS 	:= -g03 -Wall -DDEBUG -DCOMPILE_WITH_PARAM_CHECKING -DCOMPILE_WITH_FULL_SHIELD -I..

OBJS	:= $(patsubst %.c,%.o,$(wildcard *.c))

All: $(TGT)

$(TGT): $(OBJS)
	gcc -rdynamic -o $@ $(OBJS) -L../mini -lmini
 
%.o: %.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(TGT)
	rm -f $(OBJS)
