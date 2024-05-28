#include "OrbitActor.h"
#include "MeshComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "OrbitCamera.h"
#include "MoveComponent.h"
#include "Mesh.h"

// 軌道カメラ用アクター
OrbitActor::OrbitActor(Game* game):Actor(game)
{
	mMeshComp = new MeshComponent(this);
	mMeshComp->SetMesh(game->GetRenderer()->GetMesh("Assets/RacingCar.gpmesh"));
	SetPosition(Vector3(0.0f, 0.0f, -100.0f));

	mCameraComp = new OrbitCamera(this);
}

void OrbitActor::ActorInput(const uint8_t* keys)
{
	// マウスで回転　SDLの相対モードを取得
	int x, y;
	Uint32 buttons = SDL_GetRelativeMouseState(&x, &y);
	// 右クリック保持中だけ回転
	if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		// マウス動作は基本 -500 から +500の範囲
		const int maxMouseSpeed = 500;
		// 回転速度の最大
		const float maxOrbitSpeed = Math::Pi * 8;
		float yawSpeed = 0.0f;
		if (x != 0)
		{
			// ヨーの角速度を[-1.0, 1.0]の範囲に収める
			yawSpeed = static_cast<float>(x) / maxMouseSpeed;
			// ヨーの角速度に掛ける
			yawSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetYawSpeed(-yawSpeed);

		// ピッチを計算
		float pitchSpeed = 0.0f;
		if (y != 0)
		{
			// ピッチの角速度を[-1.0, 1.0]の範囲に収める
			pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
			// ピッチの角速度に掛ける
			pitchSpeed *= maxOrbitSpeed;
		}
		mCameraComp->SetPitchSpeed(pitchSpeed);
	}
}

void OrbitActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}
