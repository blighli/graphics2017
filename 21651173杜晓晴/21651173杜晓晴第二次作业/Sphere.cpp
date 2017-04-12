#include <string>
#include <windows.h>
#include <stdlib.h>
#include "Angel.h"
#include "imageloader.h"

using namespace std;

class Sphere {
private:
	float radius;
	float offset_x, offset_y, offset_z;
	string texture_name;
	vec4* points;
	vec2* tex_coords;
	vec3* normals;
	GLuint texture;
	GLuint buffer;
	GLfloat* Theta;
	float* speed;
	GLuint theta;
	int number_of_points;
	GLuint vPosition, vNormal, vTexCoord;


	void generateSpherePoints();
	void generateSphereTexture();
	bool inPoints(vec4, int);

public:
	Sphere();
	Sphere(float, float, float, float, string, float, float, float, float, float, float);
	int getNumberOfPoints();
	vec4* getSpherePoints();
	vec2* getSphereTextureCoords();
	vec3* getSphereNormals();
	GLuint getTexture();
	void loadBufferDataPoints(GLuint);
	void drawSphere();
	void changeTheta();

};

Sphere::Sphere() {
	
}

Sphere::Sphere(float radius, float offset_x, float offset_y, float offset_z, string texture_name,
			   float angle_x, float angle_y, float angle_z, float speed_x, float speed_y, float speed_z){
	this->radius = radius;
	this->offset_x = offset_x;
	this->offset_y = offset_y;
	this->offset_z = offset_z;
	this->texture_name = texture_name;
	this->number_of_points = 57680;
	this->points = new vec4[this->number_of_points];
	this->tex_coords = new vec2[this->number_of_points];
	this->normals = new vec3[this->number_of_points];
	this->Theta = new GLfloat[3];
	Theta[0] = angle_x; Theta[1] = angle_y; Theta[2] = angle_z;
	this->speed = new float[3];
	speed[0] = speed_x, speed[1] = speed_y, speed[2] = speed_z;
	generateSpherePoints();
	generateSphereTexture();
}


void Sphere::generateSpherePoints() {

	

	float angle_offseta = 0.5, angle_offsetb = 10;
	int Index = 0;
	for(float b = 0; b <= 90; b+=angle_offsetb){
		for(float a = 0; a <= 360; a+=angle_offseta)
    {
		float x = offset_x+radius*sin(a*DegreesToRadians) * sin(b*DegreesToRadians),
			  y = offset_y+radius*cos(a*DegreesToRadians) * sin(b*DegreesToRadians),
			  z = offset_z+radius*cos(b*DegreesToRadians),
			  ty = (2*b)/360,
			  tx = a/360.0;

		if( inPoints(vec4( x, y, z, 1.0), Index) == false ){
		points[Index] = vec4( x, y, z, 1.0);
		this->tex_coords[Index] = vec2( tx, -ty);
		Index++;
		}

		x = offset_x+radius*sin(a*DegreesToRadians) * sin((b+angle_offsetb)*DegreesToRadians);
		y = offset_y+radius*cos(a*DegreesToRadians) * sin((b+angle_offsetb)*DegreesToRadians);
		z = offset_z+radius*cos((b+angle_offsetb)*DegreesToRadians);
		ty = (2*(b+angle_offsetb))/360;
		tx = a/360;

		if( inPoints(vec4( x, y, z, 1.0), Index) == false ){
		points[Index] = vec4( x, y, z, 1.0);
		this->tex_coords[Index] = vec2( tx, -ty);
		Index++;
		}

		x = offset_x+radius*sin((a+angle_offsetb)*DegreesToRadians) * sin(b*DegreesToRadians);
		y = offset_y+radius*cos((a+angle_offsetb)*DegreesToRadians) * sin(b*DegreesToRadians);
		z = offset_z+radius*cos(b*DegreesToRadians);
	    ty = (2*b)/360;
		tx = (a+angle_offsetb)/360;

		if( inPoints(vec4( x, y, z, 1.0), Index) == false ){
		points[Index] = vec4( x, y, z, 1.0);
		this->tex_coords[Index] = vec2( tx, -ty);
		Index++;
		}
		//cout << "In: " << Index-4 << endl;
		//system("pause");
		vec4 u, v;
		vec3 normal;
		if(Index - 4 >= 0 ){
		u = points[Index-4]-points[Index-3];
		v = points[Index-2]-points[Index-3];
		normal = normalize( cross(u, v) );
		} else {
		u = points[Index-3]-points[Index-2];
		v = points[Index-1]-points[Index-2];
		normal = normalize( cross(u, v) );
		}
		normals[Index-4] = normal;
		normals[Index-3] = normal;
		normals[Index-2] = normal;

		x = offset_x+radius*sin((a+angle_offsetb)*DegreesToRadians) * sin((b+angle_offsetb)*DegreesToRadians);
		y = offset_y+radius*cos((a+angle_offsetb)*DegreesToRadians) * sin((b+angle_offsetb)*DegreesToRadians);
		z = offset_z+radius*cos((b+angle_offsetb)*DegreesToRadians);
		ty = (2*(b+angle_offsetb))/360;
		tx = (a+angle_offsetb)/360;
		if( inPoints(vec4( x, y, z, 1.0), Index) == false ){
		points[Index] = vec4( x, y, z, 1.0);
		this->tex_coords[Index] = vec2( tx, -ty);
		Index++;
		}
    }

}	
	cout << "Done1: " << Index << endl;
	int current_vertices = Index;
	for (int b = 0; b <current_vertices; b++) {
		vec4 u, v;
		vec3 normal;
		points[Index] = points[b];
		points[Index].z = -points[Index].z;
		this->tex_coords[Index] = this->tex_coords[b];
		this->tex_coords[Index].y = -this->tex_coords[Index].y;
		/*if(b % 4 == 0 && b>0){
		u = points[Index-4]-points[Index-3];
		v = points[Index-2]-points[Index-3];
		normal = normalize( cross(u, v) );
		normals[Index-4] = normal;
		normals[Index-3] = normal;
		normals[Index-2] = normal;
		}*/
		normals[Index] = normals[b];
		normals[Index].z = -normals[Index].z;
		Index++;
	}
	/*int trianle_number = 12;
	for(int i = 0; i<number_of_points; i+=trianle_number){
		for(int j = i+1; j<i+trianle_number && j<number_of_points; j++)
		normals[j] = normals[i];
	}*/
	int nz = 0, nn=0;
	for(int i = 0; i<number_of_points; i++){
		if(normals[i].x == 0 && normals[i].y == 0 && normals[i].z == 0){
			normals[i] = normals[i-1];
			nz++;
		}
		else
			nn++;
	}
	cout << "Nz: " << nz << " NN: " << nn << endl;
	
	/*for (int b = 0; b <current_vertices; b++) {
		vec4 u = points[Index-3]-points[Index-2];
		vec4 v = points[Index-1]-points[Index-2];
		vec3 normal = normalize( cross(u, v) );
		normals[Index-3] = normal;
		normals[Index-2] = normal;
		normals[Index-1] = normal;
	}*/
	//cout << "Done: " << Index << endl;
	/*cout << tex_coords[0] << " " << tex_coords[1] << tex_coords[4839] << endl;
	cout << points[0] << " " <<  points[1] <<  points[4839] << endl;
	cout << normals[0] << " " <<  normals[1] <<  normals[4839] << endl;*/
}

void Sphere::generateSphereTexture() {
	Image* image = loadBMP((char*)this->texture_name.c_str());
	glGenTextures(1, &texture);
	 //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, texture); //Tell OpenGL which texture to edit
	//Map the image to the texture
	
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	delete image;
}

int Sphere::getNumberOfPoints(){
	return this->number_of_points;
}

vec4* Sphere::getSpherePoints() {
	return this->points;
}

vec2* Sphere::getSphereTextureCoords(){
	return tex_coords;
}

GLuint Sphere::getTexture(){
	return this->texture;
}

bool Sphere::inPoints(vec4 new_point, int Index){
	return false;

	if(Index < 20)
		return false;

	for(int i = Index-20; i<Index; i++){
		if( points[i].x == new_point.x &&  points[i].y == new_point.y &&  points[i].z == new_point.z ){
			return true;
		}
	}
	return false;

}

vec3* Sphere::getSphereNormals() {
	return normals;
}

void Sphere::loadBufferDataPoints(GLuint program){

	glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
	glBufferData( GL_ARRAY_BUFFER,
		sizeof(vec4)*number_of_points+sizeof(vec2)*number_of_points+sizeof(vec3)*number_of_points,
		  NULL, GL_STATIC_DRAW );

	GLintptr offset = 0;
	glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(vec4)*number_of_points, points );
		offset += sizeof(vec4)*number_of_points;

		glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(vec3)*number_of_points, normals ); 
		offset += sizeof(vec3)*number_of_points;	
	
		glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(vec2)*number_of_points, tex_coords ); 
	offset += sizeof(vec2)*number_of_points;	
	theta  = glGetUniformLocation( program, "theta" );

	offset = 0;
	vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    /*glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );
    offset += sizeof(vec4)*number_of_points;*/

	vNormal = glGetAttribLocation( program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
   /* glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );*/

	offset+=sizeof(vec3)*number_of_points;

    vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
   /* glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );*/
	glUniform1i( glGetUniformLocation(program, "texture"), 0 );
}

void Sphere::drawSphere(){
	glBindBuffer(GL_ARRAY_BUFFER, this->buffer);
	GLintptr offset = 0;

	 glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );
	  offset += sizeof(vec4)*number_of_points;

	 glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );
	 offset+=sizeof(vec3)*number_of_points;

	  glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			   BUFFER_OFFSET(offset) );
	glUniform3fv( theta, 1, Theta );
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays( GL_TRIANGLES, 0, number_of_points);
}

void Sphere::changeTheta(){

	for(int i = 0; i<3; i++){
		Theta[i] += speed[i];
	if ( Theta[i] > 360.0 ) {
		Theta[i] -= 360.0;
    } else if (Theta[i] < 0 ){
		Theta[i] += 360;
	}
	}
	glutPostRedisplay();
}