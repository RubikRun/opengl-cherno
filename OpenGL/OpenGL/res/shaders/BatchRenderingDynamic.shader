#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIdx;

out vec2 v_TexCoord;
out float v_TexIdx;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
   v_TexIdx = texIdx;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in float v_TexIdx;

uniform sampler2D u_Textures[2];

void main()
{
   vec4 texColor = texture(u_Textures[int(v_TexIdx)], v_TexCoord);
   color = texColor;
};