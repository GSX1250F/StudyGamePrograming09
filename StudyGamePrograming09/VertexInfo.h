#pragma once
class VertexInfo
{
public:
	VertexInfo(const float* verts, unsigned int numVerts,
		       const unsigned int* indices,unsigned int numIndices);
	~VertexInfo();

	// ���̃o�[�e�b�N�X�z����A�N�e�B�u�ɂ���i�`��ł���悤�ɂ���j
	void SetActive();

	unsigned int GetNumVerts() const { return mNumVerts; }
	unsigned int GetNumIndices() const { return mNumIndices; }

private:
	unsigned int mVertexArray;		// �o�[�e�b�N�X�z��I�u�W�F�N�g��OpenGL ID
	unsigned int mNumVerts;			// ���_�̐�
	unsigned int mNumIndices;		// �C���f�b�N�X�̐�	
	unsigned int mVertexBuffer;		// ���_�o�b�t�@��OpenGL ID	
	unsigned int mIndexBuffer;		// �C���f�b�N�X�o�b�t�@��OpenGL ID	
};