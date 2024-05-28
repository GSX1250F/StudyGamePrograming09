#include "FollowCamera.h"
#include "Actor.h"

// �Ǐ]�J�����̃R���|�[�l���g
FollowCamera::FollowCamera(Actor* owner) : CameraComponent(owner)
	, mHorzDist(350.0f)			
	, mVertDist(150.0f)			
	, mTargetDist(100.0f)		
	, mSpringConstant(64.0f)	
{
}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// �΂˒萔���猸���W�����v�Z
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);	//�����W��=2��k
	// ���z�ʒu
	Vector3 idealPos = ComputeCameraPos();
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
	// �r���[�s����쐬�B����x�N�g����z���̊�{�x�N�g���̂܂܁B
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

// ���L�A�N�^�[FollowActor�������������Ƃ��ɌĂяo�����B���z�ʒu�Ǝ��ۈʒu�����Z�b�g�B
void FollowCamera::SnapToIdeal()
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

Vector3 FollowCamera::ComputeCameraPos() const
{
	// �Ǐ]�J�����̈ʒu���A���L�A�N�^�[�̏����ɃZ�b�g
	Vector3 cameraPos = mOwner->GetPosition() - mOwner->GetForward() * mHorzDist + Vector3::UnitZ * mVertDist;
	return cameraPos;
}
