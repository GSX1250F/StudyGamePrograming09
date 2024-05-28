#include "SpriteComponent.h"
#include "Texture.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"


SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner),
	mTexture(nullptr),
	mDrawOrder(drawOrder),
	mTexWidth(0),
	mTexHeight(0)
{
	mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

/*
void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	//------------------�ǉ������ӏ�------------------------
	if (mTexture && mOwner->GetState() != mOwner->EPaused)
	{
		SDL_Rect r;
		// �����ƕ������L�A�N�^�[�̃X�P�[���ɍ��킹��
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -Math::ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
	}


}
*/

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture && mOwner->GetState() != mOwner->EPaused)	// EPaused�̂Ƃ���Draw���Ȃ��悤�ɏC��
	{
		// �e�N�X�`���̕��ƍ����ŒZ�a���X�P�[�����O
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(mTexWidth),
			static_cast<float>(mTexHeight),
			1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		// ���[���h�ϊ��̐ݒ�
		shader->SetMatrixUniform("uWorldTransform", world);
		// ���݂̃e�N�X�`�����Z�b�g
		mTexture->SetActive();

		// �Z�a��`��
		glDrawElements(
			GL_TRIANGLES,		// �`�悷��|���S���^�v���~�e�B�u�̎��
			6,					// �C���f�b�N�X�o�b�t�@�ɂ���C���f�b�N�X�̐�
			GL_UNSIGNED_INT,	// �C���f�b�N�X�̌^
			nullptr				// �ʏ��nullptr
		);
	}
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	// Set width/height
	mTexWidth = texture->GetWidth();
	mTexHeight = texture->GetHeight();
	// Actor�̔��a���Awidth��height�̕���/2�ɂ���B
	mOwner->SetRadius((mTexWidth + mTexHeight) * 0.25f);
}