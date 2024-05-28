#pragma once
#include "CameraComponent.h"

//�Ǐ]�J�����R���|�[�l���g
class FollowCamera : public CameraComponent
{
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }
private:
	Vector3 ComputeCameraPos() const;

	// �Ǐ]�J�����̎��ۂ̈ʒu
	Vector3 mActualPos;
	// �Ǐ]�J�����̑��x
	Vector3 mVelocity;
	// �Ǐ]�A�N�^�[(FollowActor)�Ƃ̐�������
	float mHorzDist;
	// �Ǐ]�A�N�^�[(FollowActor)�Ƃ̐�������
	float mVertDist;
	// �����_�ƒǏ]�A�N�^�[(FollowActor)�Ƃ̐�������
	float mTargetDist;
	// �X�v�����O�@�\�̂΂˒萔
	float mSpringConstant;
};
