#include <stdint.h>
struct Mesh;
typedef struct Mesh Mesh;

float* VMESH_VerticesGet(Mesh* mesh);

uint32_t* VMESH_IndicesGet(Mesh* mesh);

Mesh *VMESH_LoadObj(char* path);
