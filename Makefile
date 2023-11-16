BUILD_DIR := build


all:
	cmake -B $(BUILD_DIR)/release -Wno-dev -DCMAKE_BUILD_TYPE=Release \
                                         -DLIBSMARTCALC_TESTING \
                                         -DLIBSMARTCALC_COVERAGE \
                                         -DLIBSMARTCALC_DOCS
	cmake --build $(BUILD_DIR)/release
.PHONY: all

release:
	cmake -B $(BUILD_DIR)/release -Wno-dev -DCMAKE_BUILD_TYPE=Release
	cmake --build $(BUILD_DIR)/release
.PHONY: release

debug:
	cmake -B $(BUILD_DIR)/debug -Wno-dev -DCMAKE_BUILD_TYPE=Debug
	cmake --build $(BUILD_DIR)/debug
.PHONY: debug

# verbose build
buildv:
	cmake -B build
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
	$(RM) -r build
.PHONY: clean

dvi:
	cmake --build $(BUILD_DIR)/debug --target docs
.PHONY: dvi

dist:
.PHONY: dist

test:
	cmake --build $(BUILD_DIR)/debug --target test
.PHONY: test

gcov_report:
	cmake --build $(BUILD_DIR)/debug --target coverage
.PHONY: gcov_report
