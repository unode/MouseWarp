.PHONY: clean devel devel-nix all

all: build/Makefile
	cd build && make

nix:
	nix-shell --command make

devel: build/Makefile
	cd build && make devel

devel-nix:
	nix-shell --command 'make devel'

clean:
	rm -rf build

build/Makefile:
	mkdir -p build
	cd build && cmake ..
