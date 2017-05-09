#ifndef __GLM__
#define __GLM__
#include <OpenGL/glut.h>
#ifndef M_PI
#define M_PI 3.14159265
#endif
#define GLM_NONE     (0)
#define GLM_FLAT     (1 << 0)
#define GLM_SMOOTH   (1 << 1)
#define GLM_TEXTURE  (1 << 2)
#define GLM_COLOR    (1 << 3)
#define GLM_MATERIAL (1 << 4)

typedef struct _GLMmaterial
{
  char* name;
  GLfloat diffuse[4];
  GLfloat ambient[4];
  GLfloat specular[4];
  GLfloat emmissive[4];
  GLfloat shininess;
  char* map_file;
} GLMmaterial;
typedef struct {
  GLuint vindices[3];
  GLuint nindices[3];
  GLuint tindices[3];     
  GLuint findex;
} GLMtriangle;
typedef struct _GLMgroup {
  char*             name;
  GLuint            numtriangles;
  GLuint*           triangles;
  GLuint            material;
  struct _GLMgroup* next;
} GLMgroup;

typedef struct {
  char*    pathname;
  char*    mtllibname;
  GLuint   numvertices;
  GLfloat* vertices;
  GLuint   numnormals;
  GLfloat* normals;
  GLuint   numtexcoords;
  GLfloat* texcoords;
  GLuint   numfacetnorms;
  GLfloat* facetnorms;
  GLuint       numtriangles;
  GLMtriangle* triangles;
  GLuint       nummaterials;
  GLMmaterial* materials;
  GLuint       numgroups;
  GLMgroup*    groups
  GLfloat position[3];
} GLMmodel;
extern "C" {
GLfloat glmUnitize(GLMmodel* model, GLfloat center[3]);
GLvoid glmDimensions(GLMmodel* model, GLfloat* dimensions);
GLvoid glmScale(GLMmodel* model, GLfloat scale);
GLvoid glmReverseWinding(GLMmodel* model);
GLvoid glmFacetNormals(GLMmodel* model);
GLvoid glmVertexNormals(GLMmodel* model, GLfloat angle);
GLvoid glmLinearTexture(GLMmodel* model);
GLvoid glmSpheremapTexture(GLMmodel* model);
GLvoid glmDelete(GLMmodel* model);
GLMmodel*  glmReadOBJ(char* filename);
GLvoid glmWriteOBJ(GLMmodel* model, char* filename, GLuint mode);
GLvoid glmDraw(GLMmodel* model, GLuint mode);
GLuint glmList(GLMmodel* model, GLuint mode);
GLvoid glmWeld(GLMmodel* model, GLfloat epsilon);
}

#endif 
