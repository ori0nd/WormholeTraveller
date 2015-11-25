#version 330 core

uniform mat4 MVPMatrix;

in vec4 VertexColor;
in vec4 VertexPosition;

out vec4 Color;

void main()
{
	Color = VertexColor;
	gl_Position = MVPMatrix * VertexPosition;
}
