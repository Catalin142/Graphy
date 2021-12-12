#vertex
#version 460 core

layout(location = 0) in vec4  v_Position;
layout(location = 1) in vec2 v_TexCoords;
layout(location = 2) in vec4 v_Color;
layout(location = 3) in float v_TexIndex;

out vec2 o_TexCoord;
out vec4 o_Color;
out flat float o_TexIndex;

uniform mat4 u_Orthographic;

void main()
{
	gl_Position = u_Orthographic * v_Position;
	o_TexCoord = v_TexCoords;
	o_Color = v_Color;
	o_TexIndex = v_TexIndex;
}

#fragment
#version 460 core
layout(location = 0) out vec4 FragColor;

in vec2 o_TexCoord;
in vec4 o_Color;
in flat float o_TexIndex;

uniform sampler2D u_Tex[32];

void main()
{
	vec4 pixelColor;
	switch(int(o_TexIndex))
	{
		case 0: pixelColor = vec4(1.0, 1.0, 1.0, 1.0); break;
		case 1: pixelColor = texture(u_Tex[1], o_TexCoord); break;
		case 2: pixelColor = texture(u_Tex[2], o_TexCoord); break;
		case 3: pixelColor = texture(u_Tex[3], o_TexCoord); break;
		case 4: pixelColor = texture(u_Tex[4], o_TexCoord); break;
		case 5: pixelColor = texture(u_Tex[5], o_TexCoord); break;
		case 6: pixelColor = texture(u_Tex[6], o_TexCoord); break;
		case 7: pixelColor = texture(u_Tex[7], o_TexCoord); break;
		case 8: pixelColor = texture(u_Tex[8], o_TexCoord); break;
		case 9: pixelColor = texture(u_Tex[9], o_TexCoord); break;
		case 10: pixelColor = texture(u_Tex[10], o_TexCoord); break;
		case 11: pixelColor = texture(u_Tex[11], o_TexCoord); break;
		case 12: pixelColor = texture(u_Tex[12], o_TexCoord); break;
		case 13: pixelColor = texture(u_Tex[13], o_TexCoord); break;
		case 14: pixelColor = texture(u_Tex[14], o_TexCoord); break;
		case 15: pixelColor = texture(u_Tex[15], o_TexCoord); break;
		case 16: pixelColor = texture(u_Tex[16], o_TexCoord); break;
		case 17: pixelColor = texture(u_Tex[17], o_TexCoord); break;
		case 18: pixelColor = texture(u_Tex[18], o_TexCoord); break;
		case 19: pixelColor = texture(u_Tex[19], o_TexCoord); break;
		case 20: pixelColor = texture(u_Tex[20], o_TexCoord); break;
		case 21: pixelColor = texture(u_Tex[21], o_TexCoord); break;
		case 22: pixelColor = texture(u_Tex[22], o_TexCoord); break;
		case 23: pixelColor = texture(u_Tex[23], o_TexCoord); break;
		case 24: pixelColor = texture(u_Tex[24], o_TexCoord); break;
		case 25: pixelColor = texture(u_Tex[25], o_TexCoord); break;
		case 26: pixelColor = texture(u_Tex[26], o_TexCoord); break;
		case 27: pixelColor = texture(u_Tex[27], o_TexCoord); break;
		case 28: pixelColor = texture(u_Tex[28], o_TexCoord); break;
		case 29: pixelColor = texture(u_Tex[29], o_TexCoord); break;
		case 30: pixelColor = texture(u_Tex[30], o_TexCoord); break;
		case 31: pixelColor = texture(u_Tex[31], o_TexCoord); break;
	}
	
	FragColor = o_Color * pixelColor;
}