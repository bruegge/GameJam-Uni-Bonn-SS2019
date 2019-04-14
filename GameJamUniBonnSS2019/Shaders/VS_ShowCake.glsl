#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoordinate;

uniform mat4 viewProjectionMatrix;

struct SCake
{
	vec2 Position;
	float Available;
	float fill1;
};

layout(std430, binding = 0) buffer CakeSSBO
{
	SCake cake[100];
} cake;


out VS_OUT
{
	vec2 TextureCoordinates;
	float Available;
} vs_out;


void main()
{	
	vs_out.TextureCoordinates = vec2(TextureCoordinate.x,1-TextureCoordinate.y);
	vs_out.Available = cake.cake[gl_InstanceID].Available;
	vec2 CakePosition = cake.cake[gl_InstanceID].Position;
	
	gl_Position = viewProjectionMatrix * vec4(Position.xy + CakePosition,0,1);
}