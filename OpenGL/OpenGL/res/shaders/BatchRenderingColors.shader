#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 vColor;

uniform mat4 u_MVP;

out vec4 fColor;

void main()
{
   gl_Position = u_MVP * position;
   fColor = vColor;
};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 fColor;

void main()
{
    color = fColor;
};