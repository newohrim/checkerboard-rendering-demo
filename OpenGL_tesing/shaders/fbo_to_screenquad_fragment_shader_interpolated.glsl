#version 430 core
out vec4 FragColor;
  
in vec2 TexCoords;
in vec4 gl_FragCoord;
 
uniform sampler2D screenTexture;
uniform sampler2D prevScreenTexture;
uniform bool isEvenFrame;
uniform int interp_count;

vec4 averageVector(in vec4[9] vectors, in int count)
{
	vec4 outVector = vec4(0, 0, 0, 0);
	for (int i = 0; i < count; i++)
		outVector += vectors[i];
	outVector *= 0.11111;
	return outVector;
}
 
void main()
{ 
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
	), interp_count);
	
	bool x_even = bool(int(gl_FragCoord.x) & 1);
	bool y_even = bool(int(gl_FragCoord.y) & 1);
	
	vec4 outColor1 = texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
	vec4 outColor2 = reconstructedPixel;
	
	FragColor = (outColor1 * int(y_even) * int(x_even) + outColor1 * int(!y_even) * int(!x_even) +
				outColor2 * int(y_even) * int(!x_even) + outColor2 * int(!y_even) * int(x_even)) * int(isEvenFrame) + 
				(outColor1 * int(y_even) * int(!x_even) + outColor1 * int(!y_even) * int(x_even) +
	            outColor2 * int(y_even) * int(x_even) + outColor2 * int(!y_even) * int(!x_even)) * int(!isEvenFrame);
}