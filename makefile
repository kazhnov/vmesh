build: vmesh.c
	mkdir -p build lib
	cc -c vmesh.c -fPIC -o build/vmesh.o -g
	cc build/vmesh.o -shared -o lib/libvmesh.so -g -O0 -fsanitize=address

test: test.c build
	cc test.c -o test -L ./lib -lvmesh -g -O0 -fsanitize=address
	LD_LIBRARY_PATH=./lib ./test
