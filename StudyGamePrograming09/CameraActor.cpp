#include "CameraActor.h"
#include "MoveComponent.h"
#include "Renderer.h"
#include "Game.h"

CameraActor::CameraActor(Game* game)
	:Actor(game)
{
	SetPosition(Vector3(0.0f, 0.0f, 100.0f));
	mMoveComp = new MoveComponent(this);

	//スポットライト
	SpotLight sl;
	sl.mPosition = GetPosition();
	sl.mDirection = GetForward();
	Vector3 color = Vector3(1.0f, 1.0f, 1.0f);
	sl.mDiffuseColor = color;
	sl.mSpecColor = color;
	sl.mAttenuation = 0.9f;
	sl.mCornAngle = Math::Pi / 8.0f;
	sl.mFalloff = 200.0f;
	game->GetRenderer()->AddSpotLight(sl);
}

void CameraActor::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	// カメラの位置と方向を更新
	Vector3 cameraPos = GetPosition();
	Vector3 cameraTarget = GetPosition() + GetForward() * 100.0f;
	Vector3 cameraUp = Vector3::UnitZ;

	Matrix4 view = Matrix4::CreateLookAt(cameraPos, cameraTarget, cameraUp);
	GetGame()->GetRenderer()->SetViewMatrix(view);

	//スポットライトの位置と方向を更新
	SpotLight& sl = GetGame()->GetRenderer()->GetSpotLights()[0];
	sl.mPosition = GetPosition();
	sl.mDirection = GetForward();
}

void CameraActor::ActorInput(const SDL_Event& event)
{
	float forwardSpeed = 0.0f;
	float rotSpeed = 0.0f;
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_UP)
		{
			forwardSpeed += 500.0f;
		}
		if (event.key.keysym.sym == SDLK_DOWN)
		{
			forwardSpeed -= 500.0f;
		}
		if (event.key.keysym.sym == SDLK_LEFT)
		{
			rotSpeed -= Math::Pi;
		}
		if (event.key.keysym.sym == SDLK_RIGHT)
		{
			rotSpeed += Math::Pi;
		}
	}
	mMoveComp->SetVelocity(forwardSpeed * GetForward());
	mMoveComp->SetRotSpeed(rotSpeed * GetUpward());
}
