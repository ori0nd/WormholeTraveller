#version 420 core

layout (location = 6) in vec3 position;
layout (location = 7) in vec3 normal;
layout (location = 8) in vec2 uv;

out VS_OUT {
	vec3 normal;
	vec3 position; 
	vec2 texCoord;
	vec3 reflectionVec;
} vs_out;

uniform mat4 modelViewProjection;
uniform vec3 eyePositionWorld;
uniform mat4 modelToWorld;
uniform int time;

void main(void)
{
	vec3 positionWorld = (modelToWorld * vec4(position, 1.0)).xyz;
	vec3 normalWorld = mat3(modelToWorld) * normal;

	normalWorld = normalize(normalWorld);

	vec3 incident = positionWorld - eyePositionWorld;

	vec3 R = reflect(incident, normal);

	vs_out.normal = normal;
	vs_out.position = (modelViewProjection * vec4(position, 1.0)).xyz;
	vs_out.texCoord = uv;
	vs_out.reflectionVec = R;

	gl_Position = modelViewProjection * vec4(position, 1.0);	
}