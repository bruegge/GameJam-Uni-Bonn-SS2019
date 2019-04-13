#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoordinate;

uniform mat4 viewProjectionMatrix;
uniform vec2 playerPosition;

layout(std430, binding = 0) buffer MapSSBO
{
	uint map[10000];
} map;


out VS_OUT
{
	vec2 TextureCoordinates;
	flat uint Instance;
	flat uint IsPlayerPosition;
} vs_out;


void main()
{	
	ivec2 MapCoordinate;

	MapCoordinate.x = gl_InstanceID % 100;
	MapCoordinate.y = gl_InstanceID / 100;

	if(MapCoordinate.x == uint(playerPosition.x+0.5f) && MapCoordinate.y == uint(playerPosition.y+0.5f))
	{
		vs_out.IsPlayerPosition = 1;
	}
	else
	{
		vs_out.IsPlayerPosition = 0;
	}

	//vs_out.Instance = map.map[gl_InstanceID];	
	//vs_out.TextureCoordinates = TextureCoordinates;

	vs_out.Instance = map.map[gl_InstanceID];	
	vs_out.TextureCoordinates = TextureCoordinate;



	gl_Position = viewProjectionMatrix * vec4(Position.xy + MapCoordinate,0,1);
}