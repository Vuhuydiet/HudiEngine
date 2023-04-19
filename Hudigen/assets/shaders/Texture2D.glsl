#type vertex
#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ProjectionView;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	gl_Position = u_ProjectionView * vec4(a_Position, 1.0);	
}

////////////////////////////////////////////////////////////////////

#type fragment
#version 330 core

in vec4 v_Color;				
in vec2 v_TexCoord;
in float v_TexIndex;
			
uniform sampler2D u_Textures[32];

out vec4 color;

void main()
{
	color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
}
	