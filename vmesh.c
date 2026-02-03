#include "vmesh.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include <ctype.h>

struct Mesh {
    float *vertices;
    uint32_t *faces;
    uint32_t index_count;
    uint32_t floats_count;
    uint32_t vertex_size;
};

float* VMESH_Vertices(Mesh* mesh) {
    return mesh->vertices;
}

uint32_t VMESH_FloatsCount(Mesh* mesh) {
    return mesh->floats_count;
}

uint32_t VMESH_VertexCount(Mesh* mesh) {
    return mesh->floats_count/3;
}

uint32_t* VMESH_Faces(Mesh* mesh) {
    return mesh->faces;
}

uint32_t VMESH_IndicesCount(Mesh* mesh) {
    return mesh->index_count;
}

uint32_t VMESH_FacesCount(Mesh* mesh) {
    return mesh->index_count/3;
}

Mesh *VMESH_LoadObj(char* path) {
    Mesh *mesh  = malloc(sizeof(Mesh));
    mesh->vertex_size = 3+4;
    FILE* file = fopen(path, "r");
    char c;
    float *vertex;
    uint32_t *index;
    while ((c = fgetc(file)) != EOF) {
	if (c == '#') {
	    fgetc(file);
	    c = fgetc(file);
	    if (c == 'O') {
		fscanf(file, "BJ file format with ext .obj\n");
	       
	    } else if (c == 'v') {
		int floats;
		fscanf(file, "ertex count = %d\n", &floats);
		floats *= mesh->vertex_size;
		mesh->vertices = malloc(sizeof(float)*floats);
		mesh->floats_count = floats;
		vertex = mesh->vertices;
	    } else if (c == 'f') {
		int indices;
		fscanf(file, "ace count = %d\n", &indices);
		indices *= 3;
		mesh->faces = malloc(sizeof(uint32_t)*indices);
		mesh->index_count = indices;
		index = mesh->faces;
	    }
	    
	    if (c == '\n') fgetc(file);
	} else if (c == 'v') {
	    float x, y, z;
	    fscanf(file, " %f %f %f", &x, &y, &z);
	    float r, g, b, a;
	    
	    if (fgetc(file) == ' ') {
		fscanf(file, "%f %f %f %f\n", &r, &g, &b, &a);
	    }
	    
	    printf("%f, %f, %f %f %f %f %f\n", x, y, z, r, g, b, a);
	    
	    *vertex++ = x;
	    *vertex++ = y;
	    *vertex++ = z;
	    *vertex++ = r;
	    *vertex++ = g;
	    *vertex++ = b;
	    *vertex++ = a;
	} else if (c == 'f') {
	    uint32_t x, y, z;
	    fscanf(file, " %d %d %d\n", &x, &y, &z);
	    x -= 1;
	    y -= 1;
	    z -= 1;
	    printf("%d, %d, %d\n", x, y, z);

	    *index++ = x;
	    *index++ = y;
	    *index++ = z;
	}	
    }

    return mesh;
}
