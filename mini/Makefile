

NAME := mini

TGT := lib$(NAME).a

#CFLAGS := -g03 -Wall -DDEBUG -rdynamic -DCOMPILE_WITH_PARAM_CHECKING -DCOMPILE_WITH_FULL_SHIELD
CFLAGS := -g03 -Wall -DDEBUG -DTESTING -DCOMPILE_WITH_PARAM_CHECKING -DCOMPILE_WITH_FULL_SHIELD

SRCDIRS := base

SRC_EXCLUDE := base/list.c

SRCS := $(filter-out $(SRC_EXCLUDE), $(foreach dir,$(SRCDIRS),$(wildcard $(dir)/*.c)))

OBJS := $(patsubst %.c,%.o,$(SRCS))

OBJS := $(patsubst %.c,%.o,$(SRCS))

lib_debug: $(TGT)

lib_release: 

lib_release_no_param:
	$(MAKE) CFLAGS="-rdynamic -DCOMPILE_WITH_FULL_SHIELD" NAME="$(NAME)-npck" lib_release

lib_release_no_shield:
	$(MAKE) CFLAGS="-rdynamic -DCOMPILE_WITH_PARAM_CHECKING" NAME="$(NAME)-nshd" lib_release

lib_release_no_barrier:
	$(MAKE) CFLAGS="" NAME="$(NAME)-nobarrier" lib_release

$(TGT): $(OBJS)
	ar -cr $@ $(OBJS)

./%.o: ./%.c
	gcc -c -o $@ $< $(CFLAGS)

base/%.o: base/%.c
	gcc -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(TGT) $(OBJS)

