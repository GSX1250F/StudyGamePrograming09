#pragma once
#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	// ���̃��b�V���R���|�[�l���g�ŕ`��
	virtual void Draw(class Shader* shader);
	// ���b�V���E�e�N�X�`�������b�V���R���|�[�l���g�Ŏg�p����Ă���C���f�b�N�X�Őݒ�
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
	bool mVisible;
};
