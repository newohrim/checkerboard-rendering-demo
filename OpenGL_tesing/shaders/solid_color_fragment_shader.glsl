#version 430 core
	layout(early_fragment_tests) in;
	out vec4 FragColor;
	in vec3 FragPos;
	in vec3 Normal;
	
	uniform vec3 objectColor;
	uniform vec3 lightColor;
	uniform vec3 lightPos;
	
	float ambientStrength = 0.1;
	
	void main()
	{
		vec3 ambient = ambientStrength * lightColor;
  	
		// Диффузная составляющая 
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;
				
		vec3 result = (ambient + diffuse) * objectColor;
		FragColor = vec4(result, 1.0);
	}