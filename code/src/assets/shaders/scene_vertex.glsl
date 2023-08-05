#version 330 core

uniform vec3 view_position;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colors;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 normals;

uniform mat4 world_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform mat4 light_view_proj_matrix;

out vec3 fragment_normal;
out vec3 fragment_position;
out vec4 fragment_position_light_space;
out vec3 vertexColor;
out vec2 vertexUV;

void main()
{
    fragment_normal = transpose(inverse(mat3(world_matrix)))* normals;
    fragment_position = vec3(world_matrix * vec4(position, 1.0));
    fragment_position_light_space = light_view_proj_matrix * vec4(fragment_position, 1.0);
    gl_Position = projection_matrix * view_matrix * world_matrix * vec4(position, 1.0);
	vertexColor = colors;
	vertexUV = uv;
}
