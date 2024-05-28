#pragma once
#include "CameraComponent.h"

// �O���J�����R���|�[�l���g�B�A�N�^�[�̎�������B
class OrbitCamera : public CameraComponent
{
public:
	OrbitCamera(class Actor* owner);

	void Update(float deltaTime) override;

	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetYawSpeed() const { return mYawSpeed; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
private:
	// �^�[�Q�b�g=���L�A�N�^�[�Ƃ̃I�t�Z�b�g�x�N�g��
	Vector3 mOffset;
	// �J�����̏�����x�N�g���B�s�b�`�ƃ��[�̊֌W�ɂ���Ă͏㉺���t�ɂȂ�ꍇ������B�J�����̏�������X�V����B
	Vector3 mUp;
	// �s�b�`�̊p���x
	float mPitchSpeed;
	// ���[�̊p���x
	float mYawSpeed;
};
