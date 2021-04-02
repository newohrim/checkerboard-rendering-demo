#version 430 core
out vec4 FragColor;
  
in vec2 TexCoords;
 
uniform sampler2D screenTexture;
 
void main()
{ 
	FragColor = texture(screenTexture, TexCoords);
	//FragColor = vec4(abs(gl_FragCoord.x)/1000, abs(gl_FragCoord.y)/1000, 0, 0);
}