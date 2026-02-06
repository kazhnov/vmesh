#include <stdint.h>
struct Mesh;
typedef struct Mesh Mesh;

typedef struct {
    float pos[3];
    float normal[3];
    float tex[2];
} Vertex;

struct Mesh {
    Vertex *vertices;
    uint32_t *indices;
    uint32_t index_count;
    uint32_t vertex_count;
};

/* Vertex* VMESH_Vertices(Mesh* mesh); */

/* uint32_t VMESH_VertexCount(Mesh* mesh); */

/* uint32_t VMESH_IndicesCount(Mesh* mesh); */

/* uint32_t *VMESH_Indices(Mesh* mesh); */


Mesh* VMESH_New();

void VMESH_LoadObj(Mesh* mesh, char* path);

void VMESH_Destroy(Mesh* mesh);
