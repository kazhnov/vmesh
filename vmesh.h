#include <stdint.h>
struct Mesh;
typedef struct Mesh Mesh;


float* VMESH_Vertices(Mesh* mesh);

uint32_t VMESH_FloatsCount(Mesh* mesh);

uint32_t VMESH_VertexCount(Mesh* mesh);

uint32_t* VMESH_Faces(Mesh* mesh);

uint32_t VMESH_IndicesCount(Mesh* mesh);

uint32_t VMESH_FacesCount(Mesh* mesh);


Mesh *VMESH_LoadObj(char* path);
