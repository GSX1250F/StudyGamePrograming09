#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	: Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
	, mVisible(true)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture && mVisible)
	{
		// �e�N�X�`���T�C�Y�ōăX�P�[�����O�������[���h�ϊ��s����쐬
		Matrix4 world = Matrix4::CreateScale(static_cast<float>(mTexWidth), static_cast<float>(mTexHeight), static_cast<float>(mTexWidth));
		world *= mOwner->GetWorldTransform();
		// ���[���h�ϊ�
		shader->SetMatrixUniform("uWorldTransform", world);
		// ���݂̃e�N�X�`�����Z�b�g
		mTexture->SetActive();
		// �Z�`��`��
		glDrawElements(
			GL_TRIANGLES,		// �`�悷��|���S���^�v���~�e�B�u�̎��
			mOwner->GetGame()->GetRenderer()->GetVertexInfo()->GetNumIndices(),		// �C���f�b�N�X�o�b�t�@�ɂ���C���f�b�N�X�̐�
			GL_UNSIGNED_INT,	// �C���f�b�N�X�̌^
			nullptr				// �ʏ��nullptr
		);
	}
}

void SpriteComponent::SetTexture(class Texture* texture)
{
	mTexture = texture;
	// �����ƕ���ݒ�
	mTexWidth = texture->GetTexWidth();
	mTexHeight = texture->GetTexHeight();
	// �����ƕ��̕��ς�Actor�̒��a�Ƃ���B
	mOwner->SetRadius((mTexWidth + mTexHeight) * 0.25f);
}