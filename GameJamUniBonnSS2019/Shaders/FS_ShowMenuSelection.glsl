#version 430

uniform sampler2D Texture;

in VS_OUT
{
	vec2 TextureCoordinates;
} fs_in;

out vec4 ColorOut;

void main()
{   
	ColorOut = vec4(texture(Texture, vec2(fs_in.TextureCoordinates.x - fs_in.TextureCoordinates.y /3.5f, fs_in.TextureCoordinates.y)));
	if(ColorOut == vec4(1,0,1,1))
	{
		discard;
	}
}