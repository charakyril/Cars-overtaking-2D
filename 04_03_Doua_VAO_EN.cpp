//
// ================================================
// | Computer Graphics                            |
// ================================================
// ΕΡΓΑΣΙΑ 
// ======================================
//
// Το vision είναι μία μικρή πίστα δρόμου όπου τα τρία απλά 
// θα κινούνται και τα ένα θα κάνει προσπέραση σε λορίδα 
//
//	Libraries

#include <windows.h>        //	Use of Windows system functions (creating windows, handling files and directories);
#include <stdlib.h>         //  Libraries needed for reading the shaders;
#include <stdio.h>
#include <math.h> 
#include <iostream> 
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
#include "SOIL.h"

//  Identifiers of the OpenGL objects;
GLuint
VaoId,                       // Identifiers for the two VAOs;
VboId,                       // Each VAO will contain its own buffers for coordinates, colors, indices;
ColorBufferId,
EboId,
ProgramId,
myMatrixLocation,
//myRotationLocation,
codCol,
codColLocation;

GLuint slowCarTexture, fastCarTexture, oncomingCar1Texture, oncomingCar2Texture;

//	Size of the display window;
GLfloat
winWidth = 400, winHeight = 500;
//	Variables for the transformation matrices;
glm::mat4
myMatrix, scaleMatrix, slowCarTranslation, fastCarTranslation, oncomingCarTranslation, fastCarRotation,
fastCarOrigin, fastCarBackCenter;
//myMatrix -> τελικός πίνακας του shader
// resizeMatrix -> φέρνει την σκηνή στο -1,1
// slowCarTranslation -> μετακίνηση αργού αμαξιού
// fastCarTranslation -> μετακίνηση του γρήγορου αμαξιού 
// oncomingCarTranslation -> μετακίνηση αντίθετησ κατευθυνσης
// fastCarRotation -> κίση γρήγορου αμαξιού 
//fastCarOrigin->κεντρο αυτοκινήτου 0,0
// fastCarBackCenter-> επαναφορά στη θέση του 
// 
//	Variables for the orthogonal projection;
//float xMin = -400.f, xMax = 400.f, yMin = -500.f, yMax = 500.f;

float slowCarsetY = 0.0f;	// argo autokinhto ston ajona y
float fastCarsetY = 0.0f;	//grhgoro ston ajona y
float oncomingCarssetY = 0.0f;	// antitheta ston y
float fastCarsetX = 0.0f;	// grhgoro allagh lvridas

float slowCarMinY = -500.0f;
float slowCarMaxY = -360.0f; 
float fastCarMinY = -1000.0f;
float fastCarMaxY = -860.0f; 

float gapBefore = 360.0f;
float gapAfter = -640.0f;

float fastCarCenterX = 230.0f;
float fastCarCenterY = -920.0f;
float fastAngle = 0;

int driving_straight = 1;	// 1= eutheia, 0 = allagh lvridas

void UpdatePositions() {
	slowCarsetY += 0.07f;
	fastCarsetY += 0.15f;	//grhgorh kinhsh gia prosperash 

	oncomingCarssetY -= 0.07f;
	slowCarMinY += 0.07f;
	slowCarMaxY += 0.07f;
	fastCarMinY += 0.15f;
	fastCarMaxY += 0.15f;

	gapBefore = slowCarMinY - fastCarMaxY;
	gapAfter = fastCarMinY - slowCarMaxY;
}







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
void CreateVAO(void)
{
	//BACKGROUND -- για τα αμάξια θα βάλουμε φωτογραφίες 
	//  Vertex coordinates;
	static const GLfloat Vertices[] =
	{
		
		-300.0f, -500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		300.0f,  -500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		300.0f, 500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-300.0f, 500.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		 
		0.0f, 500.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -500.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 
		-150.0f, 500.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-150.0f, 200.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-150.0f, 100.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-150.0f, -100.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-150.0f, -200.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-150.0f, -500.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 
		150.0f, 500.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		150.0f, 200.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		150.0f, 100.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		150.0f, -100.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		150.0f, -200.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		150.0f, -500.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		
		200.0f, -500.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		260.0f, -500.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		260.0f, -360.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		200.0f, -360.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		
		
		200.0f, -1000.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		260.0f, -1000.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		260.0f, -860.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		200.0f, -860.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		
		-260.0f, 360.0f,  0.0f, 1.0f, 0.5f, 0.7f, 0.0f, 0.0f, 0.0f,
		-200.0f, 360.0f,  0.0f, 1.0f, 0.5f, 0.7f, 0.0f, 1.0f, 0.0f,
		-200.0f, 500.0f,  0.0f, 1.0f, 0.5f, 0.7f, 0.0f, 1.0f, 1.0f,
		-260.0f, 500.0f,  0.0f, 1.0f, 0.5f, 0.7f, 0.0f, 0.0f, 1.0f,
		-100.0f, 360.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-40.0f, 360.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-40.0f, 500.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-100.0f, 500.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		
		220.0f, -930.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		240.0f, -930.0f, 0.0f, 1.0f, 0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
		240.0f, -910.0f, 0.0f, 1.0f, 0.6f, 0.0f, 0.0f, 1.0f, 1.0f,
		220.0f, -910.0f, 0.0f, 1.0f, 0.4f, 0.0f, 0.0f, 0.0f, 1.0f,
		
		-400.0f, -500.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-400.0f, 500.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 400.0f, -500.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 400.0f, 500.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		//fyta -- ua fygei 
		-380.0f, 420.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-320.0f, 420.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-320.0f, 480.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-380.0f, 480.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-380.0f, 220.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-320.0f, 220.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-320.0f, 280.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-380.0f, 280.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-380.0f, 40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-320.0f, 40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-320.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-380.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-380.0f, -140.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-320.0f, -140.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-320.0f, -80.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-380.0f, -80.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-380.0f, -320.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-320.0f, -320.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-320.0f, -260.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-380.0f, -260.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-380.0f, -480.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-320.0f, -480.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-320.0f, -420.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-380.0f, -420.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		380.0f, 420.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		320.0f, 420.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		320.0f, 480.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		380.0f, 480.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		380.0f, 220.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		320.0f, 220.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		320.0f, 280.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		380.0f, 280.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		380.0f, 40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		320.0f, 40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		320.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		380.0f, 100.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		380.0f, -140.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		320.0f, -140.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		320.0f, -80.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		380.0f, -80.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		380.0f, -320.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		320.0f, -320.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		320.0f, -260.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		380.0f, -260.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		380.0f, -480.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		320.0f, -480.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		320.0f, -420.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		380.0f, -420.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		
	};


	//	Indices giving the drawing order (triangles);
	static const GLuint Indices[] =
	{
		0, 1, 2, 3,
		4, 5,
		6, 7, 8, 9, 10, 11,
		12, 13, 14, 15, 16, 17,
		18, 19, 20, 21,
		22, 23, 24, 25,
		26, 27, 28, 29,
		30, 31, 32, 33,
		34, 35, 36, 37,
		38, 0, 3, 39,
		1, 40, 41, 2,
		42, 43, 44, 45,
		46, 47, 48, 49,
		50, 51, 52, 53,
		54, 55, 56, 57,
		58, 59, 60, 61,
		62, 63, 64, 65,
		66, 67, 68, 69,
		70, 71, 72, 73,
		74, 75, 76, 77,
		78, 79, 80, 81,
		82, 83, 84, 85,
		86, 87, 88, 89,
		90, 91, 92, 93
	};

	//  The first VAO (Vertex Array Object) is created / bound;
	glGenVertexArrays(1, &VaoId);         //  Generating the VAO and assigning its index to the VaoId1 variable;
	glBindVertexArray(VaoId);

	// Transmitting the data through buffers
	//  A buffer for the VERTICES is created;
	glGenBuffers(1, &VboId);                                                        //  Generating the buffer and assigning its index to the VboId1 variable;
	glBindBuffer(GL_ARRAY_BUFFER, VboId);                                           //  Setting the buffer type - vertex attributes;
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);      //  The points are "copied" into the current buffer;
	//  The attribute (0 = coordinates) is associated for the shader;
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

	//  A buffer for the COLOR is created;
	//glGenBuffers(1, &ColorBufferId1);
	//glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId1);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Colors1), Colors1, GL_STATIC_DRAW);
	//  The attribute (1 = color) is associated for the shader;
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));

	//	A buffer for the INDICES is created;
	glGenBuffers(1, &EboId);														//  Generating the buffer and assigning its index to the EboId1 variable;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

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
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	//  Deleting the buffers for VERTICES (Coordinates + Colors), INDICES;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &EboId);

	//  Releasing the VAO objects;
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);
}

//ΦΩΤΟΓΡΑΦΙΕΣ
void LoadTexture(const char* texture1, const char* texture2, const char* texture3, const char* texture4) {

	int width, height;
	unsigned char* image;

	glGenTextures(1, &slowCarTexture);
	glBindTexture(GL_TEXTURE_2D, slowCarTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	image = SOIL_load_image(texture1, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glGenTextures(1, &fastCarTexture);
	glBindTexture(GL_TEXTURE_2D, fastCarTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	image = SOIL_load_image(texture2, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glGenTextures(1, &oncomingCar1Texture);
	glBindTexture(GL_TEXTURE_2D, oncomingCar1Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	image = SOIL_load_image(texture3, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glGenTextures(1, &oncomingCar2Texture);
	glBindTexture(GL_TEXTURE_2D, oncomingCar2Texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	image = SOIL_load_image(texture4, &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);
}

//  The function that releases the resources allocated by the program;
void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

void resetOvertaking() {
	slowCarsetY = 0.0f;	// argo autokinhto ston ajona y
	fastCarsetY = 0.0f;	//grhgoro ston ajona y
	oncomingCarssetY = 0.0f;	// antitheta ston y
	fastCarsetX = 0.0f;	// grhgoro allagh lvridas

	slowCarMinY = -500.0f;
	slowCarMaxY = -360.0f;
	fastCarMinY = -1000.0f;
	fastCarMaxY = -860.0f;

	gapBefore = 360.0f;
	gapAfter = -640.0f;

	fastCarCenterX = 230.0f;
	fastCarCenterY = -920.0f;
	fastAngle = 0;

	driving_straight = 1;
}
//  Setting the parameters required for the display window;
void Initialize(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);		//  Background color of the screen;

	//  Passing the rendering data to the buffer used by the shaders;
	CreateVAO();  // CreateVBO();
	//CreateVAO2();

	LoadTexture("car1.png", "McQueen.png", "car2.png", "car3.png");

	//  Initializing the shaders;
	CreateShaders();
	//	Instantiating the uniform variables used to "communicate" with the shaders;
	//myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	//	The "clipped" rectangle;
	//resizeMatrix = glm::ortho(xMin, xMax, yMin, yMax);
}

//  The function that draws the graphics on the screen;
void RenderFunction(void)
{
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.f / winWidth, 1.f / winHeight, 1.0));
	
	//μετακίνηση του αργού,γρηγορου, αντιθετου αυτοκινήτου 
	slowCarTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, slowCarsetY, 0.0));

	fastCarTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(fastCarsetX, fastCarsetY, 0.0));

	oncomingCarTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, oncomingCarssetY, 0.0));

	fastCarRotation = rotate(glm::mat4(1.0f), fastAngle, glm::vec3(0.0, 0.0, 1.0));

	fastCarOrigin = glm::translate(glm::mat4(1.0f), glm::vec3(-fastCarCenterX, -fastCarCenterY, 0.0));

	fastCarBackCenter = glm::translate(glm::mat4(1.0f), glm::vec3(fastCarCenterX, fastCarCenterY, 0.0));

	glClear(GL_COLOR_BUFFER_BIT);			//  The OpenGL screen is cleared so that the new content can be drawn;

	//εαν το αργό αμάξι βγει εκτοσ ορίου ξανά απο την αρχή 
	if (slowCarMinY > 500.0f) {
		resetOvertaking();
	}

	codCol = 0; 
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	glUniform1i(codColLocation, codCol);

	myMatrix = scaleMatrix;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// σχεδιάζουμε τον δρόμο 
	glLineWidth(6.0);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINES, 4, GL_UNSIGNED_INT, (GLvoid*)(2 * sizeof(GLfloat)));
	glLineWidth(2.0);
	glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, (GLvoid*)(6 * sizeof(GLfloat)));
	 
	codCol = 2;
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	glUniform1i(codColLocation, codCol);

	//έξω απο τον δρόμο 
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_INT, (GLvoid*)(38 * sizeof(GLfloat)));

	codCol = 1;
	codColLocation = glGetUniformLocation(ProgramId, "codCol");
	glUniform1i(codColLocation, codCol);

	myMatrix = scaleMatrix;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// υφή για τα φυτά
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture5);
	//glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

	// τα φυτά στον πράσινο χώρο 
	glDrawElements(GL_QUADS, 48, GL_UNSIGNED_INT, (GLvoid*)(46 * sizeof(GLfloat)));

	////////

	//το αργό αυτοκίνητο 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, slowCarTexture);
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

	myMatrix = scaleMatrix * slowCarTranslation;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// Ζωγραφίζει το αργό αυτοκίνητο (4 δείκτες από τη θέση 18).
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (GLvoid*)(18 * sizeof(GLfloat)));

	//προσπέραση 
	driving_straight = 1;

	if (gapBefore <= 100.0f && gapBefore > 0.0f) {
		fastCarsetX -= 0.12f;
		fastAngle = 0.2;
		driving_straight = 0;

	}

	if (gapAfter >= 0.0f && fastCarsetX <= 0.0f) {
		fastCarsetX += 0.12f;
		fastAngle = -0.2f;
		driving_straight = 0;

	}

	if (driving_straight == 1) {
		fastAngle = 0.0;
	}

	//το γρήγορο αυτοκίνητο 
	glBindTexture(GL_TEXTURE_2D, fastCarTexture);
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

	myMatrix = scaleMatrix * fastCarTranslation* fastCarBackCenter*fastCarRotation* fastCarOrigin;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	// Ζωγραφίζει το γρήγορο αυτοκίνητο (4 δείκτες από τη θέση 22).
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (GLvoid*)(22 * sizeof(GLfloat)));


	//ta 2 αυτοκίνητα αντιθετης κατευθυνσης 

	myMatrix = scaleMatrix * oncomingCarTranslation;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	//πρώτο
	glBindTexture(GL_TEXTURE_2D, oncomingCar1Texture);
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (GLvoid*)(26 * sizeof(GLfloat)));

	//δευτερο 
	glBindTexture(GL_TEXTURE_2D, oncomingCar2Texture);
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);

	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (GLvoid*)(30 * sizeof(GLfloat)));

	UpdatePositions();
	glutSwapBuffers();
	glFlush();								//  Ensures that all previously called OpenGL commands are executed;
}

//	Program entry point, the OpenGL routine is run;
int main(int argc, char* argv[])
{
	//  GLUT and the OpenGL context are initialized, and the window and display mode are configured;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE| GLUT_RGB);					//	Window display mode: a single display buffer and RGB colors are used;
	glutInitWindowSize(winWidth, winHeight);						//  Size of the window;
	glutInitWindowPosition(200, 50);								//  Initial position of the window;
	glutCreateWindow("Overtaking 2D");		//	Creates the display window, specifying its name;

	//	GLEW is initialized and the support for modern OpenGL extensions available on the host system is checked;
	//  It must be initialized before drawing;

	glewInit();

	Initialize();						//  Setting the parameters required for the display window;
	glutDisplayFunc(RenderFunction);	//  Drawing the scene in the window;
	glutIdleFunc(RenderFunction);
	glutCloseFunc(Cleanup);				//  Releasing the resources allocated by the program;

	//  The main GLUT event-processing loop (the functions starting with glut: glutInit etc.) is started;
	//  It processes events and draws the OpenGL window until the user closes it;

	glutMainLoop();

	return 0;
}
