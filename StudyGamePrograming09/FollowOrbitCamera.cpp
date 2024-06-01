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
}

void FollowOrbitCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	
	// カメラのオフセットと上方ベクトルをヨーで変換
	Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
	mOffset = Vector3::Transform(mOffset, yaw);
	mUp = Vector3::Transform(mUp, yaw);
	Vector3 idealPos = mOwner->GetPosition() + mOffset;
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
	
	

	// FollowCamera
	// ばね定数から減衰係数を計算
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);	//減衰係数=2√k
	



	// 理想位置と実際の位置のずれ
	Vector3 diff = mActualPos - idealPos;
	// ばねによる加速度
	Vector3 acel = -mSpringConstant * diff - dampening * mVelocity;		// a = -k×Δd - 減衰係数×v
	// 速度を更新
	mVelocity += acel * deltaTime;
	// 実際のカメラ位置を更新
	mActualPos += mVelocity * deltaTime;
	// 注視ターゲットは所有アクターから前方にはなれた場所
	Vector3 target = mOwner->GetPosition() + mOwner->GetForward() * mTargetDist;
	// カメラの前方・右方を計算
	// 前方は owner.position - (owner.position + offset) = -offset
	Vector3 forward = -1.0f * mOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();

	// ビュー行列を作成。
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, mUp);
	SetViewMatrix(view);

	
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
	return Vector3::Zero;
}
