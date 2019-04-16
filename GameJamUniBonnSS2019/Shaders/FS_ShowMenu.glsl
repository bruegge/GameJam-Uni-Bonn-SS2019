#version 430

uniform sampler2DArray Texture;
uniform uint MenuState;

in VS_OUT
{
	vec2 TextureCoordinates;
} fs_in;

out vec4 ColorOut;

void main()
{   
	ColorOut = vec4(texture(Texture, vec3(fs_in.TextureCoordinates.x, fs_in.TextureCoordinates.y, MenuState)));
	//ColorOut = vec4(fs_in.TextureCoordinates, 0, 1);
}