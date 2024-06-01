#pragma once
#include "CameraComponent.h"
class FollowOrbitCamera :public CameraComponent
{
public:
	FollowOrbitCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetYawSpeed() const { return mYawSpeed; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
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
	// �^�[�Q�b�g=���L�A�N�^�[�Ƃ̃I�t�Z�b�g�x�N�g��
	Vector3 mOffset;
	// �J�����̏�����x�N�g���B�s�b�`�ƃ��[�̊֌W�ɂ���Ă͏㉺���t�ɂȂ�ꍇ������B�J�����̏�������X�V����B
	Vector3 mUp;
	// �s�b�`�̊p���x
	float mPitchSpeed;
	// ���[�̊p���x
	float mYawSpeed;
};

