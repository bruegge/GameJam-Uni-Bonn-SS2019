#version 430

uniform sampler2DArray Texture;

in VS_OUT
{
	vec2 TextureCoordinates;
	float WalkAnimation;
} fs_in;

out vec4 ColorOut;

void main()
{   
	ColorOut = vec4(texture(Texture, vec3(fs_in.TextureCoordinates,uint(fs_in.WalkAnimation))));
}