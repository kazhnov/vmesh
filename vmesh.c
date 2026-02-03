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
    return mesh->floats_count/mesh->vertex_size;
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

void iVMESH_CountVerticesAndFaces(char* path, uint32_t* vertices, uint32_t *faces) {
    FILE* file = fopen(path, "r");
    char c;
    *faces = 0;
    *vertices = 0;
    
    while ((c = fgetc(file)) != EOF) {
	if (c == '#');
	if (c == 'f') *faces += 1;
	if (c == 'v') *vertices += 1;
	while (c!= '\n' && c != EOF) {
	    c = fgetc(file);
	}
	if (c == EOF) break;
    }
    fclose(file);
}

Mesh *VMESH_LoadObj(char* path) {
    Mesh *mesh  = malloc(sizeof(Mesh));
    mesh->vertex_size = 3+4;
    FILE* file = fopen(path, "r");
    char c;
    float *vertex;
    uint32_t *index;
    uint32_t vertex_count, face_count;
    iVMESH_CountVerticesAndFaces(path, &vertex_count, &face_count);
    printf("vertices: %d, faces: %d\n", vertex_count, face_count);
    
    uint32_t floats_count = mesh->vertex_size * vertex_count;
    mesh->vertices = malloc(sizeof(float)*floats_count);
    mesh->floats_count = floats_count;

    
    uint32_t index_count = face_count*3;
    mesh->faces = malloc(sizeof(uint32_t)*index_count);
    mesh->index_count = index_count;

    vertex = mesh->vertices;    
    index = mesh->faces;
    while ((c = fgetc(file)) != EOF) {
	if (c == '#') {
	    while (c != '\n' && c != EOF) c = fgetc(file);
	} else if (c == 'v') {
	    float x, y, z;
	    fscanf(file, " %f %f %f", &x, &y, &z);
	    float r, g, b, a;
	    a = 1.0f;
	    fgetc(file);
	    /*
	    if (fgetc(file) == ' ') {
		fscanf(file, "%f %f %f %f\n", &r, &g, &b, &a);
	    }
	    */
	    
//	    printf("%f, %f, %f\n", x, y, z);
	    
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
//	    printf("%d, %d, %d\n", x, y, z);

	    *index++ = x;
	    *index++ = y;
	    *index++ = z;
	}	
    }

    fclose(file);
    return mesh;
}

void VMESH_Destroy(Mesh* mesh) {
    free(mesh->faces);
    free(mesh->vertices);
    free(mesh);
}
