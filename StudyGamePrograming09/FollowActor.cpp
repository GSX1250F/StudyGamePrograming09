#include "FollowActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
//#include "FollowCamera.h"
//practice
#include "FollowOrbitCamera.h"
#include "MoveComponent.h"
#include "Mesh.h"

//�Ǐ]�J�����p�̃A�N�^�[
FollowActor::FollowActor(Game* game):Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

	mMoveComp = new MoveComponent(this);
	//mCameraComp = new FollowCamera(this);
	//mCameraComp->SnapToIdeal();
	mCameraComp = new FollowOrbitCamera(this);
	mCameraComp->SnapToIdeal();
}

void FollowActor::ActorInput(const uint8_t* keys)
{
	float forwardSpeed = 0.0f;
	float angularSpeed = 0.0f;
	
	if (keys[SDL_SCANCODE_W])
	{
		forwardSpeed += 400.0f;
	}
	if (keys[SDL_SCANCODE_S])
	{
		forwardSpeed -= 400.0f;
	}
	if (keys[SDL_SCANCODE_A])
	{
		angularSpeed -= Math::Pi;
	}
	if (keys[SDL_SCANCODE_D])
	{
		angularSpeed += Math::Pi;
	}

	mMoveComp->SetForwardSpeed(forwardSpeed);
	mMoveComp->SetRotSpeed(angularSpeed);
	mCameraComp->SetYawSpeed(angularSpeed);


	// �J�����̐����������A�N�^�[�̃X�s�[�h�ɉ����ĕύX
	if (!Math::NearZero(forwardSpeed))
	{
		if (forwardSpeed > 0.0f)
		{
			mCameraComp->SetHorzDist(500.0f);
			mCameraComp->SetHorzDelta(-100.0f);
		}
		else
		{
			mCameraComp->SetHorzDist(500.0f);
			mCameraComp->SetHorzDelta(80.0f);
		}
	}
	else
	{
		mCameraComp->SetHorzDist(350.0f);
		mCameraComp->SetHorzDelta(0.0f);
	}



	// OrbitActor
	// �}�E�X�ŉ�]�@SDL�̑��΃��[�h���擾
	int x, y;
	Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
	// �E�N���b�N�ێ���������]
	if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		// �}�E�X����͊�{ -500 ���� +500�͈̔�
		const int maxMouseSpeed = 500;
		// ��]���x�̍ő�
		const float maxOrbitSpeed = Math::Pi * 8;
		float yawSpeed = 0.0f;
		if (x != 0)
		{
			// ���[�̊p���x��[-1.0, 1.0]�͈̔͂Ɏ��߂�
			yawSpeed = static_cast<float>(x) / maxMouseSpeed;
			// ���[�̊p���x�Ɋ|����
			yawSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetYawSpeed(-yawSpeed);

		// �s�b�`���v�Z
		float pitchSpeed = 0.0f;
		if (y != 0)
		{
			// �s�b�`�̊p���x��[-1.0, 1.0]�͈̔͂Ɏ��߂�
			pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
			// �s�b�`�̊p���x�Ɋ|����
			pitchSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetPitchSpeed(pitchSpeed);
	}
}

void FollowActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}
