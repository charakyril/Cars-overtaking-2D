//
// ================================================
// | Computer Graphics                            |
// ================================================
// | Laboratory IV - 04_04_Shader_EN.frag |
// ========================================
// 
//  Fragment shader - affects the color of the pixels;
//

#version 330

//	Input variables (coming from Shader.vert);
in vec4 ex_Color;
in vec2 tex_Coord;		//	Texture coordinate;

//	Output variables (to the frame buffer);
out vec4 out_Color;		//	The updated color;

//  Uniform variables;
uniform sampler2D myTexture;
uniform int codCol;		

//	Variables for colors;
vec4 black = vec4(0.0,0.0,0.0,1.0);

void main(void)
  {
	if ( codCol==0 )
		out_Color = ex_Color;
	if (codCol == 1)
	     out_Color =  texture(myTexture, tex_Coord);
    if (codCol == 2)
	     out_Color =  vec4(0.2,0.5,0.4,1.0);
  }
  
