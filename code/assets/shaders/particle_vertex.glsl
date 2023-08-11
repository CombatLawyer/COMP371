#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec3 vertexColor;
layout(location = 3) in vec2 vertexUV;


// Uniform Inputs
uniform mat4 ViewProjectionTransform;
uniform mat4 WorldTransform;

// Outputs to fragment shader
out vec3 normal;
out vec4 v_color;
out vec2 UV;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
    gl_Position =  ViewProjectionTransform * WorldTransform * vec4(vertexPosition, 1.0f);

    normal = vertexNormal; // Does this need to be transformed when we pass it to the fragment shader?
	v_color = vec4(vertexColor, 1.0f);
	UV = vertexUV;
}