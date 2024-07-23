BUILD_DIR ?= build
export GENERATOR ?= Unix Makefiles
export JOBS ?= 8
export MAKEFLAGS += --no-print-directory
export CMAKE_INSTALL_PREFIX ?= install
export CMAKE_BUILD_TYPE=Release

all: compile

install: compile
	cmake --install $(BUILD_DIR) 
	cp $(BUILD_DIR)/install_manifest.txt .

compile: build
	cmake --build $(BUILD_DIR) -j$(JOBS) 

build:
	cmake -S . -B $(BUILD_DIR) -G "$(GENERATOR)" \
		-DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX)

format:
	find ./src -regex '.*\.[c|h]pp' | xargs clang-format -i

clean:
	rm -rf $(BUILD_DIR)
	rm -f compile_commands.json

uninstall:
	-cat install_manifest.txt | xargs rm -f

.PHONY: all build install compile format clean uninstall

