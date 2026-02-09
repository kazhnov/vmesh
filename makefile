DEBUG = -g -O0 -fsanitize=address
RELEASE = -O3
MODE = $(DEBUG)

build: vmesh.c
	mkdir -p build lib
	cc -c vmesh.c -fPIC -o build/vmesh.o $(DEBUG)
	cc build/vmesh.o -shared -o lib/libvmesh.so $(DEBUG)

test: test.c build
	cc test.c -o test -L ./lib -lvmesh $(DEBUG)
	LD_LIBRARY_PATH=./lib ./test
