#version 460 core

// grid
in vec3 WorldPos;
in float gridSize;
in float gridMinPixelsBetween;


uniform sampler2D shadowMap;
uniform vec3 LightPos;


in vec4 FragPosLightSpace;
out vec4 FragColor;

float max2(vec2 v)
{
     return max(v.x, v.y);
} 

vec2 saturate(vec2 x)
{
	return clamp(x, vec2(0.0), vec2(1.0));
}

float log10(float value)
{
	return log(value) / log(10.0);
}




void main()
{
	vec4 gridColorThick = vec4(1.0, 0.5, 0.8, 1.0);
	vec4 gridColorThin = vec4(0.0, 0.0, 0.0, 1.0);
	float gridCellSize = 0.025;

	// making this, so camera rotation won't affect on the grid lines
	vec2 derivativeX = vec2(dFdx(WorldPos.x), dFdy(WorldPos.x));
	vec2 derivativeY = vec2(dFdx(WorldPos.z), dFdy(WorldPos.z));

	float lx = length(derivativeX);
	float ly = length(derivativeY);


	vec2 dudv = vec2(lx, ly);

	float len = length(dudv);

	// to fix Moire pattern would make distance between cells
	float LevelOfDetails = max(0.0, log10(len * gridMinPixelsBetween / gridCellSize) + 1.0);

	float GridCellSizeLod0 = gridCellSize * pow(10.0, floor(LevelOfDetails));
	float GridCellSizeLod1 = GridCellSizeLod0 * 10.0;
	float GridCellSizeLod2 = GridCellSizeLod1 * 10.0;

	dudv *= 4.0;

	vec2 modDiv = mod(WorldPos.xz, GridCellSizeLod0) / dudv;

	float Lod0a = max2(vec2(1.0) - abs(saturate(modDiv) * 2.0 - vec2(1.0)));

	modDiv = mod(WorldPos.xz, GridCellSizeLod1) / dudv;
	float Lod1a = max2(vec2(1.0) - abs(saturate(modDiv) * 2.0 - vec2(1.0)));

	modDiv = mod(WorldPos.xz, GridCellSizeLod2) / dudv;
	float Lod2a = max2(vec2(1.0) - abs(saturate(modDiv) * 2.0 - vec2(1.0)));

	vec4 Color;

	float LOD_fade = fract(LevelOfDetails);

	if(Lod2a > 0.0)
	{
		Color = gridColorThick;
			Color.a *= Lod2a;

	}
	else if (Lod1a > 0.0)
	{
		Color = mix(gridColorThick, gridColorThin, LOD_fade);
			Color.a *= Lod1a;

	}
	else
	{
		Color = gridColorThin;
		Color.a *= (Lod0a * (1.0 - LOD_fade));
	}
	

	FragColor = Color;
}

