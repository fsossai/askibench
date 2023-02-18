compile:
	git submodule update --init
	mkdir -p build
	cmake -S . -B build
	cmake --build build

clean:
	rm -rf build

.PHONY: compile clean

