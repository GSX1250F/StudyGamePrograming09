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
}

void FollowOrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	
	// �J�����̃I�t�Z�b�g�Ə���x�N�g�������[�ŕϊ�
	Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
	mOffset = Vector3::Transform(mOffset, yaw);
	mUp = Vector3::Transform(mUp, yaw);
	Vector3 idealPos = mOwner->GetPosition() + mOffset;
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
	
	

	// FollowCamera
	// �΂˒萔���猸���W�����v�Z
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);	//�����W��=2��k
	



	// ���z�ʒu�Ǝ��ۂ̈ʒu�̂���
	Vector3 diff = mActualPos - idealPos;
	// �΂˂ɂ������x
	Vector3 acel = -mSpringConstant * diff - dampening * mVelocity;		// a = -k�~��d - �����W���~v
	// ���x���X�V
	mVelocity += acel * deltaTime;
	// ���ۂ̃J�����ʒu���X�V
	mActualPos += mVelocity * deltaTime;
	// �����^�[�Q�b�g�͏��L�A�N�^�[����O���ɂ͂Ȃꂽ�ꏊ
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
	// �J�����̑O���E�E�����v�Z
	// �O���� owner.position - (owner.position + offset) = -offset
	Vector3 forward = -1.0f * mOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();

	// �r���[�s����쐬�B
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, mUp);
	SetViewMatrix(view);

	
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
	return Vector3::Zero;
}
