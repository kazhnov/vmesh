DEBUG = -g -O0 -fsanitize=address
RELEASE = -O3
MODE = $(DEBUG)

build: vmesh.c
	mkdir -p build lib
	cc -c vmesh.c -fPIC -o build/vmesh.o $(DEBUG)
	ar rcs lib/libvmesh.a build/vmesh.o

test: test.c build
	cc test.c -o test -L ./lib -lvmesh $(DEBUG)
	./test
