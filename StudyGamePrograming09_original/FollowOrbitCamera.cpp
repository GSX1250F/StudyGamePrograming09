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
	
	// Practice　追従カメラに軌道カメラ機能を追加する。
	// FollowActorのangularSpeedをヨー回転としてカメラに送る。
	// FollowActorに軌道カメラのマウス機能を追加する。
	// mOffsetベクトルをヨー方向で回転させる
	Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
	mOffset = Vector3::Transform(mOffset, yaw);
	mUp = Vector3::Transform(mUp, yaw);
	// カメラの前・右向き方向を計算
	Vector3 forward = -1.0f * mOffset;
	forward.Normalize();
	Vector3 right = Vector3::Cross(mUp, forward);
	right.Normalize();
	// mOffsetベクトルをピッチ方向で回転させる
	Quaternion pitch(right, mPitchSpeed * deltaTime);
	mOffset = Vector3::Transform(mOffset, pitch);
	// カメラの上向きを計算
	mUp = Vector3::Transform(mUp, pitch);

	// ここまででカメラ位置を設定したら軌道＆追従カメラの基本機能は備わる。
	// 次に理想位置を設定する。理想位置はmHorzDeltaだけアクターのforward方向に位置をずらす
	// 変換行列
	Vector3 idealPos = mOwner->GetPosition() + mOffset + mOwner->GetForward() * mHorzDelta;
	
	// 最後にスプリング機能を実装。
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
	// 追従カメラの位置を、所有アクターの上後方にセット
	Vector3 cameraPos = mOwner->GetPosition() - mOwner->GetForward() * mHorzDist + Vector3::UnitZ * mVertDist;
	return cameraPos;
}
