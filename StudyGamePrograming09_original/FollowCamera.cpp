#include "FollowCamera.h"
#include "Actor.h"

// 追従カメラのコンポーネント
FollowCamera::FollowCamera(Actor* owner) : CameraComponent(owner)
	, mHorzDist(350.0f)			
	, mVertDist(150.0f)			
	, mTargetDist(100.0f)		
	, mSpringConstant(64.0f)	
{
}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// ばね定数から減衰係数を計算
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);	//減衰係数=2√k
	// 理想位置
	Vector3 idealPos = ComputeCameraPos();
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
	// ビュー行列を作成。上方ベクトルはz軸の基本ベクトルのまま。
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target, Vector3::UnitZ);
	SetViewMatrix(view);
}

// 所有アクターFollowActorが初期化されるときに呼び出される。理想位置と実際位置をリセット。
void FollowCamera::SnapToIdeal()
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

Vector3 FollowCamera::ComputeCameraPos() const
{
	// 追従カメラの位置を、所有アクターの上後方にセット
	Vector3 cameraPos = mOwner->GetPosition() - mOwner->GetForward() * mHorzDist + Vector3::UnitZ * mVertDist;
	return cameraPos;
}
