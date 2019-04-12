#include "Player.h"
#include "Texture.h"

CPlayer::CPlayer()
{
	m_vPosition = glm::vec2(10, 10);
	m_fAngle = 3;
	m_nTextureID = CTexture::LoadTexture2D("../Textures/geilerTyp.png");
	m_pShader = new CShader();
	m_pShader->CreateShaderProgram("../shaders/VS_ShowGeometry.glsl", nullptr, nullptr, nullptr, "../shaders/FS_ShowGeometry.glsl");
}

CPlayer::~CPlayer()
{
	delete m_pShader;
	glDeleteTextures(1, &m_nTextureID);
}

void CPlayer::Move(glm::vec2 direction)
{

}