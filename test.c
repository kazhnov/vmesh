#include "vmesh.h"
#include <stdio.h>

int main() {    
    Mesh* mesh =VMESH_LoadObj("teapot_normals.obj");
    printf("vertices: %d, faces: %d\n", VMESH_VertexCount(mesh), VMESH_IndicesCount(mesh));
    printf("OK\n");
    return 0;
}
