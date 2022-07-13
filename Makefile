VANITY_CC ?= clang
VANITY_CXX ?= clang++
VANITY_GENERATOR ?= Ninja
VANITY_BUILD_TYPE ?= Debug
VANITY_BUILD_TESTS ?= OFF

export CC = $(VANITY_CC)
export CXX = $(VANITY_CXX)

BUILD_DIR = "build"

.PHONY: configure
configure:
	cmake -S . -B $(BUILD_DIR) -G "$(VANITY_GENERATOR)" \
		-D CMAKE_BUILD_TYPE="$(VANITY_BUILD_TYPE)" \
		-D CMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-D VANITY_BUILD_TESTS="$(VANITY_BUILD_TESTS)"

.PHONY: build
build: configure
	cmake --build $(BUILD_DIR) --config "$(VANITY_BUILD_TYPE)" --parallel

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: test
test: VANITY_BUILD_TESTS = ON
test: build
	ctest --output-on-failure --test-dir $(BUILD_DIR) -C "$(VANITY_BUILD_TYPE)"

.PHONY: patches
patches:
	- ./scripts/apply-patches.sh
	./scripts/make-patches.sh

.PHONY: submodules
submodules:
	git submodule update --init --recursive --jobs 4
	git pull --recurse-submodules --jobs 4

.PHONY: update-submodules
update-submodules: reset-submodules
	git submodule foreach git pull

.PHONY: reset-submodules
reset-submodules:
	git submodule foreach --recursive git reset --hard

.PHONY: clang-format
clang-format:
	git ls-files -- '*.cpp' '*.h' | xargs clang-format -i -style=file
	git diff --exit-code --color
