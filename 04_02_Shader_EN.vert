//
// ================================================
// | Computer Graphics                            |
// ================================================
// | Laboratory IV - 04_04_Shader_EN.vert |
// ========================================
// 
//  Vertex shader - affects the geometry of the scene; 
//

#version 330

//  Input variables (coming from the main program);
layout (location = 0) in vec4 in_Position;     //  The attribute containing the coordinates is taken from the buffer at the first position (0);
layout (location = 1) in vec4 in_Color;        //  The attribute containing the color is taken from the buffer at the second position (1);
layout (location = 2) in vec2 texCoord;          //  The attribute containing the texture is taken from the buffer at the third position (2);

//  Output variables;
out vec4 gl_Position;   //  Passes on the updated position;
out vec4 ex_Color;      //  Passes on the color (to be modified in Shader.frag); 
out vec2 tex_Coord;     //  Passes on the texture (to be modified in Shader.frag); 

//  Uniform variables;
uniform mat4 myMatrix;

void main(void)
  {
    gl_Position = myMatrix*in_Position;
	ex_Color=in_Color;
    tex_Coord = vec2(texCoord.x, 1-texCoord.y);
   }
