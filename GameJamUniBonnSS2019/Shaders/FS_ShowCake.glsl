#version 430

uniform sampler2D Texture;

in VS_OUT
{
	vec2 TextureCoordinates;
	float Available;
} fs_in;

out vec4 ColorOut;

void main()
{   
	if(fs_in.Available == 1)
	{
		ColorOut = vec4(texture(Texture, fs_in.TextureCoordinates));
	}
	else
	{
		discard;
	}
}