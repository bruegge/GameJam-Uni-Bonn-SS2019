#version 430

uniform sampler2DArray Texture;
uniform uint nWalkTextureStep;

in VS_OUT
{
	vec2 TextureCoordinates;
} fs_in;

out vec4 ColorOut;

void main()
{   
	ColorOut = texture(Texture, vec3(fs_in.TextureCoordinates,nWalkTextureStep).rgb,1);
	//ColorOut = vec4(fs_in.TextureCoordinates,0,1);
}