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
};

layout(std430, binding = 0) buffer GuardSSBO
{
	SGuard guard[100];
} guard;


out VS_OUT
{
	vec2 TextureCoordinates;
	float WalkAnimation;
} vs_out;


void main()
{	
	vs_out.TextureCoordinates = vec2(TextureCoordinate.x,1-TextureCoordinate.y);
	vs_out.WalkAnimation = guard.guard[gl_InstanceID].WalkAnimation;

	vec2 GuardPosition = guard.guard[gl_InstanceID].Position;
	float direction = guard.guard[gl_InstanceID].Direction;
	
	mat2 rotationMatrix;
	rotationMatrix[0].x = cos(-3.1415926 * 0.5f + direction);
	rotationMatrix[0].y = sin(-3.1415926 * 0.5f + direction);
	rotationMatrix[1].x = -sin(-3.1415926 * 0.5f + direction);
	rotationMatrix[1].y = cos(-3.1415926 * 0.5f + direction);

	vec2 pos = rotationMatrix * Position.xy;

	gl_Position = viewProjectionMatrix * vec4(pos + GuardPosition,0,1);
}