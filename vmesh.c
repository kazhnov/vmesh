#include "vmesh.h"
#include "external/vmath/vmath.h"
#include "stdlib.h"
#include "stdio.h"
#include <ctype.h>

struct Mesh {
    float *vertices;
};

Mesh *GMESH_LoadObj(char* path) {
    Mesh mesh  = malloc(sizeof(Mesh));
    FILE* file = fopen(path, "r");
    char c;
    while ((c = fgetc(file)) != EOF) {
	if (c == '#') {
	    while(c != EOF || c != '\n') {
		fgetc(file);
	    }
	    if (c == '\n') fgetc(file);
	} else if (c == 'v') {
	    float x, y, z;
	    fscanf(file, "v %f %f %f", &x, &y, &z);
	    float r, g, b, a;

	    if (fgetc(file) == ' ') {
		fscanf(file, "%f %f %f %f\n", &r, &g, &b, &a);
	    }

	    VM3_Copy
	    
	    
	}
	
    }
}
