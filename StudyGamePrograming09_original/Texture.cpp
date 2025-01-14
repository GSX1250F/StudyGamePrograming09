#include "Texture.h"
#include <SDL.h>
#include <glew.h>
#include <SOIL.h>

Texture::Texture()
	:mTextureID(0)
	, mWidth(0)
	, mHeight(0)
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::string& fileName)
{
	int channels = 0;

	unsigned char* image = SOIL_load_image(
		fileName.c_str(),			// �t�@�C����
		&mWidth,					// �����L�^�����
		&mHeight,					// �������L�^�����
		&channels,					// �`���l�������L�^�����
		SOIL_LOAD_AUTO				// �摜�t�@�C���̎�ށi�܂���auto�j
	);

	if (image == nullptr)
	{
		SDL_Log("SOIL���摜 %s �̃��[�h�Ɏ��s���܂���: %s", fileName.c_str(), SOIL_last_result());
		return false;
	}

	int format = GL_RGB;
	if (channels == 4)
	{
		format = GL_RGBA;
	}

	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	glTexImage2D(
		GL_TEXTURE_2D,				// �e�N�X�`���^�[�Q�b�g
		0,							// Level of Detail:�ڍ׃��x���i����0�Ƃ���j
		format,						// OpenGL���g���ׂ��J���[�t�H�[�}�b�g
		mWidth,						// �e�N�X�`���̕�
		mHeight,					// �e�N�X�`���̍���
		0,							// ���E�F�i���̒l��0�ɂ���j
		format,						// ���̓f�[�^�̃J���[�t�H�[�}�b�g
		GL_UNSIGNED_BYTE,			// ���̓f�[�^�̃r�b�g�[�x�Bunsigned byte�łW�r�b�g�`���l�����w��
		image						// �摜�f�[�^�ւ̃|�C���^
	);

	SOIL_free_image_data(image);

	// �o�C���j�A�t�B���^�����O��L����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return true;
}

void Texture::Unload()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
