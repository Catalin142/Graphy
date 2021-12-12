#vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

uniform mat4 proj;
//uniform mat4 view;
//uniform mat4 model;

out vec2 otexcoord;

void main()
{
	gl_Position = proj * vec4(position, 1.0);
	otexcoord = texcoord;
}

#fragment
#version 330 core

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

layout (location = 0) out vec4 color;

in vec2 otexcoord;

void main()
{
	color = texture(material.diffuse, otexcoord) * material.shininess;
	if (color.a < 0.1)
		discard;
}