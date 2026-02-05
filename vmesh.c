#include "vmesh.h"
#include "stdlib.h"
#include "stdint.h"
#include "stdio.h"
#include <ctype.h>
#include <assert.h>
#include <string.h>


#define INDEX_SIZE (4)
#define KEY_SIZE = (INDEX_SIZE*INDEX_SIZE*INDEX_SIZE)
#define LOOKUP_TABLE_SIZE (KEY_SIZE*INDEX_SIZE);

struct Mesh {
    Vertex *vertices;
    uint32_t *indices;
    uint32_t index_count;
    uint32_t vertex_count;
};

Vertex* VMESH_Vertices(Mesh* mesh) {
    return mesh->vertices;
}

uint32_t VMESH_VertexCount(Mesh* mesh) {
    return mesh->vertex_count;
}

uint32_t VMESH_IndicesCount(Mesh* mesh) {
    return mesh->index_count;
}

uint32_t* VMESH_Indices(Mesh *mesh) {
    return mesh->indices;
}

void iVMESH_CountVNTF(FILE* file, uint32_t* vert_count, uint32_t *face_count, uint32_t *uv_count, uint32_t *normal_count) {
    rewind(file);
    char c;
    *face_count   = 0;
    *vert_count   = 0;
    *uv_count     = 0;
    *normal_count = 0;
    
    while ((c = fgetc(file)) != EOF) {
	if (c == '#') {
	    while (c != '\n') c = fgetc(file);
	}
	if (c == 'f') *face_count += 1;
	if (c == 'v') {
	    c = fgetc(file);
	    if (c == 'n') *normal_count += 1;
	    else if (c == 't') *uv_count += 1;
	    else *vert_count += 1;
	}
	while (c!= '\n' && c != EOF) {
	    c = fgetc(file);
	}
	if (c == EOF) break;
    }
}


typedef struct {
    uint32_t pos;
    uint32_t normal;
    uint32_t tex;
} Index;

Index* iVMESH_ParseObjIndices(FILE *file, uint32_t face_count,
			      float **positions, uint32_t vcount,
			      float** normals, uint32_t ncount,
			      float** uvs, uint32_t tcount
    ) {
    rewind(file);
    Index* indices = calloc(face_count*3, sizeof(Index));
    printf("Getting Data.. ");
    fflush(stdout);
    
    *positions = calloc(vcount*3, sizeof(float));
    if (tcount) {
	*uvs = calloc(tcount*2, sizeof(float));
    }
    *normals = calloc(ncount*3, sizeof(float));
    
    uint32_t cur_index = 0, cur_pos = 0, cur_normal = 0, cur_uv = 0;
    char c = 1;
    while (c != EOF) {
	c = fgetc(file);
	if (c == 'v') {
	    c = fgetc(file);
	    if (c == ' ') { // v %f %f %f
		float x, y, z;
		fscanf(file, "%f %f %f\n", &x, &y, &z);
		(*positions)[cur_pos++] = x;
		(*positions)[cur_pos++] = y;
		(*positions)[cur_pos++] = z;
	    } else if (c == 'n') { // vn %f %f %f
		float x, y, z;
		fscanf(file, " %f %f %f\n", &x, &y, &z);
		(*normals)[cur_normal++] = x;
		(*normals)[cur_normal++] = y;
		(*normals)[cur_normal++] = z;
	    } else if (c == 't') { // vt %f %f %f
		float x, y;
		fscanf(file, " %f %f\n", &x, &y);
		(*uvs)[cur_uv++] = x;
		(*uvs)[cur_uv++] = y;
	    }
	}
	
	if (c != 'f') continue;

	for (int i = 0; i < 3; i++) {
	    
	    uint32_t v = 0, vt = 0, vn = 0;
	    fscanf(file, "%d", &v);
	    c = getc(file);
	    assert(c == '/');
	    
	    c = getc(file); // '/' or '%d'
	    if (c != '/') {
		ungetc(c, file);
		fscanf(file, "%d/", &vt);
	    }
	    
	    fscanf(file, "%d", &vn);
	    indices[cur_index].pos = v;
	    indices[cur_index].normal = vn;
	    indices[cur_index].tex = vt;
	    cur_index++;
	    
	    c = getc(file);
	    assert(c == ' ' || c == '\n');
	}
    }
    printf("Done\n");
    return indices;
}

uint32_t iVMESH_TableFind(Index* table, Index* index, uint32_t face_count) {
    for(int i = 0; i < face_count*3; i++) {
	if (table[i].normal == 0xFCFCFCFC &&
	    table[i].pos == 0xFCFCFCFC &&
	    table[i].tex == 0xFCFCFCFC) {
	    
	    table[i].normal = index->normal;
	    table[i].pos = index->pos;
	    table[i].tex = index->tex;
	    
	    return i;
	}

	if (table[i].normal == index->normal &&
	    table[i].pos == index->pos &&
	    table[i].tex == index->tex) {
	    return i;
	}
    }
    assert("How the hell");
}

Mesh* VMESH_LoadObj(char* path) {
    printf("Loading model %s..\n", path);
    FILE* file = fopen(path, "r");
    uint32_t vert_count, face_count, uv_count, normal_count;
    iVMESH_CountVNTF(file, &vert_count, &face_count, &uv_count, &normal_count);

    float* positions, *normals, *uvs;
    
    Index* indices = iVMESH_ParseObjIndices(file, face_count,
					    &positions, vert_count,
					    &normals, normal_count,
					    &uvs, uv_count
	); // get indices
    Index* lookup_table = malloc(face_count*3 * sizeof(Index));
    memset(lookup_table, 0xFC, face_count*3*sizeof(Index));
    uint32_t* mapped_indices = malloc(face_count*3 * sizeof(uint32_t));
    memset(mapped_indices, 0xFC, face_count*3*sizeof(uint32_t));
    
    for(int i = 0; i < face_count*3; i++) {
	mapped_indices[i] = iVMESH_TableFind(lookup_table,
					   indices+i,
					   face_count);
	// real indices map from indices triplets to just vertices
    }

    uint32_t out_vertex_count;
    for(out_vertex_count = 0; out_vertex_count < face_count*3; out_vertex_count++) {
	if (lookup_table[out_vertex_count].pos == 0xFCFCFCFC) {
	    break;
	}
    }

    uint32_t table_len;

    uint32_t *mesh_indices = malloc(face_count*3 * sizeof(uint32_t));
    for (int i = 0; i < face_count*3; i++) {
	mesh_indices[i] = mapped_indices[i];
    }

    Vertex* vertices = calloc(table_len, sizeof(Vertex));
    printf("Recalculating Vertices..");
    fflush(stdout);
    for(int i = 0; i < face_count*3; i++) {
	uint32_t j = mesh_indices[i];
	uint32_t p = lookup_table[j].pos - 1;
	uint32_t n = lookup_table[j].normal - 1;
	uint32_t t = lookup_table[j].tex - 1;
//	printf("%d maps to %d with position %d\n", i, j, p);

	vertices[j].pos[0] = positions[3*p+0];
	vertices[j].pos[1] = positions[3*p+1];
	vertices[j].pos[2] = positions[3*p+2];

	vertices[j].normal[0] = normals[3*n+0];
	vertices[j].normal[1] = normals[3*n+1];
	vertices[j].normal[2] = normals[3*n+2];
	if (uv_count) {
	    vertices[j].tex[0] = uvs[t*2+0];
	    vertices[j].tex[1] = uvs[t*2+1];
	}
    }
    printf(" Done\n");
    
    
    Mesh *mesh = calloc(1, sizeof(Mesh));
    mesh->vertices = vertices;
    mesh->indices = mesh_indices;
    mesh->vertex_count = out_vertex_count;
    mesh->index_count = face_count*3;

    printf("Tidying Up.. ");
    fflush(stdout);
    free(mapped_indices);	
    free(lookup_table);
    free(indices);
    free(positions);
    if (normal_count) {
	free(normals);
    } if (uv_count) {
	free(uvs);
    }
    fclose(file);
    printf("Done\n");
    printf("Loaded model %s\n", path);
    return mesh;
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

void VMESH_Destroy(Mesh* mesh) {
    free(mesh->indices);
    free(mesh->vertices);
    free(mesh);
}
