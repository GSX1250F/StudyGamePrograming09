#include "OrbitCamera.h"
#include "Actor.h"

// 軌道カメラコンポーネント。アクターの周りを回る。
OrbitCamera::OrbitCamera(Actor* owner):CameraComponent(owner)
	, mOffset(-400.0f, 0.0f, 0.0f)
	, mUp(Vector3::UnitZ)
	, mPitchSpeed(0.0f)
	, mYawSpeed(0.0f)
{
}

void OrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// ワールド上方を軸とするヨーのクォータニオンを作成
	Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
	// カメラのオフセットと上方ベクトルをヨーで変換
	mOffset = Vector3::Transform(mOffset, yaw);
	mUp = Vector3::Transform(mUp, yaw);

	// カメラの前方・右方を計算
	// 前方は owner.position - (owner.position + offset) = -offset
	Vector3 forward = -1.0f * mOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();

	// カメラ右方を軸とするピッチにクォータニオンを作成
	Quaternion pitch(right, mPitchSpeed * deltaTime);
	// カメラのオフセットと上方ベクトルをピッチで変換
	mOffset = Vector3::Transform(mOffset, pitch);
	mUp = Vector3::Transform(mUp, pitch);

	// 変換行列
	Vector3 target = mOwner->GetPosition();
	Vector3 cameraPos = target + mOffset;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, mUp);
	SetViewMatrix(view);
}
