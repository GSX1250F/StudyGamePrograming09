#include "VertexArray.h"
#include <glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts)
	, mNumIndices(numIndices)
{
	// ���_�z��I�u�W�F�N�g���쐬���A����ID�������o�[�ϐ�mVertexArray�ɕۑ�����
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	// ���_�z��I�u�W�F�N�g���ł�����A���_�o�b�t�@���쐬�ł���
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(
		GL_ARRAY_BUFFER,	// �o�b�t�@�̎��
		numVerts * 8 * sizeof(float),	//�R�s�[����o�C�g��
		verts,	//�R�s�[���i�|�C���^�j
		GL_STATIC_DRAW	//���̃f�[�^�̗��p���@
	);

	// �C���f�b�N�X�o�b�t�@�����B
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,		//�C���f�b�N�X�o�b�t�@�̎w��
		numIndices * sizeof(unsigned int),		//�f�[�^�̃T�C�Y
		indices, GL_STATIC_DRAW);

	// ���_�������w�肷��B
	// ����̃��C�A�E�g�́A�ʒu���W�Ƃ��ĂR��float�l���g���B
	// �ʒu���W:3��float
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,						// �����C���f�b�N�X�i�P�ڂ̓[���j
		3,						// �v�f���i�����ł�3�j
		GL_FLOAT,				// �v�f�̌^
		GL_FALSE,				// �����^�̂ݎg�p����B
		sizeof(float) * 8,		// �V�����X�g���C�h��float8��
		0						// ���_�f�[�^�̊J�n�ʒu���炱�̑����܂ł̃I�t�Z�b�g
	);
	// �@���x�N�g��:3��float
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,						// ���_�����C���f�b�N�X
		3,						// �����̐�
		GL_FLOAT,				// �e�����̌^
		GL_FALSE,				// GL_FLOAT�ɂ͎g��Ȃ�
		sizeof(float) * 8,		// �X�g���C�h�i�ʏ�͊e���_�̃T�C�Y�j
		reinterpret_cast<void*>(sizeof(float) * 3)		// �I�t�Z�b�g�|�C���^
	);
	// �e�N�X�`�����W:2��float
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,						// ���_�����C���f�b�N�X
		2,						// �����̐�
		GL_FLOAT,				// �e�����̌^
		GL_FALSE,				// GL_FLOAT�ɂ͎g��Ȃ�
		sizeof(float) * 8,		// �X�g���C�h�i�ʏ�͊e���_�̃T�C�Y�j
		reinterpret_cast<void*>(sizeof(float) * 6)		// �I�t�Z�b�g�|�C���^
	);

}

VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}
