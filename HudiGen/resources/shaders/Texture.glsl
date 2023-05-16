#type vertex

#version 450 core
			
layout(location = 0) in vec3   a_Position;
layout(location = 1) in vec4   a_Color;
layout(location = 2) in vec2   a_TexCoord;
layout(location = 3) in int    a_TexIndex;
layout(location = 4) in float  a_TilingFactor;
layout(location = 5) in int    a_QuadID;

uniform mat4 u_ProjectionView;

layout(location = 0) out vec4	    v_Color;
layout(location = 1) out vec2	    v_TexCoord;
layout(location = 2) out flat int   v_TexIndex;
layout(location = 3) out float	    v_TilingFactor;
layout(location = 4) out flat int   v_QuadID;

void main()
{
	v_Color			= a_Color;
	v_TexCoord		= a_TexCoord;
	v_TexIndex		= a_TexIndex;
	v_TilingFactor	= a_TilingFactor;
	v_QuadID		= a_QuadID;
	gl_Position		= u_ProjectionView * vec4(a_Position, 1.0);	
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

#type fragment
#version 450 core

layout(location = 0) in vec4	  v_Color;				
layout(location = 1) in vec2	  v_TexCoord;
layout(location = 2) in flat int  v_TexIndex;
layout(location = 3) in float     v_TilingFactor;
layout(location = 4) in flat int  v_QuadID;

uniform sampler2D u_Textures[32];

layout(location = 0) out vec4 color;
layout(location = 1) out int pixelID;

void main()
{
	color = texture(u_Textures[v_TexIndex], v_TexCoord * v_TilingFactor) * v_Color;
	pixelID = v_QuadID;
}
	