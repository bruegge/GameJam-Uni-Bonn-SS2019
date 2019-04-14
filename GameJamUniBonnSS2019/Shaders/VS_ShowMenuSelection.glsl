#version 430 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TextureCoordinate;

uniform uint SelectionPosition;

out VS_OUT
{
	vec2 TextureCoordinates;
} vs_out;


void main()
{	
	vs_out.TextureCoordinates = vec2(TextureCoordinate.x,TextureCoordinate.y);
	vec2 pos = Position.xy * vec2(1, 0.2f) + vec2(0,SelectionPosition * -0.27f + 0.4f);
	gl_Position = vec4(pos,0,1);
}