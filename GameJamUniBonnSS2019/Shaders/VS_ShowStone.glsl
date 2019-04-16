#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoordinate;

uniform mat4 viewProjectionMatrix;

struct SStone
{
	vec2 Position;
	vec2 Direction;
	float Active;
	float Speed;
	float fill1;
	float fill2;
};

layout(std430, binding = 0) buffer StoneSSBO
{
	SStone stone[100];
} stone;


out VS_OUT
{
	vec2 TextureCoordinates;
	float Available;
} vs_out;


void main()
{	
	vs_out.TextureCoordinates = vec2(TextureCoordinate.x,1-TextureCoordinate.y);
	vs_out.Available = stone.stone[gl_InstanceID].Active;
	vec2 stonePosition = stone.stone[gl_InstanceID].Position;
	
	gl_Position = viewProjectionMatrix * vec4(Position.xy * 0.5f + stonePosition,0,1);
}