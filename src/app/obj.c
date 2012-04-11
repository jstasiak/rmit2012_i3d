/* Pyarelal Knowles 2012 - updated 29/03/2012 19:15 */

#include "obj.h"
#include "uthash.h"

#include <assert.h>
#include <stdio.h>
#include <errno.h>

#ifdef _WIN32
#define strtof strtod
#pragma warning(disable: 4244) //loss of data, eg conversion double to float
#pragma warning(disable: 4996) //deprecated functions, eg fopen
#endif

//a small structure for dynamic reallocation
//similar to std::vector
typedef struct _ReallocArray
{
	//the user edits these
	void* data; //should typecast to data type
	int size; //size is in blocks of blockSize bytes
	
	//these are for private use
	int reserved; //size in blockSize bytes
	int blockSize;
} ReallocArray;

//init the ReallocArray, zero size and one reserved data block
void initArray(ReallocArray* array, int blockSize)
{
	array->data = malloc(blockSize);
	array->size = 0;
	array->reserved = 1;
	array->blockSize = blockSize;
}

//after changing size, call this to reallocate if there is not enough memory
void allocArray(ReallocArray* array)
{
	//no need to realloc
	if (array->reserved >= array->size)
		return;
	
	//printf("About to reallocate %i for %i\n", array->reserved, array->size);
	while (array->reserved < array->size)
		array->reserved <<= 1; //double size
	array->data = realloc(array->data, array->reserved * array->blockSize);
	//printf("\tReallocated %i\n", array->reserved);
}

//reallocates the array to the exact size (for final use after allocArray)
void exactAllocArray(ReallocArray* array)
{
	if (array->reserved == array->size)
		return;
	
	array->reserved = array->size;
	array->data = realloc(array->data, array->reserved * array->blockSize);
}

//free allocated array data
void freeArray(ReallocArray* array)
{
	free(array->data);
	array->data = NULL;
	array->size = 0;
	array->reserved = 0;
	array->blockSize = 0;
}

//obj faces may have different combinations of
//vertex attributes (position/texture/normal).
//opengl must have a separate normal/texture coord per vertex position
//To reduce memory, only instances of unique vertex combinations are created.
//This is accomplished using a hash table (using uthash.h). the lookup key
//is the following struct
typedef struct _VertHashKey
{
	int v, t, n;
} VertHashKey;

//the key maps to a vertex index in the final interleaved array
typedef struct _VertHash
{
	VertHashKey key;
	int index;
    UT_hash_handle hh;
} VertHash;

//another hash table is used to map material
//names to the materials index in the OBJMesh
typedef struct _MatHash
{
	char* name;
	int index;
    UT_hash_handle hh;
} MatHash;

//a strtok that works (splits "//" correctly into two tokens, not one) and
//is also threadsafe. actually runs faster than the standard strtok without optimization
char* my_strtok(char* str, const char* d, char** tmp)
{
	char* r;
	if (str)
		*tmp = str;
	if (**tmp == '\0')
		return NULL;
	r = *tmp;
	*tmp += strcspn(*tmp, d);
	if (**tmp != '\0')
		*((*tmp)++) = '\0';
	return r;
}

//reads the next float from a tokenized string, with error handling
float readTokFloat(char** tmp, int* warn)
{
	char* t = my_strtok(NULL, " ", tmp);
	if (t)
	{
		char* e;
		float f = strtof(t, &e);
		if (f != 0.0f || e != t)
			return f; //success
	}
	
	//error converting
	*warn = 1;
	return 0.0f;
}

//removes whitespace and newlines at end of string (replaces with \0)
void trimRight(char* str)
{
	int i = strlen(str);
	char* c;
	for (c = str + i - 1; c > str; --c)
		if (*c != ' ' && *c != '\r' && *c != '\n' && *c != '\t')
			break;
	++c;
	*c = '\0';
}

//updates the current faceset indices are added to. handles removal of empty
//facesets and changing/reusing material and smoothing states
void setFaceSet(ReallocArray* facesets, int material, int smooth, int index)
{
	OBJFaceSet* edit = NULL;
	OBJFaceSet* last = NULL;
	if (facesets->size > 0)
		last = ((OBJFaceSet*)facesets->data) + (facesets->size-1);
	
	if (last && (last->material == material || material == -1) && last->smooth == smooth)
		return; //nothing to do
	
	if (last && last->indexStart == index) //no faces in set last set
	{
		if (material == -1 && smooth == -1) //unique combination to mark end of faceset
		{
			facesets->size -= 1; //end of unused faceset. remove
			return;
		}
		edit = last;
	}
	else
	{
		//update end index
		if (last)
			last->indexEnd = index;
			
		if (material == -1 && smooth == -1) //unique combination to mark end of faceset
			return;
	
		//append new faceset
		facesets->size += 1;
		allocArray(facesets);
		edit = ((OBJFaceSet*)facesets->data) + (facesets->size-1);
		edit->indexStart = index;
		edit->indexEnd = index; //unnecessary, but just in case
	}
	
	if (last && material == -1)
		edit->material = last->material; //use previous material (eg for changing smooth state)
	else
		edit->material = material; //simply update material. may be -1
	edit->smooth = smooth;
}

//returns the path of a filename, eg "../here/file.txt" -> "../here"
char* getFilepath(const char* filename)
{
	char* ret;
	ptrdiff_t size = 0;
	const char* last = strrchr(filename, '/');
	if (last)
		size = (last + 1) - filename;
	ret = (char*)malloc(size + 1);
	memcpy(ret, filename, size);
	ret[size] = '\0';
	return ret;
}

//declare the mtl parser for parsing a material (.mtl) file given in an .obj file
void parseMaterials(OBJMesh* mesh, const char* filename);

OBJMesh* objMeshLoad(const char* filename)
{
	int linenum = 1;
	int warning = 0;
	int fatalError = 0;
	
	//external files referenced by filename should have filepath appended
	char* filepath = getFilepath(filename);
	
	//open the ascii file
	FILE* file = fopen(filename, "r");
	
	//check it actually opened
	if (!file)
	{
		perror(filename);
		return NULL;
	}
	
	//the mesh we're going to return
	OBJMesh* mesh = (OBJMesh*)malloc(sizeof(OBJMesh));
	memset(mesh, 0, sizeof(OBJMesh));
	
	//we don't know how much data the obj file has, so we start with one and keep realloc-ing double
	ReallocArray vertices, faceSets, vertexHash, positions, normals, texCoords, triangles;
	initArray(&vertexHash, sizeof(VertHash*)); //array of hash record pointers (for freeing)
	initArray(&positions, sizeof(float) * 3);
	initArray(&normals, sizeof(float) * 3);
	initArray(&texCoords, sizeof(float) * 2);
	initArray(&triangles, sizeof(unsigned int) * 3);
	initArray(&faceSets, sizeof(OBJFaceSet));
	vertices.size = 0; //vertices are allocated later (at first face)
	
	//obj indices start at 1. we'll use the zero element for "error", giving with zero data
	positions.size = 1;
	normals.size = 1;
	texCoords.size = 1;
	allocArray(&positions);
	allocArray(&normals);
	allocArray(&texCoords);
	memset(positions.data, 0, positions.blockSize);
	memset(normals.data, 0, normals.blockSize);
	memset(texCoords.data, 0, texCoords.blockSize);
	
	//vertex combinations v/t/n are not always the same index. different vertex
	//combinations are hashed and reused, saving memory
	//see uthash.h (http://uthash.sourceforge.net/)
	VertHash* vertexRecords = NULL;
	
	//materials are referenced by name. this hash maps a name to material index
	MatHash* materialRecords = NULL;
	
	//whether the mesh contains normals or texture coordinates, and hence
	//the vertex data stride, is decided at the first face line ("f ...")
	int reachedFirstFace = 0;
	
	//current shading state
	int smoothShaded = 1;
	
	//start reading, line by line
	char* tok;
	char* tmpTok;
	char line[OBJ_MAX_LINE_LEN];
	while (fgets(line, OBJ_MAX_LINE_LEN, file) != NULL)
	{
		//split line by spaces
		tok = my_strtok(line, " ", &tmpTok);
		
		//what data does this line give us, if any?
		if (line[0] == 'v')
		{
			//this line contains vertex data
			int incomplete = 0;
			if (line[1] == '\0') //\0 as strtok replaced the space
			{
				//position data. allocate more memory if needed
				positions.size++;
				allocArray(&positions);
				
				//read data from string
				((float*)positions.data)[(positions.size-1)*3+0] = readTokFloat(&tmpTok, &incomplete);
				((float*)positions.data)[(positions.size-1)*3+1] = readTokFloat(&tmpTok, &incomplete);
				((float*)positions.data)[(positions.size-1)*3+2] = readTokFloat(&tmpTok, &incomplete);
			}
			else if (line[1] == 'n')
			{
				//normal data. allocate more memory if needed
				normals.size++;
				allocArray(&normals);
				
				//read data from string
				((float*)normals.data)[(normals.size-1)*3+0] = readTokFloat(&tmpTok, &incomplete);
				((float*)normals.data)[(normals.size-1)*3+1] = readTokFloat(&tmpTok, &incomplete);
				((float*)normals.data)[(normals.size-1)*3+2] = readTokFloat(&tmpTok, &incomplete);
			}
			else if (line[1] == 't')
			{
				//texture data. allocate more memory if needed
				texCoords.size++;
				allocArray(&texCoords);
				
				//read data from string
				((float*)texCoords.data)[(texCoords.size-1)*2+0] = readTokFloat(&tmpTok, &incomplete);
				((float*)texCoords.data)[(texCoords.size-1)*2+1] = readTokFloat(&tmpTok, &incomplete);
			}
			
			if (incomplete)
			{
				#if OBJ_PRINT_DEBUG
				printf("Mesh warning: incomplete data at line %s %i\n", line, linenum);
				#endif
				warning = 1;
			}
		}
		else if (line[0] == 'f')
		{
			//this line contains face data. this may have many vertices
			//but we just want triangles. so we triangulate.
			//NOTE: ALL vertex data must be given before being referenced by a face
			//NOTE: At least one vt or vn must be specified before the first f, or the
			//      mesh is considered not to have those vertex attributes
			if (!reachedFirstFace)
			{
				//must have previously specified vertex positions
				if (positions.size == 1)
				{
					fatalError = 1;
					break;
				}
				
				//calculate vertex stride
				mesh->hasNormals = (normals.size > 1) ? 1 : 0;
				mesh->hasTexCoords = (texCoords.size > 1) ? 1 : 0;
				mesh->normalOffset = 3 * sizeof(float);
				mesh->texcoordOffset = mesh->normalOffset + mesh->hasNormals * 3 * sizeof(float);
				mesh->stride = mesh->texcoordOffset + mesh->hasTexCoords * 2 * sizeof(float);
				initArray(&vertices, mesh->stride);
				reachedFirstFace = 1;
			}
			
			//start by extracting groups of vertex data (pos/tex/norm)
			int i = 0;
			char* vert[OBJ_MAX_POLYGON];
			while (i < OBJ_MAX_POLYGON && (vert[i] = my_strtok(NULL, " ", &tmpTok)) != NULL)
				++i;
			if (i == OBJ_MAX_POLYGON)
			{
				warning = 1;
				#if OBJ_PRINT_DEBUG
				printf("Mesh warning: reached polygon vertex limit at line %i\n", linenum);
				#endif
			}
			
			//triangulate using the "fan" method (splits could definitely be chosen better)
			int triVert = 0; //triVert contains the current face's vertex index. may not equal v as vertices can be ignored. 
			int triangulate[2];
			for (int v = 0; v < i; ++v)
			{
				//split groups by "/" and store in inds[3]
				int t = 0;
				int inds[3];
				char* tok = my_strtok(vert[v], "/", &tmpTok);
				while (t < 3 && tok != NULL)
				{
					inds[t++] = atoi(tok);
					tok = my_strtok(NULL, "/", &tmpTok);
				}
				while (t < 3)
					inds[t++] = 0;
				
				//set "provided, yet unused indices" as invalid. this
				//prevents unnecessary unique vertices being created
				if (!mesh->hasTexCoords)
					inds[1] = -1;
				if (!mesh->hasNormals)
					inds[2] = -1;
					
				//ignore vertex if position indices are out of bounds
				if (inds[0] < 0 || inds[0] >= positions.size)
				{
					warning = 1;
					#if OBJ_PRINT_DEBUG
					printf("Mesh warning: vertex index out of bounds at line %i\n", linenum);
					#endif
					continue;
				}
				
				//use zero for out of bound normals and texture coordinates
				if ((mesh->hasTexCoords && (inds[1] < 0 || inds[1] >= texCoords.size)) ||
					(mesh->hasNormals && (inds[2] < 0 || inds[2] >= normals.size)))
				{
					warning = 1;
					inds[1] = 0;
					inds[2] = 0;
					#if OBJ_PRINT_DEBUG
					printf("Mesh warning: non-vertex index out of bounds at line %i\n", linenum);
					#endif
				}
					
				//since vertices will be reused a lot, we need to hash the v/t/n combination
				int uniqueVertIndex;
				
				//check if the vertex already exists in hash
				VertHash h;
				VertHash* found = NULL;
				memset(&h, 0, sizeof(VertHash));
				h.key.v = inds[0];
				h.key.t = inds[1];
				h.key.n = inds[2];
				//printf("looking up %i/%i/%i\n", h.key.v, h.key.t, h.key.n);
				#if OBJ_INDEX_VERTICES
				HASH_FIND(hh, vertexRecords, &h.key, sizeof(VertHashKey), found);
				#endif
				//printf("done looking up\n");
				if (found)
				{
					//found. use that vertex
					//printf("vert %i/%i/%i already exists as %i\n", inds[0], inds[1], inds[2], found->index);
					uniqueVertIndex = found->index;
				}
				else
				{
					//printf("new vert %i/%i/%i\n", inds[0], inds[1], inds[2]);
				
					//not found. create a new vertex
					uniqueVertIndex = vertices.size++;
					allocArray(&vertices);
					
					//copy data for vertex
					memcpy(((float*)vertices.data) + uniqueVertIndex * mesh->stride / sizeof(float), 
						((float*)positions.data) + inds[0] * 3,
						sizeof(float) * 3);
					if (mesh->hasTexCoords)
						memcpy(((float*)vertices.data) + (uniqueVertIndex * mesh->stride + mesh->texcoordOffset) / sizeof(float),
							((float*)texCoords.data) + inds[1] * 2,
							sizeof(float) * 2);
					if (mesh->hasNormals)
					{
						memcpy(((float*)vertices.data) + (uniqueVertIndex * mesh->stride + mesh->normalOffset) / sizeof(float),
							((float*)normals.data) + inds[2] * 3,
							sizeof(float) * 3);
					}
					
					//add vertex to hash table
					#if OBJ_INDEX_VERTICES
					vertexHash.size++;
					allocArray(&vertexHash);
					VertHash* newRecord = (VertHash*)malloc(sizeof(VertHash));
					((VertHash**)vertexHash.data)[vertexHash.size-1] = newRecord; //store pointer to quickly free hash records later
					
					h.index = uniqueVertIndex;
					*newRecord = h;
					
					HASH_ADD(hh, vertexRecords, key, sizeof(VertHashKey), newRecord);
					#endif
				}
				
				if (triVert == 0)
				{
					//store the first vertex
					triangulate[0] = uniqueVertIndex;
				}
				else if (triVert > 1)
				{
					//printf("tri %i->%i->%i\n", triangulate[0], triangulate[1], uniqueVertIndex);
					
					//this is at least the 3rd vertex - we have a new triangle to add
					//always create triangles between the current, previous and first vertex
					triangles.size++;
					allocArray(&triangles);
				
					//read data from string
					((unsigned int*)triangles.data)[(triangles.size-1)*3+0] = triangulate[0];
					((unsigned int*)triangles.data)[(triangles.size-1)*3+1] = triangulate[1];
					((unsigned int*)triangles.data)[(triangles.size-1)*3+2] = uniqueVertIndex;
					
				}
				//store the last vertex
				triangulate[1] = uniqueVertIndex;
				++triVert;
			}
		}
		#if OBJ_ENABLE_MATERIALS
		else if (strcmp(line, "usemtl") == 0)
		{
			//the material state has changed - create a new faceset
			char* mtlname = my_strtok(NULL, " ", &tmpTok);
			if (!mtlname)
			{
				#if OBJ_PRINT_DEBUG
				printf("Error parsing model at %s:%i\n", filename, linenum);
				#endif
				warning = 1;
				continue;
			}
			trimRight(mtlname);
			
			//find the corresponding material
			MatHash* matRecord;
			HASH_FIND_STR(materialRecords, mtlname, matRecord);
			if (matRecord)
			{
				//printf("material: '%s'\n", mtlname);
				setFaceSet(&faceSets, matRecord->index, smoothShaded, triangles.size * 3);
			}
			else
			{
				warning = 1;
				#if OBJ_PRINT_DEBUG
				printf("Undefined material: '%s'\n", mtlname);
				#endif
				setFaceSet(&faceSets, -1, -1, triangles.size * 3);
			}
		}
		else if (strcmp(line, "mtllib") == 0)
		{
			//load all materials from the given .mtl file
			if (mesh->numMaterials > 0)
			{
				warning = 1; //shouldn't specify multiple .mtl files
				#if OBJ_PRINT_DEBUG
				printf("Warning: mesh %s contains references to multiple material files. ignoring\n", filename);
				#endif
				continue;
			}
			tok = my_strtok(NULL, " ", &tmpTok);
			if (!tok)
			{
				#if OBJ_PRINT_DEBUG
				printf("Error parsing model at %s:%i\n", filename, linenum);
				#endif
				warning = 1;
				continue;
			}
			
			//trim and append filepath
			trimRight(tok);
			char* mtlname = (char*)malloc(strlen(filepath) + strlen(tok) + 1);
			mtlname[0] = '\0';
			strcat(mtlname, filepath);
			strcat(mtlname, tok);
			
			//parse material file
			parseMaterials(mesh, mtlname);
				
			//add all materials to the hash
			MatHash* matRecord;
			for (int m = 0; m < mesh->numMaterials; ++m)
			{
				if (mesh->materials[m].name == NULL)
				{
					warning = 1;
					#if OBJ_PRINT_DEBUG
					printf("Warning %s contains invalid materials\n", mtlname);
					#endif
					continue;
				}
				HASH_FIND_STR(materialRecords, mesh->materials[m].name, matRecord);
				if (matRecord)
				{
					warning = 1;
					#if OBJ_PRINT_DEBUG
					printf("Multiple materials with name '%s'\n", mesh->materials[m].name);
					#endif
					continue; //can't have multiple definitions of the same material
				}
				matRecord = (MatHash*)malloc(sizeof(MatHash));
				matRecord->name = mesh->materials[m].name;
				matRecord->index = m;
				HASH_ADD_KEYPTR(hh, materialRecords, matRecord->name, strlen(matRecord->name), matRecord);
			}
			
			free(mtlname);
		}
		#endif //#if OBJ_ENABLE_MATERIALS
		else if (line[0] == 's')
		{
			#if !OBJ_IGNORE_SMOOTHING
			//smooth shading has been changed - create a new faceset
			char* smooth = my_strtok(NULL, " ", &tmpTok);
			trimRight(smooth);
			smoothShaded = atoi(smooth);
			if (strcmp(smooth, "on") == 0) smoothShaded = 1;
			if (strcmp(smooth, "off") == 0) smoothShaded = 0;
			setFaceSet(&faceSets, -1, smoothShaded, triangles.size * 3);
			#endif
		}
		//options o and g are ignored
		
		linenum++; //for warnings/errors
	}
	setFaceSet(&faceSets, -1, -1, triangles.size * 3); //update end of final faceset
	
	//fill the rest of the mesh structure
	exactAllocArray(&vertices);
	exactAllocArray(&triangles);
	exactAllocArray(&faceSets);
	mesh->vertices = (float*)vertices.data;
	mesh->indices = (unsigned int*)triangles.data;
	mesh->facesets = (OBJFaceSet*)faceSets.data;
	mesh->numVertices = vertices.size;
	mesh->numIndices = triangles.size * 3;
	mesh->numFacesets = faceSets.size;
	
	//cleanup
	//NOTE: vertices and indices are used in the returned mesh data and are not freed
	
	MatHash* matRecord;
	MatHash* matTmp;
	HASH_ITER(hh, materialRecords, matRecord, matTmp) {
	  HASH_DEL(materialRecords, matRecord);
	  free(matRecord);
	}
	
	HASH_CLEAR(hh, vertexRecords);
	for (int i = 0; i < vertexHash.size; ++i)
		free(((VertHash**)vertexHash.data)[i]);
	freeArray(&vertexHash);
	
	freeArray(&positions);
	freeArray(&normals);
	freeArray(&texCoords);
	
	fclose(file);
	
	free(filepath);
	
	if (fatalError != 0)
	{
		printf("Error: could not load mesh %s (line %i)\n", filename, linenum);
		objMeshFree(&mesh);
		return NULL;
	}
	if (warning != 0)
		printf("Warning: mesh %s contains errors\n", filename);
	
	return mesh;
}

//replaces or sets *dest with filepath + src.
//used for setting material and texture names
void replaceString(char** dest, const char* filepath, const char* src)
{
	if (*dest)
		free(*dest);
	*dest = (char*)malloc(strlen(filepath) + strlen(src) + 1);
	strcpy(*dest, filepath);
	strcpy((*dest) + strlen(filepath), src);
}


//free just the materials array in objmesh
//useful because the array contains string pointers to dynamic memory
void objMeshFreeMaterials(OBJMesh* mesh)
{
	if (!mesh->materials)
		return;
	for (int i = 0; i < mesh->numMaterials; ++i)
	{
		free(mesh->materials[i].name);
		free(mesh->materials[i].texture);
		free(mesh->materials[i].texNormal);
		free(mesh->materials[i].texSpecular);
	}
	free(mesh->materials);
	mesh->materials = NULL;
	mesh->numMaterials = 0;
}

//parses the material file "filename" and sets the material array in "mesh"
void parseMaterials(OBJMesh* mesh, const char* filename)
{
	int warning = 0;
	int notImportantWarning = 0;

	//external files referenced by filename should have filepath appended
	char* filepath = getFilepath(filename);
	
	//open/check etc
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		perror(filename);
		return;
	}
	
	ReallocArray materials;
	initArray(&materials, sizeof(OBJMaterial));
	OBJMaterial* mat = NULL;
	
	//read line by line
	char* tmpTok;
	char line[OBJ_MAX_LINE_LEN];
	while (fgets(line, OBJ_MAX_LINE_LEN, file) != NULL)
	{
		//split line by spaces
		my_strtok(line, " ", &tmpTok);
		
		//extract data
		if (mat && line[0] == 'K') //colours (rgb)
		{
			float col[4];
			col[0] = readTokFloat(&tmpTok, &warning);
			col[1] = readTokFloat(&tmpTok, &warning);
			col[2] = readTokFloat(&tmpTok, &warning);
			col[3] = readTokFloat(&tmpTok, &notImportantWarning);
			if (line[1] == 'a')	memcpy(mat->ambient, col, sizeof(col));
			if (line[1] == 'd')	memcpy(mat->diffuse, col, sizeof(col));
			if (line[1] == 's')	memcpy(mat->specular, col, sizeof(col));
		}
		else if (mat && (line[0] == 'd' || strcmp(line, "Tr") == 0)) //transparency
			mat->transparency = readTokFloat(&tmpTok, &warning);
		else if (mat && strcmp(line, "Ns") == 0) //specular shininess
			mat->shininess = readTokFloat(&tmpTok, &warning);
		else if (mat && strcmp(line, "illum") == 0) //rendering mode (not very useful)
		{
			char* mode = my_strtok(NULL, " ", &tmpTok);
			if (mode)
				mat->mode = atoi(mode);
		}
		else if (mat && strncmp(line, "map_", 4) == 0) //texture map
		{
			char* texname = my_strtok(NULL, " ", &tmpTok);
			trimRight(texname);
			if (strcmp(line, "map_Kd") == 0 || strcmp(line, "map_Ka") == 0) //colour map
			{
				if (line[5] == 'a' && mat->texture)
					continue; //can't replace a Kd with a Ka
				replaceString(&mat->texture, filepath, texname);
			}
			else if (mat && strcmp(line, "map_Ks") == 0) //specular map
			{
				replaceString(&mat->texSpecular, filepath, texname);
			}
			else if (mat && (strcmp(line, "map_bump") == 0 || strcmp(line, "bump") == 0)) //normal map
			{
				replaceString(&mat->texNormal, filepath, texname);
			}
		}
		else if (strcmp(line, "newmtl") == 0) //new material - create "mat"
		{
			//extract material name (referenced within the obj file)
			char* matname = my_strtok(NULL, " ", &tmpTok);
			if (!matname)
			{
				warning = 1;
				#if OBJ_PRINT_DEBUG
				printf("Mesh warning: newmtl without name\n");
				#endif
				continue; //can't add with no name
			}
			
			materials.size++;
			allocArray(&materials);
			mat = &((OBJMaterial*)materials.data)[materials.size-1];
			memset(mat, 0, sizeof(OBJMaterial));
			mat->shininess = 50.0f; //default
			
			trimRight(matname);
			replaceString(&mat->name, "", matname);
		}
	}
	
	//update the mesh's material array with the new one
	exactAllocArray(&materials);
	objMeshFreeMaterials(mesh);
	mesh->materials = (OBJMaterial*)materials.data;
	mesh->numMaterials = materials.size;
	
	free(filepath);
}

void objMeshFree(OBJMesh** mesh)
{
	//free all dynamic memory
	objMeshFreeMaterials(*mesh);
	free((*mesh)->vertices);
	free((*mesh)->indices);
	free((*mesh)->facesets);
	free(*mesh);
	*mesh = NULL;
}
