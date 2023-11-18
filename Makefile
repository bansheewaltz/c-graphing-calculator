BUILD_TYPE ?= debug
BUILD_DIR := build/$(BUILD_TYPE)
INSTALL_DIR := installation

all: build
.PHONY: all

configure:
	cmake -B $(BUILD_DIR) -Wno-dev -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

debug:   BUILD_TYPE = debug
release: BUILD_TYPE = release
build release debug: configure	
	cmake --build $(BUILD_DIR)
.PHONY: build release debug

# verbose build
buildv:
	cmake -B $(BUILD_DIR) -Wno-dev -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)
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
	cmake --install $(BUILD_DIR) --prefix $(INSTALL_DIR)
.PHONY: install

uninstall:
	cmake --build $(BUILD_DIR) --target uninstall
.PHONY: uninstall

clean:
	$(RM) -r $(BUILD_DIR) $(INSTALL_DIR)
	$(RM) *.tar* *.sh
.PHONY: clean

dvi:
	cmake --build $(BUILD_DIR) --target docs
.PHONY: dvi

dist:
	cpack --config $(BUILD_DIR)/CPackSourceConfig.cmake
	rm -r _CPack_Packages
.PHONY: dist

bdist:
	cpack --config $(BUILD_DIR)/CPackConfig.cmake
	rm -r _CPack_Packages
.PHONY: bdist

test:
	cmake --build $(BUILD_DIR) --target run_tests
.PHONY: test

gcov_report:
	cmake --build $(BUILD_DIR) --target coverage
.PHONY: gcov_report
