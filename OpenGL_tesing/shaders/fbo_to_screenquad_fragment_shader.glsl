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
	bool x_even = bool(int(gl_FragCoord.x) & 1);
	bool y_even = bool(int(gl_FragCoord.y) & 1);
	
	vec4 outColor1 = texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
	vec4 outColor2 = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
	
	FragColor = (outColor1 * int(y_even) * int(x_even) + outColor1 * int(!y_even) * int(!x_even) +
				outColor2 * int(y_even) * int(!x_even) + outColor2 * int(!y_even) * int(x_even)) * int(isEvenFrame) + 
				(outColor1 * int(y_even) * int(!x_even) + outColor1 * int(!y_even) * int(x_even) +
	            outColor2 * int(y_even) * int(x_even) + outColor2 * int(!y_even) * int(!x_even)) * int(!isEvenFrame);
}