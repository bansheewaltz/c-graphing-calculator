### Project name
PROGRAM = smartcalc
PROGRAM_EXE = $(BIN_DIR)/$(PROGRAM)
### Directories
SRC_DIR = src
BLD_DIR = build
BIN_DIR = $(BLD_DIR)/bin
OBJ_DIR = $(BLD_DIR)/obj
### Files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
HDR_FILES = $(SRC_DIR)/*.h
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
### Tools
CC   = gcc
LINT = clang-format
### Flags
CPPFLAGS =
STDFLAGS = -std=c11
DBGFLAGS = -g
OPTFLAGS =
LINTFLAG = --style=google
WFLAGS   = -Wall -Werror -Wextra -Wpedantic
WFLAG1   = 

CFLAGS  = $(CPPFLAGS) $(STDFLAGS) $(DBGFLAGS) $(OPTFlAGS) $(WFLAGS)
LDFLAGS =
LDLIBS  =


all: smartcalc

smartcalc: ${PROGRAM_EXE}
${PROGRAM_EXE}: ${OBJ_FILES} | ${BIN_DIR}
	${CC} -o $@ ${CFLAGS} ${OBJ_FILES} ${LDFLAGS} ${LDLIBS}
${OBJ_DIR}/%.o: ${SRC_DIR}/%.c $(HDR_FILES) | ${OBJ_DIR}
	${CC} ${CFLAGS} -c -o $@ $<
# Directories creation
${BIN_DIR}:
	mkdir -p $@
${OBJ_DIR}:
	mkdir -p $@

run: $(PROGRAM_EXE)
	$(PROGRAM_EXE)
PHONY: run

container:
	bash test/run_container.sh
PHONY: container

clean:
	$(RM) -r build
PHONY: clean

linter_fix:   LINTFLAGS := -i $(LINTFLAGS)
linter_check: LINTFLAGS := -n $(LINTFLAGS)
linter_check linter_fix:
	$(LINT) $(LINTFLAGS) $(shell find . -type f -name '*.h' -o -name '*.c')
.PHONY: linter_check linter_fix


check:
	gcc -g -Wall -Werror -Wextra -o build/check src/check.c
	leaks --atExit -- build/check | grep LEAK
