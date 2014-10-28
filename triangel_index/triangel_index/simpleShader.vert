#version 330

uniform mat4 mat_view;
uniform mat4 mat_proj;
uniform mat4 mat_rot;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 ver_color;
out vec3 ver_normal;
out vec3 ver_pos;
void main(void)
{
    vec4 pos = vec4(position,1);
    gl_Position = mat_proj * mat_view * mat_rot * pos;
    
    ver_color = vec3(0.5,0.5,0.5);
    ver_normal = normal;
}

