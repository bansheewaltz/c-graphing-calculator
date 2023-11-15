BUILD_DIR := build


all: build
.PHONY: all

build:
	cmake -B $(BUILD_DIR) -Wno-dev -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR)
.PHONY: build

buildd:
	cmake -B $(BUILD_DIR) -Wno-dev -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR)
.PHONY: buildd

re: clean buildd
.PHONY: re

# verbose build
buildv: config
	VERBOSE=1 cmake --build $(BUILD_DIR)
.PHONY: buildv

LINTFLAGS := --style=google
lint:     LINTFLAGS += -n
lint_fix: LINTFLAGS += -i
lint lint_fix:
	clang-format ${LINTFLAGS} $(shell find src -type f \
                                             -name '*.h'   -or -name '*.c' \
                                             -name '*.hpp' -or -name '*.cpp')
.PHONY: lint lint_fix

install:
.PHONY: install

uninstall:
.PHONY: uninstall

clean:
	$(RM) -r build bin
.PHONY: clean

dvi:
.PHONY: dvi

dist:
.PHONY: dist

test:
	@cd build && ctest
.PHONY: test

gcov_report:
.PHONY: gcov_report
