OBJDIR := build
EXECUTABLE := text-editor
CLEANTARGETS := $(OBJDIR) $(EXECUTABLE)

SRCDIR := src
INCDIR := include

GITVERSION := $(shell git describe --dirty --broken --tags --always 2>/dev/null || echo unknown)
GITVFILE := $(SRCDIR)/gitversion.c

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC) $(GITVFILE))

LDLIBS := -lcurses
CFLAGS ?= -O2 -march=native -pipe
CPPFLAGS += -I $(INCDIR)
LDFLAGS ?= -Wl,-O1 -Wl,--as-needed

# echo all commands if $V is set; replacing echo commands with "true"
ifneq ($(V),)
	SHELL := sh -x
	Q = true ||
endif

.PHONY: all debug valgrind gdb clean prepare run devlog $(GITVFILE)
.INTERMEDIATE: $(GITVFILE)

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

$(GITVFILE):
	@$(Q)echo "  VERSION	$@"
	@echo "const char *argp_program_version = \"$(GITVERSION)\";" > $@

$(EXECUTABLE): $(OBJ)
	@$(Q)echo "  LD		$@"
	@$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@$(Q)echo "  CC		$@"
	@$(COMPILE.c) $(OUTPUT_OPTION) $<

