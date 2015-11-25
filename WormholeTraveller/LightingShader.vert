#version 330 core

layout (location = 0) in vec4 position;
layout (location = 2) in vec4 color;
layout (location = 1) in vec3 normal;

out vec4 fragColor;
out vec4 fragNormal;
out vec3 fragPosition;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
	// set the normal (transformed into world coords)
	fragNormal = vec4(normal, 1.0);
	fragPosition = position.xyz;
	fragColor = color;

	mat4 modelViewProjection = projection * modelView;

	gl_Position = modelViewProjection * position;
}
