CMAKE_FLAGS := -GNinja

all: examples

run: examples
	@cd build && ./examples/$(PROG)

examples: CMAKE_FLAGS += -DBUILD_EXAMPLES=1
examples: build

build: CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=Debug
build: cmake

cmake: build-folder
	@cd build && cmake $(CMAKE_FLAGS) .. --log-level=STATUS && ninja

build-folder:
	-@sh -c 'mkdir build 2>/dev/null || true'

clean:
	rm -rf build/CMakeFiles
	rm -rf build/CMakeCache.txt

superclean:
	rm -rf build

.PHONY: cmake