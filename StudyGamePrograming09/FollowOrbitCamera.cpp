#include "FollowOrbitCamera.h"
#include "Actor.h"

FollowOrbitCamera::FollowOrbitCamera(Actor* owner) : CameraComponent(owner)
	, mHorzDist(350.0f)
	, mVertDist(150.0f)
	, mTargetDist(100.0f)
	, mSpringConstant(64.0f)
	, mUp(Vector3::UnitZ)
	, mPitchSpeed(0.0f)
	, mYawSpeed(0.0f)
{
	mOffset = Vector3(-mHorzDist, 0.0f, mVertDist);
	preActorForward = mOwner->GetForward();
}

void FollowOrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	
	// Practice
	

}

void FollowOrbitCamera::SnapToIdeal()
{
	// ���ۈʒu�𗝑z�ʒu�ɂ���B
	mActualPos = ComputeCameraPos();
	// ���x�����Z�b�g
	mVelocity = Vector3::Zero;
	// �����^�[�Q�b�g���Z�b�g
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
	// �r���[�s����쐬�B����x�N�g����z���̊�{�x�N�g���̂܂܁B
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

Vector3 FollowOrbitCamera::ComputeCameraPos() const
{
	// �Ǐ]�J�����̈ʒu���A���L�A�N�^�[�̏����ɃZ�b�g
	Vector3 cameraPos = mOwner->GetPosition() - mOwner->GetForward() * mHorzDist + Vector3::UnitZ * mVertDist;
	return cameraPos;
}
