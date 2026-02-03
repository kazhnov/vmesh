#include <stdint.h>
struct Mesh;
typedef struct Mesh Mesh;

float* VMESH_Vertices(Mesh* mesh);
uint32_t VMESH_VerticesSize(Mesh* mesh);

uint32_t* VMESH_Indices(Mesh* mesh);
uint32_t VMESH_IndicesSize(Mesh* mesh);

Mesh *VMESH_LoadObj(char* path);
