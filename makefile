build: vmesh.c
	mkdir -p build lib
	cc -c vmesh.c -fPIC -o build/vmesh.o
	cc build/vmesh.o -shared -o lib/vmesh.so
