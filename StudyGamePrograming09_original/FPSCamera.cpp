#include "FPSCamera.h"
#include "Actor.h"

// 一人称アクター(FPSActor)用のカメラコンポーネント
FPSCamera::FPSCamera(Actor* owner) : CameraComponent(owner)
	, mPitchSpeed(0.0f)
	, mMaxPitch(Math::Pi / 3.0f)
	, mPitch(0.0f)
{
}

void FPSCamera::Update(float deltaTime)
{
	// 親クラスのupdateを呼び出す
	CameraComponent::Update(deltaTime);
	// カメラの位置は所有アクターの位置
	Vector3 cameraPos = mOwner->GetPosition();

	// ピッチの角速度に基づいてピッチを更新
	mPitch += mPitchSpeed * deltaTime;
	// ピッチを [-max, +max]の範囲に収める。
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	// ピッチ回転を表すクォータニオンを作成。（オーナーの右向きベクトルを軸とする回転）
	Quaternion q(mOwner->GetStrafe(), mPitch);

	// ピッチのクォータニオンで、所有アクターの前方ベクトルを回転
	Vector3 viewForward = Vector3::Transform(mOwner->GetForward(), q);
	// ターゲットの位置は所有アクターの100単位前方とする
	Vector3 target = cameraPos + viewForward * 100.0f;
	// 上方ベクトルもピッチのクォータニオンで回転
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	// 注視行列を作って、ビューに設定する。
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}
