#version 460 core
layout(location = 0) out vec4 FragColor;


in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

struct Material{
	sampler2D texture_diffuse;
	sampler2D texture_normal;
	sampler2D texture_emission;
//	sampler2D texture_roughness;
	sampler2D texture_specular;
};

uniform Material material;

vec3 calculateLightning()
{
	vec3 diffuse = texture(material.texture_diffuse, TexCoords).rgb;
	vec3 specular = texture(material.texture_specular, TexCoords).rgb;
	vec3 emission = texture(material.texture_emission, TexCoords).rbg;

	vec3 normalMap = normalize(Normal);
    normalMap = normalize(normalMap * 2.0 - 1.0);
    vec3 norm = normalize(normalMap);

	vec3 lightDir = normalize(FragPos - LightPos);
	float diff = max(dot(norm, -lightDir), 0.0);


	//float roughness = texture(material.texture_roughness1, TexCoords).r;

	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	// halfway direction - vector of halfway for a blinn-phong light
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 64); //* (1.0 - roughness)//

	// gamma attenuation
	float distance = length(LightPos - FragPos);
	float attenuation = 1.0 / distance * distance;
	diffuse *= attenuation;

	vec3 ambient = 0.3 * diffuse;

	return ambient + diff * diffuse + spec * specular;
}

void main()
{
	vec3 res = calculateLightning();	

	FragColor = vec4(res, 1.0);
}