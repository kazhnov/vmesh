#include "vmesh.h"
#include <stdio.h>

int main() {    
    Mesh* mesh =VMESH_LoadObj("bunny.obj");
    printf("vertices: %d, faces: %d\n", VMESH_VertexCount(mesh), VMESH_IndicesCount(mesh));
    float* normals = VMESH_Normals(mesh);
    for(int i = 0; i < 100; i++) {
	printf("%f %f %f\n", normals[i*3+0], normals[i*3+1], normals[i*3+2]);
    }
    printf("OK\n");
    return 0;
}
