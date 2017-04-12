
#include "Angel.h"

class Camera {

private:
	vec4 eye;
	vec4 at;
	vec4 up;

	float* camera_coordinates;
	float* at_coordinates;
	float* up_coordinates;

	mat4 modelView;
	mat4 projection;

	GLfloat  fovy;  // Field-of-view in Y direction angle (in degrees)
	GLfloat  aspect;       // Viewport aspect ratio
	GLfloat  zNear;
	GLfloat  zFar;

	bool updateModelView;
	bool updateProjection;

	GLuint  ModelView;
	GLuint  Projection;

	

public:
	Camera();
	Camera(vec4, vec4, vec4);
	void calculateModelView();
	void calculateProjection();
	void initUniforms(GLuint);
	void loadUniforms();
	void moveEye(int, float);
	void moveAt(int, float);
	void moveUp(int, float);

};

Camera::Camera(){
	Camera(vec4(0, 0, 2, 1), vec4(0, 0, 0, 1), vec4( 0.0, 1.0, 0.0, 0.0 ));
}
Camera::Camera(vec4 eye, vec4 at, vec4 up){

	this->fovy = 130;
	this->aspect = 1;
	this->zNear = 0.1;
	this->zFar = 15;

	this->eye = eye;
	this->at = at;
	this->up = up;

	camera_coordinates = new float[3];
	camera_coordinates[0] = eye.x, camera_coordinates[1] = eye.y, camera_coordinates[2] = eye.z;

	at_coordinates = new float[3];
	at_coordinates[0] = at.x, at_coordinates[1] = at.y, at_coordinates[2] = at.z;

	up_coordinates = new float[3];
	up_coordinates[0] = up.x, at_coordinates[1] = up.y, at_coordinates[2] = up.z;

	modelView = LookAt( eye, at, up );
	projection = Perspective( fovy, aspect, zNear, zFar );

	updateModelView = true;
	updateProjection = true;

}


void Camera::initUniforms(GLuint program) {
	ModelView = glGetUniformLocation( program, "model_view" );
    Projection = glGetUniformLocation( program, "projection" );
}

void Camera::loadUniforms(){

	if( updateModelView == true ) {
		calculateModelView();
	glUniformMatrix4fv( ModelView, 1, GL_TRUE, modelView );
	}
	if( updateProjection == true ){
		calculateProjection();
	glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );
	}
	
	updateModelView = false;
	updateProjection = false;
}

void Camera::moveEye(int index, float value){
	if(index == 0){
		eye.x += value;
	} else if(index == 1) {
		eye.y += value;
	} else if (index == 2) {
		eye.z += value;
	} else {
		return;
	}
	
	updateModelView = true;
}

void Camera::moveAt(int index, float value){
	if(index == 0){
		at.x += value;
	} else if(index == 1) {
		at.y += value;
	} else if (index == 2) {
		at.z += value;
	} else {
		return;
	}
	
	updateModelView = true;;
}

void Camera::moveUp(int index, float value){
	if(index == 0){
		up.x += value;
	} else if(index == 1) {
		up.y += value;
	} else if (index == 2) {
		up.z += value;
	} else {
		return;
	}
	
	updateModelView = true;
}

void Camera::calculateModelView(){
	this->modelView = LookAt( eye, at, up ); 
}

void Camera::calculateProjection(){
	this->projection = Perspective( fovy, aspect, zNear, zFar );
}