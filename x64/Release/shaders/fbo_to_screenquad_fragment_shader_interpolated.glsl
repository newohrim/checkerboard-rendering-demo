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
	outVector /= count;
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
	
	//int x_even = int(gl_FragCoord.x) & 1;
	//int y_even = int(gl_FragCoord.y) & 1;
	
	vec4 outColor1 = texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
	vec4 outColor2 = reconstructedPixel;
					//texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
	
	FragColor = (outColor1 * int(y_even) * int(x_even) + outColor1 * int(!y_even) * int(!x_even) +
				outColor2 * int(y_even) * int(!x_even) + outColor2 * int(!y_even) * int(x_even)) * int(isEvenFrame) + 
				(outColor1 * int(y_even) * int(!x_even) + outColor1 * int(!y_even) * int(x_even) +
	            outColor2 * int(y_even) * int(x_even) + outColor2 * int(!y_even) * int(!x_even)) * int(!isEvenFrame);
	
	
	/*
	if (y_even == !isEvenFrame) 
	{
		if (x_even == isEvenFrame) 
		{
			FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
		}
		else 
		{
			FragColor = texture(screenTexture, TexCoords);
		}
		//FragColor = vec4(255, 0, 0, 0);
	}
	else 
	{
		if (x_even == !isEvenFrame)
		{
			FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
		}
		else 
		{
			FragColor = texture(screenTexture, TexCoords);
		}
		//FragColor = vec4(255, 0, 0, 0);
	}*/
	/*
	if (isEvenFrame) {
		if (y_even == 1) // !isEvenFrame
		{
			if (x_even == 0) // isEvenFrame
			{
				//FragColor = vec4(255, 0, 0, 0);
				//FragColor = texture(prevScreenTexture, TexCoords);
				FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
			else 
			{
				//FragColor = texture(screenTexture, TexCoords);
				FragColor = texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
		}
		else 
		{
			if (x_even == 1) // isEvenFrame
			{
				//FragColor = vec4(255, 0, 0, 0);
				//FragColor = texture(prevScreenTexture, TexCoords);
				FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
			else 
			{
				//FragColor = texture(screenTexture, TexCoords);
				FragColor = texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
		}
	}
	else {
		if (y_even == 0)
		{
			if (x_even == 0) 
			{
				//FragColor = vec4(255, 0, 0, 0);
				//FragColor = texture(prevScreenTexture, TexCoords);
				FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
			else 
			{
				//FragColor = texture(screenTexture, TexCoords);
				FragColor = texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
		}
		else 
		{
			if (x_even == 1) 
			{
				//FragColor = vec4(255, 0, 0, 0);
				//FragColor = texture(prevScreenTexture, TexCoords);
				FragColor = texelFetch(prevScreenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
			else 
			{
				//FragColor = texture(screenTexture, TexCoords);
				FragColor = texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
			}
		}
	}*/
	//FragColor = vec4(abs(gl_FragCoord.x)/1000, abs(gl_FragCoord.y)/1000, 0, 0);
	//FragColor = texture(screenTexture, TexCoords);
	//FragColor = texelFetch(screenTexture, ivec2(gl_FragCoord.x, gl_FragCoord.y), 0);
}