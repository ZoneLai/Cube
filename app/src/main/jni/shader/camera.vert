const char* camera_play_vert = STRINGIFY(

attribute vec3    a_Position;
attribute vec3    a_Color;
attribute vec2    a_TextureUV;

uniform   mat4    u_MvpMatrix;
varying   vec3    v_VetexColor;
varying   vec2    v_TextureUV;

void main()
{
	gl_Position     = u_MvpMatrix * vec4(a_Position, 1.0);
	v_VetexColor    = a_Color;
	v_TextureUV     = a_TextureUV;
}
);