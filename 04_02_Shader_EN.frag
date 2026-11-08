//
// ================================================
// | Computer Graphics                            |
// ================================================
// | Laboratory IV - 04_02_Shader_EN.frag |
// ========================================
// 
//  Fragment shader - affects the color of the pixels;
//

#version 330

//	Input variables (coming from Shader.vert);
in vec4 ex_Color;

//	Output variables (to the frame buffer);
out vec4 out_Color;		//	The updated color;

void main(void)
  {
    out_Color = ex_Color;
  }