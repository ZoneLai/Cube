const char* camera_play_frag = STRINGIFY(

precision mediump float;

varying vec3 v_VetexColor;
varying vec2 v_TextureUV;

void main()
{
	gl_FragColor = vec4(v_VetexColor, 1.0);
}
);