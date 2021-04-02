#version 430 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec4 gl_FragCoord;
 
uniform sampler2D screenTexture;
uniform sampler2D prevScreenTexture;
uniform bool isEvenFrame;

vec4 averageVector(in vec4[9] vectors)
{
	vec4 outVector = vec4(0, 0, 0, 0);
	for (int i = 0; i < 9; i++)
		outVector += vectors[i];
	outVector /= 9;
	return outVector;
}
 
void main()
{ 
	/*
	vec4 reconstructedPixel = averageVector(vec4[9]
	(
		texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0),
		texelFetch(screenTexture, ivec2(gl_FragCoord.x + 1, gl_FragCoord.y), 0),
		texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y + 1), 0),
		texelFetch(screenTexture, ivec2(gl_FragCoord.x - 1, gl_FragCoord.y), 0),
		texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y - 1), 0),
		texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x + 1, gl_FragCoord.y + 1), 0),
		texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x - 1, gl_FragCoord.y + 1), 0),
		texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x - 1, gl_FragCoord.y - 1), 0),
		texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x + 1, gl_FragCoord.y - 1), 0)
	));*/
	
	if (isEvenFrame) {
		if (mod(floor(gl_FragCoord.y), 2) == 1)
		{
			if (mod(floor(gl_FragCoord.x), 2) == 0) 
			{
				//FragColor = vec4(255, 0, 0, 0);
				//FragColor = texture(prevScreenTexture, TexCoords);
				FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
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
				//FragColor = vec4(255, 0, 0, 0);
				//FragColor = texture(prevScreenTexture, TexCoords);
				FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
			else 
			{
				FragColor = texture(screenTexture, TexCoords);
			}
		}
	}
	else {
		if (mod(floor(gl_FragCoord.y), 2) == 0)
		{
			if (mod(floor(gl_FragCoord.x), 2) == 0) 
			{
				//FragColor = vec4(255, 0, 0, 0);
				//FragColor = texture(prevScreenTexture, TexCoords);
				FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
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
				//FragColor = vec4(255, 0, 0, 0);
				//FragColor = texture(prevScreenTexture, TexCoords);
				FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
			else 
			{
				FragColor = texture(screenTexture, TexCoords);
			}
		}
	}
	//FragColor = vec4(abs(gl_FragCoord.x)/1000, abs(gl_FragCoord.y)/1000, 0, 0);
	//FragColor = texture(screenTexture, TexCoords);
}