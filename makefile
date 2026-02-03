build: vmesh.c
	mkdir -p build lib
	cc -c vmesh.c -fPIC -o build/vmesh.o
	cc build/vmesh.o -shared -o lib/libvmesh.so

test: test.c build
	cc test.c -o test -L ./lib -lvmesh
	LD_LIBRARY_PATH=./lib ./test
