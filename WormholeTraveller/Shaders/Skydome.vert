#version 420 core

layout (location = 0) in vec3 position;

out VS_OUT {
	vec3 texCoord;
} vs_out;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
	vec4 wvpPos = projection * modelView * vec4(position, 1.0);
	gl_Position = wvpPos.xyww;
	vs_out.texCoord = position;
}
