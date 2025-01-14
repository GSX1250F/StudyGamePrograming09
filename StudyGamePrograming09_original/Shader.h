#pragma once
#include <string>
#include <glew.h>
#include "Math.h"

class Shader
{
public:
	Shader();
	~Shader();
	//�w�肳�ꂽ���O�̒��_/�t���O�����g�V�F�[�_�[��ǂݍ���
	bool Load(const std::string& vertName,
	          const std::string& fragName);
	void Unload();
	//�A�N�e�B�u�ȃV�F�[�_�[�v���O�����Ƃ��Đݒ�
	void SetActive();
	// �}�g���b�N�X���j�t�H�[����ݒ�
	void SetMatrixUniform(const char* name, const Matrix4& matrix);
	// Vector3���j�t�H�[����ݒ�
	void SetVectorUniform(const char* name, const Vector3& vector);
	// float���j�t�H�[����ݒ�
	void SetFloatUniform(const char* name, float value);

private:
	//�V�F�[�_�[���R���p�C������
	bool CompileShader(const std::string& fileName,
					   GLenum shaderType,
					   GLuint& outShader);
	//�V�F�[�_�[�̃R���p�C���ɐ����������̔���
	bool IsCompiled(GLuint shader);
	//���_/�t���O�����g�v���O�����̃����N���m�F
	bool IsValidProgram();
	//�V�F�[�_�[�I�u�W�F�N�g��ID���i�[
	GLuint mVertexShader;
	GLuint mFragShader;
	GLuint mShaderProgram;
};
