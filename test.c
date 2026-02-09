#include "vmesh.h"
#include <stdio.h>

int main() {
    Mesh* mesh = VMESH_New();
    VMESH_LoadObj(mesh, "../vmesh/teapot_normals.obj");
    Vertex *vertices = mesh->vertices;
    uint32_t *index = mesh->indices;
    for(int i = 0; i < 100; i++) {
	int j = index[i];
	printf("%d: %f %f %f : %f %f %f\n", j,
	       vertices[j].pos[0], vertices[j].pos[1], vertices[j].pos[2],
	       vertices[j].normal[0], vertices[j].normal[1], vertices[j].normal[2]);
    }
    printf("OK\n");
    VMESH_Destroy(mesh);
    return 0;
}
