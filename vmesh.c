#include "vmesh.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include <ctype.h>

struct Mesh {
    float *vertices;
    uint32_t *indices;
};

float* VMESH_VerticesGet(Mesh* mesh) {
    return mesh->vertices;
}

uint32_t* VMESH_IndicesGet(Mesh* mesh) {
    return mesh->indices;
}

Mesh *VMESH_LoadObj(char* path) {
    Mesh *mesh  = malloc(sizeof(Mesh));
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
		int vertices;
		fscanf(file, "ertex count = %d\n", &vertices);
		mesh->vertices = malloc(sizeof(float)*vertices);
		vertex = mesh->vertices;
	    } else if (c == 'f') {
		int indices;
		fscanf(file, "ace count = %d\n", &indices);
		mesh->indices = malloc(sizeof(uint32_t)*indices);
		index = mesh->indices;
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
