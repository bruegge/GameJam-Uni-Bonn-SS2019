#version 430

uniform sampler2DArray Texture;

in VS_OUT
{
	vec2 TextureCoordinates;
	float NoiseLevel;
} fs_in;

out vec4 ColorOut;

void main()
{   
	uint nTextureLevel = fs_in.NoiseLevel > fs_in.TextureCoordinates.x ? 1 : 0;

	ColorOut = vec4(texture(Texture, vec3(fs_in.TextureCoordinates,nTextureLevel)).rgb, 1);
	//ColorOut = vec4(fs_in.TextureCoordinates, 0, 1);
}