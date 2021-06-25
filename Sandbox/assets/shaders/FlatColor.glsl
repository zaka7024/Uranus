#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;

out vec3 v_Position;

void main() {
	//v_Color = a_Color * vec4(a_Position, 1.0);
	//v_Color = a_Color;
	v_Position = a_Position;
	gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;
uniform vec4 u_Color;

in vec3 v_Position;

void main() {
	//color = vec4(v_Position + 0.5, 1);
	color = u_Color;
}
