#include "FPSActor.h"
#include "MoveComponent.h"
#include "SDL_scancode.h"
#include "Renderer.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"
#include "FPSCamera.h"
#include "MeshComponent.h"
#include "Mesh.h"

// ��l�̃A�N�^�[
FPSActor::FPSActor(Game* game):Actor(game)
{
	mMoveComp = new MoveComponent(this);
	mAudioComp = new AudioComponent(this);
	mLastFootstep = 0.0f;
	mFootstep = mAudioComp->PlayEvent("event:/Footstep");
	mFootstep.SetPaused(true);

	mCameraComp = new FPSCamera(this);

	mFPSModel = new Actor(game);
	mFPSModel->SetScale(0.75f);
	mMeshComp = new MeshComponent(mFPSModel);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/Rifle.gpmesh"));
}

void FPSActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// �������Ƃ����������Ă�
	mLastFootstep -= deltaTime;
	if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}

	// �A�N�^�[�̈ʒu�ɉ�����FPS���f���̈ʒu���X�V
	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetStrafe() * modelOffset.y;
	modelPos.z += modelOffset.z;
	mFPSModel->SetPosition(modelPos);
	// �A�N�^�[�̌����ɏ�����
	Quaternion q = GetRotation();
	// �J�����̃s�b�`�ɂ���]
	q = Quaternion::Concatenate(q, Quaternion(GetStrafe(), mCameraComp->GetPitch()));
	mFPSModel->SetRotation(q);
}

void FPSActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float strafeSpeed = 0.0f;
	
	if (keys[SDL_SCANCODE_UP])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_DOWN])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_LEFT])
	{
		strafeSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_RIGHT])
	{
		strafeSpeed += 400.0f;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetStrafeSpeed(strafeSpeed);

	// SDL���瑊�Ή^�����擾
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	// �}�E�X�̓����́A�ʏ�-500����+500�͈̔͂Ƃ݂Ȃ�
	const int maxMouseSpeed = 500;
	// �ő�ړ��ʂɂ�����p���x
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		// ���悻[-1.0, 1.0]�͈̔͂ɕϊ�
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// �ő�ړ��ʂł̊p���x���|����
		angularSpeed *= maxAngularSpeed;
	}
	mMoveComp->SetRotSpeed(angularSpeed);

	// �s�b�`���v�Z
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		// ���悻[-1.0, 1.0]�͈̔͂Ɏ��߂�
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetFootstepSurface(float value)
{
	// �����~�߂āA������ύX
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}
