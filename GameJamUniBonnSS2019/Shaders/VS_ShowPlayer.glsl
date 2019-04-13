#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoordinates;

uniform float direction;
uniform mat4 viewProjectionMatrix;

out VS_OUT
{
	vec2 TextureCoordinates;
} vs_out;


void main()
{	
	mat2 rotationMatrix;
	rotationMatrix[0].x = cos(-3.1415926 * 0.5f + direction);
	rotationMatrix[0].y = sin(-3.1415926 * 0.5f + direction);
	rotationMatrix[1].x = -sin(-3.1415926 * 0.5f + direction);
	rotationMatrix[1].y = cos(-3.1415926 * 0.5f + direction);

	vec2 pos = rotationMatrix * Position.xy;

	vs_out.TextureCoordinates = vec2(TextureCoordinates.x,1-TextureCoordinates.y);
	gl_Position = viewProjectionMatrix * vec4(pos, 0, 1);
}