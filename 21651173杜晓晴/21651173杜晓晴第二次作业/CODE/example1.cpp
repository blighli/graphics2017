// Two-Dimensional Sierpinski Gasket       
// Generated using randomly selected vertices and bisection

#include "Angel.h"

const int NumPoints=8;

#include <iostream>
using namespace std;

//global variables to store position, orientation of turtle and status of pen, angle is in radians
double x,y,angle;
bool pen_down;

//global index for points vector to keep track of next point to be added
int index = 0;

//constant to convert degrees to radians
const double convertToRad = 0.0174532925;

vec2 p[NumPoints];
//----------------------------------------------------------------------------

//function prototypes
void right(double); 
void left(double);
void pen(bool);
void forward(double);


void
init(double x_coo, double y_coo, double alpha)
{
	//set initial values
	x = x_coo;
	y = y_coo;
	angle = alpha*convertToRad;

	/*example to draw a square, turtle turns left 45 degrees and moves forward with the pen up
	then turns to the right, goes forward with the pen down, and goes around the sides of the square
	and draws it
	*/

	pen(false);
	left(45);
	forward(0.5);

	pen(true);
	right(45+80);
	forward(0.7);

	right(90);
	forward(0.7);

	right(90);
	forward(0.7);

	right(90);
	forward(0.7);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(p), p, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader21.glsl", "fshader21.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

void right(double alpha){
	//convert degrees to radians and subtract them from current value of angle
	angle -= alpha * convertToRad;
}

void left(double alpha){
	//convert degrees to radians and add them to current value of angle
	angle += alpha * convertToRad;
}

void pen(bool pen__down){
	//update status of pen
	pen_down = pen__down;
}

void forward(double distance){
	//calculate new position
	double newX = x + (cos(angle) * distance);
	double newY = y + (sin(angle) * distance);
	
	//if pen is down, add two new points to connect, if pen is up, nothing happens
	if (pen_down){
		p[index++] = vec2(x,y);
		p[index++] = vec2(newX,newY);
	}

	//in both cases, current position of the turtle is updated
	x = newX;
	y = newY;
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT );     // clear the window
    glDrawArrays( GL_LINES, 0, NumPoints );    // draw the points

    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
    case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{

	glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( 500, 500 );


    glutCreateWindow( "Turtle Program" );


    glewInit();
    
	//initialize turtle in center and facing up, 90 degrees
    init(0.0,0.0,90.0);


    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}
