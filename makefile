DEBUG = -O0 -g -fsanitize=address
RELEASE = -O3
MODE = $(DEBUG)

build: vmesh.c
	mkdir -p build lib
	cc -c vmesh.c -o build/vmesh.o $(MODE)
	ar rcs lib/libvmesh.a build/vmesh.o

test: test.c build
	cc test.c -o build/test -L./lib -lvmesh $(MODE) 
	build/test
