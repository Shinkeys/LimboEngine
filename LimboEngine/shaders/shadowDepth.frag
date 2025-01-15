#version 460 core


uniform sampler2D shadowMap;


uniform vec3 LightPos;

in vec3 FragPos;

in vec4 FragPosLightSpace;


out vec4 FragColor;	
float calculateShadow(vec4 fragPosLS)
{
	// we should do perspective divide for parameter
	// to transform it from vec4(homogenius) to vec3
	// now all the coords are in -1 1 range
	vec3 projCoords = fragPosLS.xyz / fragPosLS.w;

	// depth in depth map in the range 0,1 so we should make this
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;

	float currentDepth = projCoords.z;

	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;

	return shadow;
}

vec3 calculateLightning()
{
    vec3 color = vec3(0.2);
	vec3 lightDir = (FragPos - LightPos);

	vec3 lightColor = vec3(1.0);

	vec3 ambient = 0.3 * lightColor;

	vec3 normal = vec3(0.0, 1.0, 0.0);

	float  diff = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diff * lightColor;

	float shadow = calculateShadow(FragPosLightSpace);

	vec3 light = (ambient + (1.0 - shadow) * diffuse * color);

	return light;
}

void main()
{
	FragColor = vec4(calculateLightning(), 1.0);
}