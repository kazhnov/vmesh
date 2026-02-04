#include "vmesh.h"
#include <stdio.h>

int main() {    
    Mesh* mesh =VMESH_LoadObj("bunny.obj");
    Vertex *vertices = VMESH_Vertices(mesh);
    for(int i = 0; i < 100; i++) {
	printf("%f %f %f\n", vertices[i].pos[0], vertices[i].pos[1], vertices[i].pos[2]);
    }
    printf("OK\n");
    return 0;
}
