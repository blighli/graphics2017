//
// Perspective view of a color cube using LookAt() and Perspective()
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.
//

#include "Angel.h"
#include "Sphere.cpp"
#include "Camera.cpp"
#include "Light.cpp"

int number_of_spheres = 11;
Sphere *spheres;
Camera camera(vec4( 0, 0, -3, 1.0 ), vec4( 0, 0, 3, 1.0 ), vec4( 0.0, 1.0, 0.0, 0.0 ));
// OpenGL initialization
void
init()
{

	spheres = new Sphere[number_of_spheres];
	spheres[0] = Sphere(1,  0,      0, 0, "son.bmp",       0, 5,   0, 0,   0.3,   0);
	spheres[1] = Sphere(0.38,  15.27,  0, 0, "mercury.bmp",   0, 360,   0, 0.3, 0.3, 0);
	spheres[2] = Sphere(0.94,  17.44,  0, 0, "venus.bmp",     0, 315,  0, 0.3, 0.3, 0);
	spheres[3] = Sphere(1,     19.94,  0, 0, "earth.bmp",     0, 270,  0, 0.3, 0.3, 0);
	spheres[4] = Sphere(0.53,  21.13,  0, 0, "mars.bmp",      0, 225,  0, 0.3, 0.3, 0);
	spheres[5] = Sphere(10.97, 37.81,  0, 0, "jupiter.bmp",   0, 180, 0, 0.3, 0.3, 0);
	spheres[6] = Sphere(9.14,  57.03,  0, 0, "saturn.bmp",    0, 135, 0, 0.3, 0.3, 0);
	spheres[7] = Sphere(3.98,  61.99,  0, 0, "uran.bmp",      0, 90, 0, 0.3, 0.3, 0);
	spheres[8] = Sphere(3.86,  96.43,  0, 0, "neptun.bmp",    0, 45, 0, 0.3, 0.3, 0);
	spheres[9] = Sphere(0.18,  136.63, 0, 0, "pluto.bmp",     0, 0, 0, 0.3, 0.3, 0);
	spheres[10] = Sphere(30,     0,    -10, 0, "galaxy3.bmp",   0, 0, 0, 0, 0.01, 0);


    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
	 GLuint program = InitShader( "vshader42.glsl", "fshader42.glsl" );
	     glUseProgram( program );
	for(int i=0; i<number_of_spheres; i++){
			spheres[i].loadBufferDataPoints(program);
	}

	Light l;
	l.loadUnforms(program);

	camera.initUniforms(program);
	camera.loadUniforms();


    
    glEnable( GL_DEPTH_TEST );
	glShadeModel(GL_SMOOTH);
    glClearColor( 0.0, 0.0, 0.0, 0.0 ); 
}

//----------------------------------------------------------------------------
bool change = false;
void
display( void )
{
	
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	camera.loadUniforms();
	for(int i=0; i<number_of_spheres; i++){
	spheres[i].drawSphere();
	}
	

    glutSwapBuffers();
}

//----------------------------------------------------------------------------
bool pause = true;
void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
	    break;

	//case 'z': zNear  *= 1.1; zFar *= 1.1; break;
	//case 'Z': zNear *= 0.9; zFar *= 0.9; break;
	case 'a': camera.moveEye(0, -0.1);  break;
	case 'd': camera.moveEye(0, 0.1);  break;
	case 's': camera.moveEye(2, 0.1);  break;
	case 'w': camera.moveEye(2, -0.1); break;
	case 'p': pause = !pause; break;

	/*case ' ':  // reset values to their defaults
	    zNear = 0.5;
	    zFar = 3.0;*/
    }

    glutPostRedisplay();
}

//----------------------------------------------------------------------------

/*void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );

    aspect = GLfloat(width)/height;
}*/

//----------------------------------------------------------------------------

void
idle( void )
{
	if( pause )
		return;

	for(int i = 0; i<number_of_spheres; i++){
	spheres[i].changeTheta();
}
	
    //
	//display();
}

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 1024, 1024 );
    //glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    //glutReshapeFunc( reshape );
	 glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
