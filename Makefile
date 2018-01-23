OBJDIR := build
EXECUTABLE := text-editor
CLEANTARGETS := $(OBJDIR) $(EXECUTABLE)
GITVERSION := $(shell git describe --dirty --broken --tags --always)

SRC := $(wildcard *.c)
OBJ := $(addprefix $(OBJDIR)/, $(SRC:.c=.o) gitversion.o)
LDLIBS := -lcurses


CFLAGS ?= -O2 -march=native -pipe
LDFLAGS ?= -Wl,-O1 -Wl,--as-needed

# echo all commands if $V is set; replacing echo commands with "true"
ifneq ($(V),)
	SHELL := sh -x
	Q = true ||
endif

.PHONY: all debug valgrind gdb clean prepare run devlog gitversion.c
.INTERMEDIATE: gitversion.c

all: prepare $(EXECUTABLE)

#debug: CC=clang
#debug: CFLAGS=-g -Weverything
debug: CFLAGS=-g
debug: clean all
#	clang-tidy $(SRC)

valgrind: debug
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(EXECUTABLE)

gdb: debug
	gdb $(EXECUTABLE)

clean:
	@$(foreach i, $(CLEANTARGETS), $(Q)echo "  CLEAN		$(i)"; rm -rf $(i);)

prepare:
	@$(Q)echo "  MKDIR		$(OBJDIR)"
	@mkdir -p $(OBJDIR)

run: all
	./$(EXECUTABLE)

devlog:
	@$(Q)echo "  MKDEVLOG	DEVLOG"
	@housekeeping/mkdevlog

gitversion.c: .git/HEAD .git/index
	@$(Q)echo "  VERSION	$@"
	@echo "const char *argp_program_version = \"$(GITVERSION)\";" > $@

$(EXECUTABLE): $(OBJ)
	@$(Q)echo "  LD		$@"
	@$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: %.c
	@$(Q)echo "  CC		$@"
	@$(CC) $(CFLAGS) -c -o $@ $<

