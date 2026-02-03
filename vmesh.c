#include "vmesh.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include <ctype.h>

struct Mesh {
    float *vertices;
    float *normals;
    uint32_t *faces;
    uint32_t index_count;
    uint32_t floats_count;
    uint32_t vertex_size;
    bool has_normals;
};

float* VMESH_Vertices(Mesh* mesh) {
    return mesh->vertices;
}

uint32_t VMESH_FloatsCount(Mesh* mesh) {
    return mesh->floats_count;
}

bool VMESH_HasNormals(Mesh* mesh) {
    return mesh->has_normals;
}

float* VMESH_Normals(Mesh* mesh) {
    return mesh->normals;
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

void iVMESH_CountVerticesAndFaces(char* path, uint32_t* vertices, uint32_t *faces, uint32_t *normals) {
    FILE* file = fopen(path, "r");
    char c;
    *faces = 0;
    *vertices = 0;
    *normals = 0;
    
    while ((c = fgetc(file)) != EOF) {
	if (c == '#');
	if (c == 'f') *faces += 1;
	if (c == 'v') {
	    c = fgetc(file);
	    if (c == 'n') *normals += 1;
	    else *vertices += 1;
	}
	while (c!= '\n' && c != EOF) {
	    c = fgetc(file);
	}
	if (c == EOF) break;
    }
    fclose(file);
}

Mesh *VMESH_LoadObj(char* path) {
    Mesh *mesh  = malloc(sizeof(Mesh));
    mesh->vertex_size = 3;
    FILE* file = fopen(path, "r");
    char c;
    uint32_t *index;
    uint32_t vertex_count, face_count, normal_count;
    iVMESH_CountVerticesAndFaces(path, &vertex_count, &face_count, &normal_count);
    printf("vertices: %d, faces: %d, normals: %d\n", vertex_count, face_count, normal_count);
    uint32_t floats_count = mesh->vertex_size * vertex_count;
    mesh->has_normals = 0;
    if (normal_count != 0) {
	mesh->has_normals = 1;
	if (vertex_count != normal_count) {
	    printf("vertex count and normal count are not the same!\n");
	    return NULL;
	}
	mesh->normals = calloc(sizeof(float), floats_count);
    }

    mesh->vertices = calloc(sizeof(float),floats_count);
    mesh->floats_count = floats_count;
    
    
    
    uint32_t index_count = face_count*3;
    mesh->faces = calloc(sizeof(uint32_t), index_count);
    mesh->index_count = index_count;
    
    uint32_t vertex = 0;
    uint32_t normal = 0;
    index = mesh->faces;
    while ((c = fgetc(file)) != EOF) {
	if (c == '#') {
	    while (c != '\n' && c != EOF) c = fgetc(file);
	} else if (c == 'v') {
	    c = fgetc(file);
	    if (c == 'n') {
		float x, y, z, a;
		fscanf(file, "%f %f %f", &x, &y, &z);
//		printf("normal: %f, %f, %f\n", x, y, z);
		a = 1.0;
		fgetc(file);
		
		mesh->normals[normal*mesh->vertex_size+0] = x;
		mesh->normals[normal*mesh->vertex_size+1] = y;
		mesh->normals[normal*mesh->vertex_size+2] = z;
		mesh->normals[normal*mesh->vertex_size+3] = a;
		
		normal++;
	    } else {
		float x, y, z;
		fscanf(file, "%f %f %f", &x, &y, &z);
		fgetc(file);
		mesh->vertices[vertex*mesh->vertex_size+0] = x;
		mesh->vertices[vertex*mesh->vertex_size+1] = y;
		mesh->vertices[vertex*mesh->vertex_size+2] = z;
		vertex++;
	    }
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
