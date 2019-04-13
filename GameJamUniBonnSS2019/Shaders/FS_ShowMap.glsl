#version 430

uniform sampler2DArray Texture;

in VS_OUT
{
	vec2 TextureCoordinates;
	flat uint Instance;
	flat uint IsPlayerPosition;
} fs_in;

out vec4 ColorOut;

void main()
{   
	ColorOut = vec4(texture(Texture, vec3(fs_in.TextureCoordinates,fs_in.Instance)).rgb,1);
	if(fs_in.IsPlayerPosition == 1)
	{
		ColorOut = vec4(1,0,1,1);
	}	
}