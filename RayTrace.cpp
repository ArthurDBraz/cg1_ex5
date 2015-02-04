/* ----------------------------------------------------------------
   name:           RayTrace.cpp
   version:	   
   TODO:           
   author:         
   ------------------------------------------------------------- */

#ifdef __APPLE__ 
  #include <OpenGL/gl.h>
#include <GL/freeglut.h>
#elif _WIN32
  #include "win32/glew.h"
#include "win32/freeglut.h"
#else
  #include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <string>
#include <iostream>

#include "glm/glm.hpp"
#define GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "Context.hpp"
#include "RayTrace.hpp"
#include "TriMesh.hpp"
#include "Image.hpp"
#include "GLSLShader.hpp"

using namespace glm;
using namespace std;

static TriMesh mesh;
// full screen quad

#define PI  3.14159265358979323846264338327950288f
#define RADIANS(x) (((x)*PI)/180.0f)

// current state of mouse action
static enum{
  ROTATE, SHIFT_XY, SHIFT_Z, SCALE, NO_DRAG, DRAW, ERASE
} drag= NO_DRAG;

static bool lighting= true;

static GLuint modulation= GL_MODULATE;

static mat4 cameraMatrix;
static mat4 rotation= mat4(1); // current rotation of object
static vec3 shift= vec3(0); // offset
static float scaling= 1; // scale

static vec2 screen;  // screen size
static float fov= 60.0; // field of view
// camera setup
// camera position
static GLfloat cameraZ;
// cameraZ in original sphere map setup
static GLfloat cameraZMap= 0;
// near and far plane
static GLfloat nearPlane;
static GLfloat farPlane;

static Image texture;

vector<Ray> rays;

/*************************************************************************************/

// load Shaders
void Common::loadShaders(){

}


static void reset(){

  rotation= mat4(1); // current rotation of object
  cameraZMap= 0;
  shift= vec3(0); // offset
  fov= 60.0;
}

void Common::keyPressed(unsigned char key, int x, int y){

  switch (key){

  case 'q':
  case 'Q':
    exit(EXIT_SUCCESS);
    break;
	  
  case 'r':
    reset();
    break;

  case 's':
    scaling*=0.9;
    break;

  case 'S':
    scaling*=1.1;
    break;

  case 'c':
    cameraZMap+=0.1;
    break;
    
  case 'C':
    cameraZMap-=0.1;
    break;
	 
  default: 
    break;
  }

  Context::displayWorldWindow();
}

// Create the rays
void createPrimaryRays(float windowX, float windowY, float samplingRate){

	mat4 projectionMatrix;
	glGetFloatv(GL_PROJECTION_MATRIX, &projectionMatrix[0][0]);
	mat4 modelViewMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelViewMatrix[0][0]);

	for (size_t y = 0; y < windowY; y++)
		for (size_t x = 0; x < windowX; x++)
		{
			vec3 camera = glm::vec3(glm::inverse(modelViewMatrix) * glm::vec4(0.f, 0.f, 0.f, 1.f));
			// unProject() give the coordinates like the image in world
			vec3 origin = glm::unProject(vec3(x, y, 0), modelViewMatrix, projectionMatrix, vec4(0, 0, windowX, windowY));
			vec3 direction = origin - camera;
			rays.push_back( Ray(origin, direction) );
		}
}

// -------------------------------------------------------
// RAY TRACING WINDOW
// -------------------------------------------------------

int RayTrace::menuOptions[]=  {0, 17, 18, 0, 1, 2, 3, 4, 5, 0, 6, 7, 8, 9, 10, 11, 0, 12, 13, 14, 15, 16, 0, 19, 20, 21, 22, 23, 24, 25, 26, 0, 27, 28}; 
string RayTrace::menuText[]= {"TOOLS:", "    Pen", "    Eraser", 
			     "SPHERICAL TEXTURES:", "    Earth", "    Earth (ice)", "    Earth (night)", "    Saturn", "    Marble", 
			     "ENVIRONMENT TEXTURES:", "    St Peters dome", "    Uffizium", "    Supernova", "    Landscape", "    Forest", "    Phong Light",
			     "MISC TEXTURES", "    Checkerboard", "    Grid1", "    Grid2",  "    Spectrum",  "    Nemo",
			     "FILTERING", "    mag: NEAREST",  "    mag: LINEAR", "    min: NEAREST" , "    min: LINEAR", "    min: NEAREST_MIPMAP_NEAREST  ", "    min: LINEAR_MIPMAP_NEAREST", "    min: NEAREST_MIPMAP_LINEAR", "    min: LINEAR_MIPMAP_LINEAR",
			   "SILHOUETTES", "    narrow", "    broad"};

vec2 RayTrace::previousMouse; // previous mouse position

void RayTrace::reshape(int width, int height){

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, width, height);

  gluOrtho2D(0, width, 0, height);

  screen= vec2(width, height);
}

// display ray tracing render
void RayTrace::display(void){

  // setup model matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClearColor(0.5, 0.5, 0.5, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutSolidSphere(2, 30, 10);

  //createPrimaryRays();

  glutSwapBuffers();
}

void RayTrace::mousePressed(int button, int state, int x, int y){

  if(button == GLUT_DOWN) previousMouse= vec2(x, y);
}

// mouse dragged callback
// XXX: NEEDS TO BE IMPLEMENTED
void RayTrace::mouseDragged(int x, int y){

  previousMouse= vec2(x, y);

    Context::displayTextureWindow();
	Context::displayWorldWindow();
}

void RayTrace::mouseMoved(int x, int y){

  if (x > 0 && x < screen.x && y > 0 && y < screen.y){
  }
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void RayTrace::menu(int value){

  switch(value){
  default: 
    break;
  }

    Context::displayTextureWindow();
	Context::displayWorldWindow();
}

// -------------------------------------------------------
// WORLD-SPACE WINDOW
// -------------------------------------------------------

int World::menuOptions[]= {24, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
			   0, 15, 16, 17, 18, 19, 20, 21, 22};
string World::menuText[]= {"    reset", "MODEL", "    Plane", "    Spiky Sphere", "    Car", "    Bunny", "    Cone", "    Cow", "    Cowboy Hat", "    Dragon", "    Chess", "    Temple", "    Cup", "    Space Shuttle", "    Sphere", "    None",
			   "RENDERING", "    Lighting on/off", "    Texture on/off", "    Coordinate System on/off", "    Origin on/off", 
			   "    Texture Coordinate Correction on/off  ", "    Texture Mode (WRAP/CLAMP) ", "    Environment mapping on/off", "    Move object/environment"};


static string models[]= {"", "data/quad.off", "data/4cow.off", "data/auto3.off", "data/bunny2.off", "data/cone.off", "data/cow.off", "data/cowboyhut.off", "data/MEGADRACHE.off", "data/Schachfigur.off", "data/tempel.off", "data/tasse.off", "data/spaceshuttle.off", "data/sphere.off"};


vec2 World::previousMouse;

struct LightSource lightSource; // light
struct Material material; // material

void World::reshape(int width, int height){

    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport to be the entire window
    glViewport(0, 0, width, height);

    cameraZ= 1 / tan(fov/180.0);

    // near and far plane
    nearPlane= cameraZ/10.0;
    farPlane= cameraZ*10.0;
    
    gluPerspective(fov, (float)width/(float)height, nearPlane, farPlane);
    
    //position the camera at (0,0,cameraZ) looking down the
    //negative z-axis at (0,0,0)
    cameraMatrix= lookAt(vec3(0.0, 0.0, cameraZ), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));

    screen= vec2(width, height);
}

// display callback
void World::display(void){

  glClearColor(0.2, 0.2, 0.2, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf(&cameraMatrix[0][0]);

  glEnable(GL_DEPTH_TEST);

  // shift object
  glTranslatef(shift.x, shift.y, shift.z);

  // rotate Object
  glMultMatrixf(&rotation[0][0]);

  glScalef(scaling, scaling, scaling);


  glutSwapBuffers();
}

void World::mousePressed(int button, int state, int x, int y){

  int modifier;

  switch(button){
  case GLUT_LEFT_BUTTON:
    if(state == GLUT_DOWN){
      previousMouse= vec2(x, y);
      modifier = glutGetModifiers();
      if(modifier & GLUT_ACTIVE_CTRL)
	drag = SHIFT_XY;
      else if(modifier & GLUT_ACTIVE_SHIFT)
	drag = SHIFT_Z;
      else
	drag = ROTATE;
    }
    else if(state == GLUT_UP){
      drag = NO_DRAG;
    }
    break;
  default: break;
  }
Context::displayWorldWindow();
}

void World::mouseDragged(int x, int y){

  vec2 v= (vec2(x,y) - previousMouse) / screen;
  
	switch(drag){
	case ROTATE:
	  if(length(v)==0) break;
	  rotation= rotate(mat4(1), RADIANS(180 * length(v)), normalize(vec3(v.y, v.x, 0))) * rotation;
		break;
	case SHIFT_XY:
	  if(false){
	    shift.x+= 3.3*v.x;
	    shift.y-= 3.3*v.y;	
	  }
	  else{
	    shift.x+= 3.3*v.x;
	    shift.y-= 3.3*v.y;
	  }  
		break;
	case SHIFT_Z:
	 shift.z+= 3.3*sign(dot(v, vec2(1,1))) * length(v);
	  break;
	default:
	  break;
	}
	previousMouse= vec2(x, y);

Context::displayWorldWindow();
}

// menu callback
// XXX: NEEDS TO BE IMPLEMENTED
void World::menu(int value){
   
  switch(value){
  case 24:
    reset();
    break;
  case 1:
    break;
  case 2:
  case 3:
  case 4:	
  case 5:	
  case 6:
  case 7:
  case 8:
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
    break;
  case 16:
    break;
  case 17:
    break;
  case 18:
    break;

  case 19:
    mesh.reload();
    mesh.center();
    mesh.unitize();
    mesh.computeNormals();
    break;
  case 20:
    break;
  case 21:
    break;
  case 22:
    reset();
    break;
  default:
    break;
  }
 Context::displayWorldWindow();
}

void World::setLighting(){
	lightSource.position = vec4(0, 0, 1, 1);
	lightSource.ambient = vec4(0.1, 0.1, 0.1, 1);
	lightSource.diffuse = vec4(1, 1, 1, 1);
	lightSource.specular = vec4(1, 1, 1, 1);
}

void World::setMaterial(){
	material.ambient = vec4(1, 1, 1, 1);
	material.diffuse = vec4(1, 1, 1, 1);
	material.specular = vec4(1, 1, 1, 1);
	material.shininess = 0.75;
}