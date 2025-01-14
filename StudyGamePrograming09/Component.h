#pragma once
#include <SDL.h>

class Component
{
public:
	// �R���X�g���N�^
	// updateOrder���������R���|�[�l���g�قǑ����X�V�����
	Component(class Actor* owner, int updateOrder = 100);

	// �f�X�g���N�^
	virtual ~Component();

	// �e�R���|�[�l���g���͏����i�I�[�o�[���C�h�\�j
	virtual void ProcessInput(const SDL_Event& event) {}

	// �e�R���|�[�l���g�X�V�i�I�[�o�[���C�h�\�j
	virtual void Update(float deltaTime) {}

	// �X�V���Q�b�^�[
	int GetUpdateOrder() const { return mUpdateOrder; }

	// ���[���h�ϊ��̒ʒm���󂯎��B
	virtual void OnUpdateWorldTransform() {}


protected:
	// ���L�A�N�^�[
	class Actor* mOwner;

	// �R���|�[�l���g�̍X�V����
	int mUpdateOrder;
};