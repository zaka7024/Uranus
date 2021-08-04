#type vertex

#version 450 core

layout(location = 0) in vec3	a_Position;
layout(location = 1) in vec4	a_Color;
layout(location = 2) in vec2	a_TexCoord;
layout(location = 3) in float	a_TexIndex;
layout(location = 4) in float	a_TilingFactor;
layout(location = 5) in int		a_EntityId;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;
layout(location = 3) out flat float v_TexIndex;
out float v_TilingFactor;
layout (location = 4) flat out int v_EntityId;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	v_EntityId = a_EntityId;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment

#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;
layout(location = 3) in flat float v_TexIndex;

in vec4 v_Color;
in vec2 v_TexCoord;
//in float v_TexIndex;
in float v_TilingFactor;
layout (location = 4) flat in int v_EntityId;

uniform float u_TilingFactor;
uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_TexIndex);
	color = texture(u_Textures[index], v_TexCoord * v_TilingFactor) * v_Color;
	//color = vec4(index, 0,0,1);
	color2 = v_EntityId;
}
