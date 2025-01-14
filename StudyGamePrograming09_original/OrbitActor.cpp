#include "OrbitActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "OrbitCamera.h"
#include "MoveComponent.h"
#include "Mesh.h"

// �O���J�����p�A�N�^�[
OrbitActor::OrbitActor(Game* game):Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

	mCameraComp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const uint8_t* keys)
{
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

void OrbitActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}
