### Project name and output location
PROGRAM = smartcalc
PROGRAM_EXE = $(BIN_DIR)/$(PROGRAM)
### Directories
SRC_DIR = src
BLD_DIR = build
BIN_DIR = $(BLD_DIR)/bin
OBJ_DIR = $(BLD_DIR)/obj
DEP_DIR = $(BLD_DIR)/dep
TST_DIR = test
CHK_DIR = $(TST_DIR)/suites
### Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
DEPS = $(SRCS:$(SRC_DIR)/%.c=$(DEP_DIR)/%.d)
CHKS = $(wildcard $(CHK_DIR)/*.check)
### Tools
CC   = gcc
LINT = clang-format
### Flag groups
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.d
STDFLAGS = -std=c11
DBGFLAGS = -g
OPTFLAGS =
WFLAGS   = -Wall -Werror -Wextra -Wpedantic
LINTFLAGS = --style=google
### Stage flags
CPPFLAGS = $(DEPFLAGS)
CFLAGS   = $(STDFLAGS) $(DBGFLAGS) $(OPTFlAGS) $(WFLAGS)
LDFLAGS  =
LDLIBS   =


all: $(PROGRAM)
.PHONY: all

$(PROGRAM): $(PROGRAM_EXE)
# Link the object files into one binary
$(PROGRAM_EXE): $(OBJS) | $(BIN_DIR)
	@echo \### Linking $@...
	$(CC) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)
# Compile the object files, generating dependency files in the process
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEP_DIR)/%.d | $(OBJ_DIR) $(DEP_DIR)
	@echo \### Compiling $<...
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ -c $<
# Handle missing .d files
$(DEPS):
# Include the dependency files that exist
include $(DEPS)
# Create the directories if they don't already exist
$(BIN_DIR) $(OBJ_DIR) $(DEP_DIR):
	@echo \### Creating $@...
	mkdir -p $@

test: | $(TST_BLD_DIR)
	checkmk $(CHKS) > $(BIN_DIR)/test.c
.PHONY: test

clean:
	$(RM) -r $(BLD_DIR)
.PHONY: clean

re: clean all
.PHONY: re

run: $(PROGRAM_EXE)
	$(PROGRAM_EXE)
.PHONY: run

container:
	bash test/run_container.sh
.PHONY: container

linter_check: LINTFLAGS := -n $(LINTFLAGS)
linter_fix:   LINTFLAGS := -i $(LINTFLAGS)
linter_check linter_fix:
	$(LINT) $(LINTFLAGS) $(shell find . -type f -name '*.h' -o -name '*.c')
.PHONY: linter_check linter_fix

echo:
	@echo $(CHKS)
check:
	gcc -g -Wall -Werror -Wextra -o build/check src/check.c
	leaks --atExit -- build/check | grep LEAK
.PHONY: check
