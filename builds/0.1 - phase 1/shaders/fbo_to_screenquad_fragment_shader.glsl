#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;
layout (origin_upper_left) in vec4 gl_FragCoord;
 
uniform sampler2D screenTexture;
uniform sampler2D prevScreenTexture;
uniform bool isEvenFrame;
 
void main()
{ 
	if (isEvenFrame) {
		if (mod(floor(gl_FragCoord.y), 2) == 0)
		{
			if (mod(floor(gl_FragCoord.x), 2) == 0) 
			{
				FragColor = vec4(255, 0, 0, 0);
			}
			else 
			{
				FragColor = texture(screenTexture, TexCoords);
			}
		}
		else 
		{
			if (mod(floor(gl_FragCoord.x), 2) == 1) 
			{
				FragColor = vec4(255, 0, 0, 0);
			}
			else 
			{
				FragColor = texture(screenTexture, TexCoords);
			}
		}
	}
	else {
		if (mod(floor(gl_FragCoord.y), 2) == 1)
		{
			if (mod(floor(gl_FragCoord.x), 2) == 0) 
			{
				FragColor = vec4(255, 0, 0, 0);
			}
			else 
			{
				FragColor = texture(screenTexture, TexCoords);
			}
		}
		else 
		{
			if (mod(floor(gl_FragCoord.x), 2) == 1) 
			{
				FragColor = vec4(255, 0, 0, 0);
			}
			else 
			{
				FragColor = texture(screenTexture, TexCoords);
			}
		}
	}
	//FragColor = vec4(abs(gl_FragCoord.x)/1000, abs(gl_FragCoord.y)/1000, 0, 0);
}