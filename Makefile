.PHONY: all clean devel devel-nix nix run

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

run:
	nix-shell --command 'make && ./build/mousewarp'

build/Makefile:
	mkdir -p build
	cd build && cmake ..
