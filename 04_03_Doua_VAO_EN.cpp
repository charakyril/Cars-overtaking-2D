//
// ================================================
// | Computer Graphics                            |
// ================================================
// | Laboratory IV - 04_03_DouaVAO_EN.cpp |
// ======================================
//
//	Program that draws two geometric figures;
//	NEW ELEMENTS:
//	 - using two VAOs to draw two different objects (each VAO contains the information about the vertices that define an object);
//
//
//
//	Libraries

#include <windows.h>        //	Use of Windows system functions (creating windows, handling files and directories);
#include <stdlib.h>         //  Libraries needed for reading the shaders;
#include <stdio.h>
#include <GL/glew.h>        //  Defines the OpenGL function prototypes and the constants needed for modern OpenGL programming;
#include <GL/freeglut.h>    //	Includes functions for:
							//	- managing windows and keyboard and mouse events,
							//  - drawing graphics primitives such as rectangles, circles or lines,
							//  - creating menus and submenus;
#include "loadShaders.h"	//	The file that links the program to the shaders;
#include "glm/glm.hpp"		//	Libraries used for graphics transformations;
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//  Identifiers of the OpenGL objects;
GLuint
VaoId1, VaoId2,                       // Identifiers for the two VAOs;
VboId1, VboId2,                       // Each VAO will contain its own buffers for coordinates, colors, indices;
ColorBufferId1, ColorBufferId2,
EboId1, EboId2,
ProgramId,
myMatrixLocation;
//	Size of the display window;
GLfloat
winWidth = 800, winHeight = 600;
//	Variables for the transformation matrices;
glm::mat4
myMatrix, resizeMatrix;
//	Variables for the orthogonal projection;
float xMin = -80.f, xMax = 80.f, yMin = -60.f, yMax = 60.f;

//  Creating and compiling the shader objects;
//	They must be in the same directory as the current project;
//  Vertex shader - affects the geometry of the scene;
//  Fragment shader - affects the color of the pixels;
void CreateShaders(void)
{
	ProgramId = LoadShaders("04_02_Shader_EN.vert", "04_02_Shader_EN.frag");
	glUseProgram(ProgramId);
}

//  Vertex Buffer Objects (VBOs) are initialized to transfer data to the graphics card memory (to the shaders);
//  They store data about the vertices (coordinates, colors, indices, texturing etc.);

// The first VAO is created - drawing a square in shades of blue
void CreateVAO1(void)
{

	//  Vertex coordinates;
	static const GLfloat Vertices1[] =
	{
		// walls (square)
		-60.0f, -40.0f, 0.0f, 1.0f,   // κάτω αριστερά 0 (-60, -40)
		-20.0f, -40.0f, 0.0f, 1.0f,   // κάτω δεξιά 1 (-20, -40)
		-20.0f,   0.0f, 0.0f, 1.0f,   // πάνω δεξιά 2 (-20, 0)
		-60.0f,   0.0f, 0.0f, 1.0f,   // πάνω αριστερά 3 (-60, 0)
		
		// door (rectangle)
		-55.0f, -25.0f, 0.0f, 1.0f,   // 7 (-55, -35)
		-15.0f, -25.0f, 0.0f, 1.0f,   // 8 (-15, -40)
		-15.0f, 5.0f, 0.0f, 1.0f,   // 9 (-35, -20)
		-55.0f, 5.0f, 0.0f, 1.0f    // 10 (-55, -20)
	};

	//	Colors as vertex attributes (one per vertex);
	static const GLfloat Colors1[] =
	{
		// walls - sand
		0.9f, 0.8f, 0.5f, 1.0f,
		0.9f, 0.8f, 0.5f, 1.0f,
		0.9f, 0.8f, 0.5f, 1.0f,
		0.9f, 0.8f, 0.5f, 1.0f,
		
		// door - brown
		0.4f, 0.2f, 0.1f, 1.0f,
		0.4f, 0.2f, 0.1f, 1.0f,
		0.4f, 0.2f, 0.1f, 1.0f,
		0.4f, 0.2f, 0.1f, 1.0f
	};

	//	Indices giving the drawing order (triangles);
	static const GLuint Indices1[] =
	{
		0, 1, 2,   0, 2, 3,     // walls
		4, 5, 6,   4, 7, 6 	// door
	};

	//  The first VAO (Vertex Array Object) is created / bound;
	glGenVertexArrays(1, &VaoId1);         //  Generating the VAO and assigning its index to the VaoId1 variable;
	glBindVertexArray(VaoId1);

	// Transmitting the data through buffers
	//  A buffer for the VERTICES is created;
	glGenBuffers(1, &VboId1);                                                        //  Generating the buffer and assigning its index to the VboId1 variable;
	glBindBuffer(GL_ARRAY_BUFFER, VboId1);                                           //  Setting the buffer type - vertex attributes;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices1), Vertices1, GL_STATIC_DRAW);      //  The points are "copied" into the current buffer;
	//  The attribute (0 = coordinates) is associated for the shader;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//  A buffer for the COLOR is created;
	glGenBuffers(1, &ColorBufferId1);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors1), Colors1, GL_STATIC_DRAW);
	//  The attribute (1 = color) is associated for the shader;
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//	A buffer for the INDICES is created;
	glGenBuffers(1, &EboId1);														//  Generating the buffer and assigning its index to the EboId1 variable;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices1), Indices1, GL_STATIC_DRAW);

}

// The second VAO is created - drawing a pentagon in shades of gray
void CreateVAO2(void)
{

	//  Vertex coordinates;
	static const GLfloat Vertices2[] =
	{
		// walls (square)
		60.0f, 40.0f, 0.0f, 1.0f,   // 0
		20.0f, 40.0f, 0.0f, 1.0f,   // 1
		20.0f,   0.0f, 0.0f, 1.0f,   // 2
		60.0f,   0.0f, 0.0f, 1.0f,   // 3
	};

	//	Colors as vertex attributes (one per vertex);
	static const GLfloat Colors2[] =
	{
		0.9f, 0.9f, 0.5f, 1.0f,
		0.9f, 0.3f, 0.5f, 1.0f,
		0.9f, 0.0f, 0.2f, 1.0f,
		0.9f, 0.8f, 0.5f, 1.0f
	};

	//	Indices giving the drawing order (triangles);
	static const GLuint Indices2[] =
	{
		0, 1, 2,   0, 2, 3,     // trunk
		//4, 5, 6,                // lower foliage
		//7, 8, 9                 // upper foliage
	};

	//  The second VAO (Vertex Array Object) is created / bound;
	glGenVertexArrays(1, &VaoId2);         //  Generating the VAO and assigning its index to the VaoId2 variable;
	glBindVertexArray(VaoId2);

	//  A buffer for the VERTICES is created;
	glGenBuffers(1, &VboId2);                                                        //  Generating the buffer and assigning its index to the VboId2 variable;
	glBindBuffer(GL_ARRAY_BUFFER, VboId2);                                           //  Setting the buffer type - vertex attributes;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
	//  The attribute (0 = coordinates) is associated for the shader;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//  A buffer for the COLOR is created;
	glGenBuffers(1, &ColorBufferId2);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors2), Colors2, GL_STATIC_DRAW);
	//  The attribute (1 = color) is associated for the shader;
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	//	A buffer for the INDICES is created;
	glGenBuffers(1, &EboId2);														//  Generating the buffer and assigning its index to the EboId2 variable;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices2), Indices2, GL_STATIC_DRAW);
}

//  Deletes the shader objects after running;
void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

//  Deleting the VBO objects after running;
void DestroyVBO(void)
{
	//  Releasing the attributes in the shaders (position, color, texturing etc.);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//  Deleting the buffers for VERTICES (Coordinates + Colors), INDICES;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId1);
	glDeleteBuffers(1, &ColorBufferId1);
	glDeleteBuffers(1, &EboId1);
	glDeleteBuffers(1, &VboId2);
	glDeleteBuffers(1, &ColorBufferId2);
	glDeleteBuffers(1, &EboId2);

	//  Releasing the VAO objects;
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId1);
	glDeleteVertexArrays(1, &VaoId2);
}

//  The function that releases the resources allocated by the program;
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

//  Setting the parameters required for the display window;
void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);		//  Background color of the screen;

	//  Passing the rendering data to the buffer used by the shaders;
	CreateVAO1();
	CreateVAO2();

	//  Initializing the shaders;
	CreateShaders();
	//	Instantiating the uniform variables used to "communicate" with the shaders;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	//	The "clipped" rectangle;
	resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);
}

//  The function that draws the graphics on the screen;
void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);			//  The OpenGL screen is cleared so that the new content can be drawn;
	//	Passing the uniform variable for the TRANSFORMATION MATRIX to the shaders;
	myMatrix = resizeMatrix;
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	//	Drawing the primitives

	glPointSize(20.0);
	// The first primitive (the square in shades of blue)
	glBindVertexArray(VaoId1);                                             // The first VAO is used;
	glDrawElements(GL_TRIANGLES, 15, GL_UNSIGNED_INT, (void*)(0));       // The drawing function;
	// glDrawElements(GL_POINTS, 5, GL_UNSIGNED_INT, (void*)(0));


	// The second primitive (the pentagon in shades of gray)
	glBindVertexArray(VaoId2);                                             // The second VAO is used;
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(0));       // The drawing function;
	// glDrawElements(GL_LINE_LOOP, 5, GL_UNSIGNED_INT, (void*)(0));

	// Exercises:
	// (i) comment out, in turn, the lines where glBindVertexArray() is called
	//		and draw the first primitive with the primitive type GL_POINT, and the second with GL_LINE_LOOP
	// (ii) draw the square using GL_TRIANGLES and GL_TRIANGLE_STRIP as primitive types - the Indices1 array must be modified accordingly!

	glFlush();								//  Ensures that all previously called OpenGL commands are executed;
}

//	Program entry point, the OpenGL routine is run;
int main(int argc, char* argv[])
{
	//  GLUT and the OpenGL context are initialized, and the window and display mode are configured;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);					//	Window display mode: a single display buffer and RGB colors are used;
	glutInitWindowSize(winWidth, winHeight);						//  Size of the window;
	glutInitWindowPosition(100, 100);								//  Initial position of the window;
	glutCreateWindow("Using two VAOs (Vertex Array Objects)");		//	Creates the display window, specifying its name;

	//	GLEW is initialized and the support for modern OpenGL extensions available on the host system is checked;
	//  It must be initialized before drawing;

	glewInit();

	Initialize();						//  Setting the parameters required for the display window;
	glutDisplayFunc(RenderFunction);	//  Drawing the scene in the window;
	glutCloseFunc(Cleanup);				//  Releasing the resources allocated by the program;

	//  The main GLUT event-processing loop (the functions starting with glut: glutInit etc.) is started;
	//  It processes events and draws the OpenGL window until the user closes it;

	glutMainLoop();

	return 0;
}