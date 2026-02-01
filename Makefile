.PHONY: all build shared examples examples-shared examples-debug test clean 

BUILD_DIR := build
CMAKE_FLAGS :=

all: build

# static library
build: 
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Release \
		$(CMAKE_FLAGS) \
		.
	@cmake --build $(BUILD_DIR)
	@echo "Static library built: $(BUILD_DIR)/libhandrolled.a"

# shared library
shared:
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_SHARED_LIBS=ON \
		$(CMAKE_FLAGS) \
		.
	@cmake --build $(BUILD_DIR)
	@echo "Shared library built: $(BUILD_DIR)/libhandrolled.so"

# build example executables with static library
examples:
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_EXAMPLES=ON \
		$(CMAKE_FLAGS) \
		.
	@cmake --build $(BUILD_DIR)
	@echo "Examples built with static library"

# build examples executables with shared library
examples-shared:
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_EXAMPLES=ON \
		-DBUILD_SHARED_LIBS=ON \
		$(CMAKE_FLAGS) \
		.
	@cmake --build $(BUILD_DIR)
	@echo "Examples built with shared library"

# build example executables in debug mode with static library
examples-debug:
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) \
		-DBUILD_EXAMPLES=ON \
		-DCMAKE_BUILD_TYPE=Debug \
		$(CMAKE_FLAGS) \
		.
	@cmake --build $(BUILD_DIR)
	@echo "Examples built with debug static library"

# build and run tests
test:
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) \
		-DCMAKE_BUILD_TYPE=Debug \
		-DBUILD_TESTS=ON \
		-DCMAKE_C_FLAGS="-Wno-unused-parameter" \
		$(CMAKE_FLAGS) \
		.
	@cmake --build $(BUILD_DIR)
	@cd $(BUILD_DIR) && ctest --verbose

clean:
	@rm -rf $(BUILD_DIR)
	@echo "Removed build directory: build/"
