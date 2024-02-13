CMAKE_FLAGS := -GNinja -DBUILD_EXAMPLES=1

all: build

build: CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=Debug
build: cmake

cmake: build-folder
	@cd build && cmake $(CMAKE_FLAGS) .. --log-level=STATUS && ninja

build-folder:
	-@sh -c 'mkdir build 2>/dev/null || true'

clean:
	rm -rf build

.PHONY: cmake