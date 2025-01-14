#pragma once
#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	// Load/unload mesh
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	// ����Mesh�Ɋ��蓖�Ă�ꂽ���_�z����擾
	class VertexArray* GetVertexArray() { return mVertexArray; }
	// index����e�N�X�`�����擾
	class Texture* GetTexture(size_t index);
	// �V�F�[�_�[�����擾
	const std::string& GetShaderName() const { return mShaderName; }
	// �I�u�W�F�N�g��Ԃł̋��E���̔��a���擾
	float GetRadius() const { return mRadius; }
	// Get specular power of mesh
	float GetSpecPower() const { return mSpecPower; }
private:
	// Mesh�̃e�N�X�`���Q
	std::vector<class Texture*> mTextures;
	// Mesh�̒��_�z��
	class VertexArray* mVertexArray;
	// �V�F�[�_�[�̖��O
	std::string mShaderName;
	// �I�u�W�F�N�g��Ԃł̋��E���̔��a
	float mRadius;
	// Specular power of surface
	float mSpecPower;
};