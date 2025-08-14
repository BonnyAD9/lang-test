TARGET=primes
BUILD_TYPE?=Release
PARALLEL?=-j $(shell nproc)
SOURCES=$(shell find src/ -name '*.c')
HEADERS=$(shell find src/ -name '*.h')
FILES=$(SOURCES) $(HEADERS)
BUILD_FOLDER=target/$(BUILD_TYPE)

.PHONY: build
build:
	if [ ! -f $(BUILD_FOLDER)/Makefile ]; then \
		mkdir -p $(BUILD_FOLDER); \
		cd $(BUILD_FOLDER) \
			&& cmake ../.. -DCMAKE_BUILD_TYPE=$(BUILD_TYPE); \
	fi
	cd $(BUILD_FOLDER) && $(MAKE) $(PARALLEL)

.PHONY: debug
debug:
	$(MAKE) BUILD_TYPE=Debug

.PHONY: release
release:
	$(MAKE) BUILD_TYPE=Release

.PHONY: fmt
fmt:
	clang-format -i $(FILES)

.PHONY: cppcheck
cppcheck:
	cppcheck --check-level=exhaustive $(FILES)

.PHONY: tidy
tidy: debug
	run-clang-tidy $(PARALLEL) -use-color -quiet -p $(BUILD_FOLDER) \
		-header-filter=src/ 'src/.*\.c' 'src/.*\.h'

.PHONY: check
check: fmt cppcheck tidy

.PHONY: clean
clean:
	-rm -rf target
