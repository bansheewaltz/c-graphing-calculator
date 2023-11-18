BUILD_DIR := build
INSTALL_DIR := installation

all: debug
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
	cmake -B $(BUILD_DIR)
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
	cmake --install build/debug --prefix $(INSTALL_DIR)
.PHONY: install

uninstall:
	cmake --build $(BUILD_DIR)/debug --target uninstall
#	xargs rm < build/debug/install_manifest.txt
.PHONY: uninstall

clean:
	$(RM) -r $(BUILD_DIR) $(INSTALL_DIR)
	$(RM) *.tar*
.PHONY: clean

dvi:
	cmake --build $(BUILD_DIR)/debug --target docs
.PHONY: dvi

dist:
	cpack --config $(BUILD_DIR)/debug/CPackSourceConfig.cmake
	rm -r _CPack_Packages
.PHONY: dist

bdist:
	cd $(BUILD_DIR)/debug && cpack -B ../..
	rm -r _CPack_Packages
.PHONY: bdist

test:
	cmake --build $(BUILD_DIR)/debug --target run_tests
.PHONY: test

gcov_report:
	cmake --build $(BUILD_DIR)/debug --target coverage
.PHONY: gcov_report
