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
	
	// Practice�@�Ǐ]�J�����ɋO���J�����@�\��ǉ�����B
	// FollowActor��angularSpeed�����[��]�Ƃ��ăJ�����ɑ���B
	// FollowActor�ɋO���J�����̃}�E�X�@�\��ǉ�����B
	// mOffset�x�N�g�������[�����ŉ�]������
	Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
	mOffset = Vector3::Transform(mOffset, yaw);
	mUp = Vector3::Transform(mUp, yaw);
	// �J�����̑O�E�E�����������v�Z
	Vector3 forward = -1.0f * mOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();
	// mOffset�x�N�g�����s�b�`�����ŉ�]������
	Quaternion pitch(right, mPitchSpeed * deltaTime);
	mOffset = Vector3::Transform(mOffset, pitch);
	// �J�����̏�������v�Z
	mUp = Vector3::Transform(mUp, pitch);

	// �����܂łŃJ�����ʒu��ݒ肵����O�����Ǐ]�J�����̊�{�@�\�͔����B
	// ���ɗ��z�ʒu��ݒ肷��B���z�ʒu��mHorzDelta�����A�N�^�[��forward�����Ɉʒu�����炷
	// �ϊ��s��
	Vector3 idealPos = mOwner->GetPosition() + mOffset + mOwner->GetForward() * mHorzDelta;
	
	// �Ō�ɃX�v�����O�@�\�������B
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
	// �Ǐ]�J�����̈ʒu���A���L�A�N�^�[�̏����ɃZ�b�g
	Vector3 cameraPos = mOwner->GetPosition() - mOwner->GetForward() * mHorzDist + Vector3::UnitZ * mVertDist;
	return cameraPos;
}
