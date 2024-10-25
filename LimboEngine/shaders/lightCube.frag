#version 460 core
out vec4 FragColor;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_roughness1;



in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;

void main()
{
	vec3 diffuse = texture(texture_diffuse1, TexCoords).rgb;
	vec3 specular = texture(texture_specular1, TexCoords).rgb;


	vec3 normalMap = normalize(Normal);
    normalMap = normalize(normalMap * 2.0 - 1.0);
    vec3 norm = normalize(normalMap);

	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);


	float roughness = texture(texture_roughness1, TexCoords).r;

	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64 * (1.0 - roughness));



	vec3 ambient = 0.1 * diffuse;


	vec3 res = (ambient + diff * diffuse + spec * specular);	

	FragColor = vec4(res, 1.0);
}