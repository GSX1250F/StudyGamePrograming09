#include "OrbitCamera.h"
#include "Actor.h"

// �O���J�����R���|�[�l���g�B�A�N�^�[�̎�������B
OrbitCamera::OrbitCamera(Actor* owner):CameraComponent(owner)
	, mOffset(-400.0f, 0.0f, 0.0f)
	, mUp(Vector3::UnitZ)
	, mPitchSpeed(0.0f)
	, mYawSpeed(0.0f)
{
}

void OrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// ���[���h��������Ƃ��郈�[�̃N�H�[�^�j�I�����쐬
	Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
	// �J�����̃I�t�Z�b�g�Ə���x�N�g�������[�ŕϊ�
	mOffset = Vector3::Transform(mOffset, yaw);
	mUp = Vector3::Transform(mUp, yaw);

	// �J�����̑O���E�E�����v�Z
	// �O���� owner.position - (owner.position + offset) = -offset
	Vector3 forward = -1.0f * mOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();

	// �J�����E�������Ƃ���s�b�`�ɃN�H�[�^�j�I�����쐬
	Quaternion pitch(right, mPitchSpeed * deltaTime);
	// �J�����̃I�t�Z�b�g�Ə���x�N�g�����s�b�`�ŕϊ�
	mOffset = Vector3::Transform(mOffset, pitch);
	mUp = Vector3::Transform(mUp, pitch);

	// �ϊ��s��
	Vector3 target = mOwner->GetPosition();
	Vector3 cameraPos = target + mOffset;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, mUp);
	SetViewMatrix(view);
}
