/* Pyarelal Knowles 2012 - updated 29/03/2012 19:15 */

#ifndef SIMPLE_OBJ_H
#define SIMPLE_OBJ_H

#ifdef __cplusplus
extern "C" {
#endif

//ignores the "s" directive in the obj file - creates less facesets
#define OBJ_IGNORE_SMOOTHING 1

//prints warnings and sometimes line numbers
#define OBJ_PRINT_DEBUG 1

//groups unique vertex/attribs (using uthash)
//makes arrays smaller and rendering faster at the cost of load time
#define OBJ_INDEX_VERTICES 1

//fills the faceset and materials array with colour/texture names etc
#define OBJ_ENABLE_MATERIALS 1

//obj files should not have lines longer than this
#define OBJ_MAX_LINE_LEN 1024

//polygons will be capped at 12 vertices
#define OBJ_MAX_POLYGON 12

/*
The material stores renderign attributes such
as colour and texture name. Materials are instanced
with facesets.
*/
typedef struct _OBJMaterial
{
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;
	float transparency;
	int mode;
	char* name;
	char* texture;
	char* texNormal;
	char* texSpecular;
} OBJMaterial;

/*
FaceSets are used to specify which faces
materials are applied to. This way, the
material does not have to be duplicated each
time it is used.

material is an index into the Mesh's materials

indexStart is the start of the faceset and,
like indexEnd must be a multiple of 3 (as
the Mesh is a triangle mesh)

if smooth is nonzero, smooth shading should be applied

if materials aren't used but smooth shading
is toggled, material may be -1
*/
typedef struct _OBJFaceSet
{
	int material;
	int indexStart;
	int indexEnd;
	int smooth;
} OBJFaceSet;

/*
The Mesh contains all materials, facesets and mesh
data. Neither materials or facesets can be shared
across meshes.

vertices contains all vertex data including normals
and texture coordinates (if they exist)

indices index the vertex data, giving triangles
(every 3 indices forms a triangle).

vertex data must include position (first 3 floats)

if hasNormals is nonzero the vertex data includes
normals (3 floats after position)

if hasTexCoords is nonzero the vertex data includes
texture coordinates (2 floats after normal, or
position if there is no normal)

normalOffset, texcoordOffset and
stride are all given in bytes.
*/
typedef struct _OBJMesh
{
	float* vertices;
	unsigned int* indices;
	OBJMaterial* materials;
	OBJFaceSet* facesets;
	int numVertices;
	int numIndices;
	int numMaterials;
	int numFacesets;
	int hasNormals;
	int hasTexCoords;
	int normalOffset;
	int texcoordOffset;
	int stride;
} OBJMesh;

/*
use these functions to load and free an obj mesh
*/
OBJMesh* objMeshLoad(const char* filename);
void objMeshFree(OBJMesh** mesh);

#ifdef __cplusplus
} /* end extern "C" */
#endif

#endif

