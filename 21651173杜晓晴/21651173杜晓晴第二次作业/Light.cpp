
#include "Angel.h"

class Light {
private:
	
	vec4 light_position;
    vec4 light_ambient;
    vec4 light_diffuse;
    vec4 light_specular;

	vec4 material_ambient;
    vec4 material_diffuse;
    vec4 material_specular;
    float  material_shininess;

	
	vec4 ambient_product;
    vec4 diffuse_product;
    vec4 specular_product;

public:
	Light();
	Light(vec4, vec4, vec4, vec4, vec4, vec4, vec4, float);
	void loadUnforms(GLuint);
	
};

Light::Light(){

	vec4 light_position( 0.0, 0.0, 0.0, 1.0 );
    vec4 light_ambient( 1, 1, 1, 1.0 );
    vec4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    vec4 light_specular( 1.0, 1.0, 1.0, 1.0 );

    vec4 material_ambient( 0.3, 0.3, 0.3, 1.0 );
    vec4 material_diffuse( 1.0, 1, 1, 1.0 );
    vec4 material_specular( 1.0, 1, 1, 1.0 );
    float  material_shininess = 100.0;

	this->light_position = light_position;
    this->light_ambient = light_ambient;
    this->light_diffuse = light_diffuse;
    this->light_specular = light_specular;

    this->material_ambient = material_ambient;
    this->material_diffuse = material_diffuse;
    this->material_specular = material_specular;

	this->material_shininess = material_shininess;

	this->ambient_product = light_ambient * material_ambient;
    this->diffuse_product = light_diffuse * material_diffuse;
    this->specular_product = light_specular * material_specular;

	
}

Light::Light(vec4 light_position, vec4 light_ambient, vec4 light_diffuse, vec4 light_specular,
			 vec4 material_ambient, vec4 material_diffuse, vec4 material_specular, float material_shininess ){
	
	this->light_position = light_position;
    this->light_ambient = light_ambient;
    this->light_diffuse = light_diffuse;
    this->light_specular = light_specular;

    this->material_ambient = material_ambient;
    this->material_diffuse = material_diffuse;
    this->material_specular = material_specular;

	this->material_shininess = material_shininess;

	this->ambient_product = light_ambient * material_ambient;
    this->diffuse_product = light_diffuse * material_diffuse;
    this->specular_product = light_specular * material_specular;

}

void Light::loadUnforms(GLuint program){
	glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
		  1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
		  1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
		  1, specular_product );
	
    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
		  1, light_position );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
		 material_shininess );
}