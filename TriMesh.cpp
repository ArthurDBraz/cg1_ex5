/* ----------------------------------------------------------------
   name:           TriMesh.cpp
   purpose:        cg1_ex3 2014 triangle mesh for OpenGL rendering
   version:	   SKELETON CODE
   TODO:           computeSphereUVs
   author:         katrin lang
                   computer graphics
                   tu berlin
   ------------------------------------------------------------- */

#include "TriMesh.hpp"
#include "fstream"
#include "iostream"

#include "glm\glm\gtc\constants.hpp"

// use this with care
// might cause name collisions
using namespace glm;

using namespace std;

// NVIDIA wants it like this
// see https://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/attributes.php
const GLuint TriMesh::attribVertex= 0;
const GLuint TriMesh::attribNormal= 2;
const GLuint TriMesh::attribColor= 3;

TriMesh::TriMesh(){
  winding= CW;
}

TriMesh::TriMesh(const std::string& fileName, bool normalize){

name= fileName;
  winding= CW;
  loadOff(fileName);
  if (normalize){
	  center();
	  unitize();
  }
  computeNormals();
}

TriMesh::~TriMesh(){
}

void TriMesh::setWinding(PolygonWinding winding){
  this->winding= winding;
}

// center model at its origin
void TriMesh::center(void){

  calculateBoundingBox();

  vec3 center= (boundingBoxMin + boundingBoxMax) * vec3(0.5);
  
  for(unsigned int i= 0; i<positions.size(); i++){  
    positions[i]-= center;
  }
  boundingBoxMin-= center;
  boundingBoxMax-= center;
}

// normalize to bounding sphere radius 1
void TriMesh::unitize(void){

  calculateBoundingSphere();

  for(unsigned int i= 0; i<positions.size(); ++i){
    positions[i]/= boundingSphereRadius;
  }
  boundingSphereRadius= 1;
  boundingBoxMin= vec3(-1);
  boundingBoxMax= vec3(1);
}

// calculate bounding sphere
void TriMesh::calculateBoundingSphere(void){

  boundingSphereRadius= 0;
  for(unsigned int i= 0; i<positions.size(); i++){
    vec3 v= positions[i];
    if(length(v) > boundingSphereRadius) boundingSphereRadius= length(v);
  }
}

// calculate bounding box
void TriMesh::calculateBoundingBox(void){

  boundingBoxMin= vec3(numeric_limits<float>::max());
  boundingBoxMax= vec3(numeric_limits<float>::min());
  for(unsigned int i= 0; i<positions.size(); i++){
    if(positions[i].x < boundingBoxMin.x) boundingBoxMin.x= positions[i].x;
    if(positions[i].x > boundingBoxMax.x) boundingBoxMax.x= positions[i].x;
    if(positions[i].y < boundingBoxMin.y) boundingBoxMin.y= positions[i].y;
    if(positions[i].y > boundingBoxMax.y) boundingBoxMax.y= positions[i].y;
    if(positions[i].z < boundingBoxMin.z) boundingBoxMin.z= positions[i].z;
    if(positions[i].z > boundingBoxMax.z) boundingBoxMax.z= positions[i].z;
  }
}

void TriMesh::correctTexture(bool correct){
  textureCorrection= correct;
}

// load triangle mesh in OFF format
void TriMesh::reload(){
  loadOff(name);
}

// load triangle mesh in .OFF format
void TriMesh::loadOff(const string& fileName){

	name= fileName;

	positions.clear();
	normals.clear();
	faces.clear();

	ifstream file(fileName);
	string line;
	if (!file.is_open()){
		cout << "File not found" << '\n';
		exit(0);
	}
	// take away trash
	getline(file, line);

	// read these fields from the file
	unsigned int num_vertex, num_polygon, num_sides;
	file >> num_vertex >> num_polygon >> num_sides;

	// read vertex from file and put on vector "positions"
	for (size_t i = 0; i < num_vertex; i++)
	{
		float x, y, z;
		file >> x >> y >> z;
		positions.push_back(glm::vec3(x, y, z));
	}

	// read polygons from the file
	int trash;
	for (size_t i = 0; i < num_polygon; i++)
	{
		unsigned int x, y, z;
		file >> trash >> x >> y >> z;
		faces.push_back(glm::uvec3(x, y, z));
		trash++;
	}

	file.close();
}


// calculate smooth per-vertex normals
void TriMesh::computeNormals(void){
	vector<vec3> faceNormals;

	// Calculate the normals for all faces
	for (uvec3 face : faces) {
		faceNormals.push_back(
			glm::normalize(glm::cross(
			(positions[face.y] - positions[face.x]),
			(positions[face.z] - positions[face.x])
			)));
	}

	// Iterate over all vertices
	for (int i = 0; i < positions.size(); ++i) {
		int numFaces = 0;
		vec3 normal = vec3(0, 0, 0);

		// Determines which face contains this vertex and add the face normal to the vertex normal
		for (int j = 0; j < faces.size(); ++j) {
			if (faces[j].x == i || faces[j].y == i || faces[j].z == i) {
				++numFaces;
				normal += faceNormals[j];
			}
		}

		// Weight the face normals and calculate the final vertex normal
		normal /= numFaces;
		normals.push_back(glm::normalize(normal));
	}
}

// draw the mesh using vertex arrays
void TriMesh::draw(void){

	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, &positions[0]);
	glEnableVertexAttribArray(attribVertex);

	glVertexAttribPointer(attribNormal, 3, GL_FLOAT, GL_FALSE, 0, &normals[0]);
	glEnableVertexAttribArray(attribNormal);

	glDrawElements(GL_TRIANGLES, faces.size() * 3, GL_UNSIGNED_INT, &faces[0]);

	glBindVertexArray(0);

}
