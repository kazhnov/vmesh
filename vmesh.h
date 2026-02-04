#include <stdint.h>
struct Mesh;
typedef struct Mesh Mesh;

typedef struct {
    float pos[3];
    float normal[3];
    float tex[2];
} Vertex;

Vertex* VMESH_Vertices(Mesh* mesh);

uint32_t VMESH_VertexCount(Mesh* mesh);

uint32_t VMESH_IndicesCount(Mesh* mesh);

uint32_t *VMESH_Indices(Mesh* mesh);

Mesh *VMESH_LoadObj(char* path);

void VMESH_Destroy(Mesh* mesh);
