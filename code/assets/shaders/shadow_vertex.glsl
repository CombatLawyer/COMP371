#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 light_view_proj_matrix;
uniform mat4 world_matrix;

void main()
{
    gl_Position = light_view_proj_matrix * world_matrix * vec4(position, 1.0f);
}
