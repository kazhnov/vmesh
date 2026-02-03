#include <stdint.h>
struct Mesh;
typedef struct Mesh Mesh;


float* VMESH_Vertices(Mesh* mesh);

uint32_t VMESH_FloatsCount(Mesh* mesh);

uint32_t VMESH_VertexCount(Mesh* mesh);

uint32_t* VMESH_Faces(Mesh* mesh);

uint32_t VMESH_IndicesSize(Mesh* mesh);

uint32_t VMESH_FacesCound(Mesh* mesh);


Mesh *VMESH_LoadObj(char* path);
