#pragma once
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void SetActive();

	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

private:
	// ���̃e�N�X�`����OpenGL ID
	unsigned int mTextureID;

	// �e�N�X�`���̕��ƍ���
	int mTexWidth;
	int mTexHeight;
};