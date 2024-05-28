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

// 一人称アクター
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

	// 動いたとき足音をたてる
	mLastFootstep -= deltaTime;
	if (!Math::NearZero(mMoveComp->GetForwardSpeed()) && mLastFootstep <= 0.0f)
	{
		mFootstep.SetPaused(false);
		mFootstep.Restart();
		mLastFootstep = 0.5f;
	}

	// アクターの位置に応じてFPSモデルの位置を更新
	const Vector3 modelOffset(Vector3(10.0f, 10.0f, -10.0f));
	Vector3 modelPos = GetPosition();
	modelPos += GetForward() * modelOffset.x;
	modelPos += GetStrafe() * modelOffset.y;
	modelPos.z += modelOffset.z;
	mFPSModel->SetPosition(modelPos);
	// アクターの向きに初期化
	Quaternion q = GetRotation();
	// カメラのピッチによる回転
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

	// SDLから相対運動を取得
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);
	// マウスの動きは、通常-500から+500の範囲とみなす
	const int maxMouseSpeed = 500;
	// 最大移動量における角速度
	const float maxAngularSpeed = Math::Pi * 8;
	float angularSpeed = 0.0f;
	if (x != 0)
	{
		// およそ[-1.0, 1.0]の範囲に変換
		angularSpeed = static_cast<float>(x) / maxMouseSpeed;
		// 最大移動量での角速度を掛ける
		angularSpeed *= maxAngularSpeed;
	}
	mMoveComp->SetRotSpeed(angularSpeed);

	// ピッチを計算
	const float maxPitchSpeed = Math::Pi * 8;
	float pitchSpeed = 0.0f;
	if (y != 0)
	{
		// およそ[-1.0, 1.0]の範囲に収める
		pitchSpeed = static_cast<float>(y) / maxMouseSpeed;
		pitchSpeed *= maxPitchSpeed;
	}
	mCameraComp->SetPitchSpeed(pitchSpeed);
}

void FPSActor::SetFootstepSurface(float value)
{
	// 音を止めて、足音を変更
	mFootstep.SetPaused(true);
	mFootstep.SetParameter("Surface", value);
}

void FPSActor::SetVisible(bool visible)
{
	mMeshComp->SetVisible(visible);
}
