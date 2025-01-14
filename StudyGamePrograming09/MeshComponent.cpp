#include "MeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexInfo.h"

MeshComponent::MeshComponent(Actor* owner)
	:Component(owner)
	, mMesh(nullptr)
	, mTextureIndex(0)
	, mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader)
{
	if (mMesh && mVisible)
	{
		// ���[���h���W�ϊ��̐ݒ�
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());		
		// �e�N�X�`���̐ݒ�
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// ���ʔ��ˎw���̐ݒ� 
		float sp = mMesh->GetSpecPower();
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// ���b�V���̒��_���N���X���A�N�e�B�u�ɂ���B
		VertexInfo* vi = mMesh->GetVertexInfo();
		vi->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, vi->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}
