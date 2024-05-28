#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

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

	// �Q�[������Ăяo�����X�V�֐�(�I�[�o�[���C�h�s��)
	void Update(float deltaTime);
	// �A�N�^�[�����S�R���|�[�l���g���X�V(�I�[�o�[���C�h�s��)
	void UpdateComponents(float deltaTime);
	// �A�N�^�[�Ǝ��̍X�V����(�I�[�o�[���C�h�\)
	virtual void UpdateActor(float deltaTime);
	// �Q�[������Ăяo�����Process Input(�I�[�o�[���C�h�s��)
	void ProcessInput(const uint8_t* keyState);
	// �A�N�^�[�Ǝ��̓��͏���(�I�[�o�[���C�h�\)
	virtual void ActorInput(const uint8_t* keyState);

	// �Q�b�^�[�E�Z�b�^�[
	// �ʒu
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true;}
	// �g�嗦�i���ʂ͕ς��Ȃ��B���a�ɉe������j
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true;}
	// ��]
	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeWorldTransform = true;}
	// ���a
	float GetRadius() { return mRadius * mScale; }	//�g�嗦���l��
	void SetRadius(float radius) { mRadius = radius; mRecomputeWorldTransform = true;}
	

	// ���[���h�ϊ�
	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	// �����̒P�ʃx�N�g��
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }		//�O���P�ʃx�N�g���@�O�����̎����N�H�[�^�j�I���ŉ�]
	Vector3 GetStrafe() const { return Vector3::Transform(Vector3::UnitY, mRotation); }			//�E���P�ʃx�N�g���@�E�����̎����N�H�[�^�j�I���ŉ�]

	// ���
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }


	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	// �A�N�^�[�̏��
	State mState;

	// �ړ�
	Vector3 mPosition;		//��ʏ�̈ʒu(�R�����j
	Quaternion mRotation;	//��]�i�N�H�[�^�j�I���j
	float mScale;			//�g�嗦
	float mRadius;			//���a�i�g�嗦�͖����j
	
	// �ϊ�
	Matrix4 mWorldTransform;
	bool mRecomputeWorldTransform;

	std::vector<class Component*> mComponents;
	class Game* mGame;
};