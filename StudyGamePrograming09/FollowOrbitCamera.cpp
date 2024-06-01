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
	
	// Practice
	

}

void FollowOrbitCamera::SnapToIdeal()
{
	// 実際位置を理想位置にする。
	mActualPos = ComputeCameraPos();
	// 速度をリセット
	mVelocity = Vector3::Zero;
	// 注視ターゲットをセット
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
	// ビュー行列を作成。上方ベクトルはz軸の基本ベクトルのまま。
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

Vector3 FollowOrbitCamera::ComputeCameraPos() const
{
	// 追従カメラの位置を、所有アクターの上後方にセット
	Vector3 cameraPos = mOwner->GetPosition() - mOwner->GetForward() * mHorzDist + Vector3::UnitZ * mVertDist;
	return cameraPos;
}
