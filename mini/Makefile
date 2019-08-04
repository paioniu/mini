

NAME := mini

TGT := lib$(NAME).a

CFLAGS := -g03 -Wall -DDEBUG

OBJS := $(patsubst %.c,%.o,$(SRCS))

SRCDIRS := base

SRC_EXCLUDE := base/list.c

SRCS := $(filter-out $(SRC_EXCLUDE), $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c)))

OBJS := $(patsubst %.c,%.o,$(SRCS))

lib_debug: $(TGT)

lib_release: 

$(TGT): $(OBJS)
	ar -cr $@ $(OBJS)

./%.o: ./%.c
	gcc -c -o $@ $< $(CFLAGS)

base/%.o: base/%.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(TGT) $(OBJS)

