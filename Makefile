CMAKE_FLAGS := -GNinja

all: examples

run: examples
	@cd build && ./examples/$(PROG)

examples: CMAKE_FLAGS += -DBUILD_EXAMPLES=1
examples: build

build: CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=Debug
build: cmake

cmake: build-folder deps
	@cd build && cmake $(CMAKE_FLAGS) .. --log-level=STATUS && ninja

build-folder:
	-@sh -c 'mkdir build 2>/dev/null || true'

deps: lib/eigen/.phony lib/raylib/.phony

lib/eigen/.phony: lib/eigen
	@touch lib/eigen/.phony

lib/eigen: lib
	-@git clone https://gitlab.com/libeigen/eigen.git --branch 3.4.0 --depth 1 lib/eigen

lib/raylib/.phony: lib/raylib
	@touch lib/raylib/.phony

lib/raylib: lib
	-@git clone https://github.com/raysan5/raylib.git --branch 5.0 --depth 1 lib/raylib

lib:
	-@sh -c 'mkdir lib 2>/dev/null || true'

clean:
	rm -rf build/CMakeFiles
	rm -rf build/CMakeCache.txt

superclean:
	rm -rf build

cleandeps:
	rm -rf lib


.PHONY: cmake
.PHONY: deps