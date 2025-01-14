#pragma once
#include <vector>
#include "Math.h"
#include <SDL.h>

class Actor
{
public:
	enum State
	{
		EActive,		//�ғ���
		EPaused,		//�X�V��~��
		EDead			//�폜�Ώ�
	};

	Actor(class Game* game);
	virtual ~Actor();

	// �Q�[������Ăяo�����Process Input(�I�[�o�[���C�h�s��)
	void ProcessInput(const SDL_Event& event);
	// �A�N�^�[�Ǝ��̓��͏���(�I�[�o�[���C�h�\)
	virtual void ActorInput(const SDL_Event& event);

	// �Q�[������Ăяo�����X�V�֐�(�I�[�o�[���C�h�s��)
	void Update(float deltaTime);
	// �A�N�^�[�����S�R���|�[�l���g���X�V(�I�[�o�[���C�h�s��)
	void UpdateComponents(float deltaTime);
	// �A�N�^�[�Ǝ��̍X�V����(�I�[�o�[���C�h�\)
	virtual void UpdateActor(float deltaTime);
	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	class Game* GetGame() { return mGame; }
	// ���[���h�ϊ�
	void ComputeWorldTransform();
	Matrix4 GetWorldTransform() { return mWorldTransform; }

	// �S���W�n
	State GetState() const { return mState; }	// ���	
	void SetState(State state) { mState = state; }
	float GetRadius() const { return mRadius * mScale; }	// ���a
	void SetRadius(float radius) { mRadius = radius; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }		// �g�嗦�i���ʂ͕ς��Ȃ��j	
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }

	// ���[���h���W�n
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }
	Vector3 GetRightward() const { return Vector3::Transform(Vector3::UnitY, mRotation); }
	Vector3 GetUpward() const { return Vector3::Transform(Vector3::UnitZ, mRotation); }

private:
	class Game* mGame;
	std::vector<class Component*> mComponents;
	Matrix4 mWorldTransform;
	bool mRecomputeWorldTransform;

	// �S���W�n
	State mState;			//�A�N�^�[���
	float mScale;			//�g�嗦
	float mRadius;			//���a�i�g�嗦�͖����j
	// ���[���h���W�n
	Vector3 mPosition;		//��ʏ�̈ʒu
	Quaternion mRotation;	//��]�i���Ɗp�x�j
};