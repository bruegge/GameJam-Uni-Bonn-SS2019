#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoordinate;

uniform mat4 viewProjectionMatrix;

struct SGuard
{
	vec2 Position;
	vec2 InitialPosition;
	vec2 PositionToGo;
	float Direction;
	float WalkAnimation;
	float NoiseLevel;
	float fill1;
	float fill2;
	float fill3;
};

layout(std430, binding = 0) buffer GuardSSBO
{
	SGuard guard[100];
} guard;


out VS_OUT
{
	vec2 TextureCoordinates;
	float NoiseLevel;
} vs_out;


void main()
{	
	vs_out.TextureCoordinates = vec2(TextureCoordinate.x,1-TextureCoordinate.y);
	vs_out.NoiseLevel = guard.guard[gl_InstanceID].NoiseLevel;

	vec2 GuardPosition = guard.guard[gl_InstanceID].Position;
	vec2 pos = Position.xy * vec2(1,0.2f)+ vec2(0,0.6f);

	gl_Position = viewProjectionMatrix * vec4(pos + GuardPosition,0,1);
}


