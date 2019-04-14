#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoordinate;

out VS_OUT
{
	vec2 TextureCoordinates;
} vs_out;


void main()
{	
	vs_out.TextureCoordinates = vec2(TextureCoordinate.x,TextureCoordinate.y);
	vec2 pos = Position.xy * 1.9f;
	gl_Position = vec4(pos,0,1);
}