#version 460 core

uniform mat4 gridViewProjection;
uniform vec3 cameraWorldPosition;


const vec3 gridVertices[4] = vec3[4](
	vec3(-1.0, 0.0, -1.0), // b left
	vec3(1.0, 0.0, -1.0), // b right
	vec3(1.0, 0.0, 1.0), // top right
	vec3(-1.0, 0.0, 1.0) // top left
);

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

out vec3 WorldPos;
out float gridSize;
out float gridMinPixelsBetween;

void main()
{
	float gridSize = 100.0;
	// variable to fix Moire pattern
	float grindMinPixelsBetween = 2.0;

	int index = Indices[gl_VertexID];
	vec3 vectorPos = gridVertices[index] * gridSize;

	vectorPos.x += cameraWorldPosition.x;
	vectorPos.z += cameraWorldPosition.z;

	vec4 vectorPos4 = vec4(vectorPos, 1.0);
		
	WorldPos = vectorPos;

	gl_Position = gridViewProjection * vectorPos4;
}

