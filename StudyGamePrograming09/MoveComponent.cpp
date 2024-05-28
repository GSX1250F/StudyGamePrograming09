#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner),
	mForwardSpeed(0.0f),				// 前進移動速度
	mStrafeSpeed(0.0f),					// 横進移動速度
	mRotSpeed(0.0f),					// 回転速度
	mMass(1.0f),						// 質量
	mForwardForce(0.0f),				// 前進方向の重心にかかる力
	mStrafeForce(0.0f),					// 横進方向の重心にかかる力
	mForwardAccel(0.0f),				// 前進方向の重心加速度	=重心にかかる力 / 質量
	mStrafeAccel(0.0f),					// 横進方向の重心加速度	=重心にかかる力 / 質量
	mRotForce(0.0f),	// 回転方向の力F +方向はCCW
	mRotAccel(0.0f),	// 回転加速度
	mMoveResist(0.0f),					// 重心速度抵抗率(%)
	mRotResist(0.0f),					// 回転速度抵抗率(%)
	mTorque(0.0f),		// トルク=回転方向の力 * 半径 = 慣性モーメント * 回転加速度
	mImoment(0.0f)						// 慣性モーメント
{
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update(float deltatime)
{
	// Actorの位置と方向を更新
	if (!Math::NearZero(mForwardSpeed)) 
	{
		mOwner->SetPosition(mOwner->GetPosition() + mForwardSpeed * mOwner->GetForward() * deltatime);		//前進　x = xo + vt
	}
	if (!Math::NearZero(mStrafeSpeed))
	{
		mOwner->SetPosition(mOwner->GetPosition() + mStrafeSpeed * mOwner->GetStrafe() * deltatime);		//前進　x = xo + vt
	}
	if (!Math::NearZero(mRotSpeed))
	{
		// Temporary +Z軸周りの回転のみを行う。
		Quaternion rot = mOwner->GetRotation();
		float angle = mRotSpeed * deltatime;
		// 回転を追加させるクォータニオンを作成
		Quaternion inc(Vector3::UnitZ, angle);
		// もとのrotと増分のクォータニオンを結合
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}
	
	// 速度を更新
	if (!Math::NearZero(mMass))
	{
		//重心加速度の計算　F=ma  a=F*(1/m)
		mForwardAccel = mForwardForce * (1.0f / mMass);
		mStrafeAccel = mStrafeForce * (1.0f / mMass);
		//抵抗力 = 速さ*抵抗係数    減速 = -速さ*抵抗係数/質量
		float movedecel = Math::Sqrt(mForwardSpeed * mForwardSpeed + mStrafeSpeed * mStrafeSpeed)* mMoveResist * 0.01f * (1 / mMass);
		mForwardAccel -= movedecel * mOwner->GetForward().Length();
		mStrafeAccel -= movedecel * mOwner->GetStrafe().Length();
	}
	else { mForwardAccel = 0.0f; mStrafeAccel = 0.0f; }
	// Temporary +Z軸周りの回転のみを行う。
	// 慣性モーメント計算	 ※3次元においては、一様密度の球とする。 I=0.4*質量*半径^2
	mImoment = 0.4f * mMass * mOwner->GetRadius() * mOwner->GetRadius();
	if (!Math::NearZero(mImoment))
	{
		// トルク計算　　トルク=回転方向の力 * 半径
		mTorque = mRotForce * mOwner->GetRadius();
		// 回転加速度の計算　回転加速度 = トルク / 慣性モーメント
		mRotAccel = mTorque / mImoment;		//回転加速度の計算 Fr=Ia  a=Fr/I
		//抵抗力 = 速さ*抵抗係数    減速 = -速さ*抵抗係数*半径/慣性モーメント
		float rotdecel = mRotSpeed * mOwner->GetRadius() * mRotResist / mImoment;
		mRotAccel -= rotdecel;
	}
	else { mRotAccel = 0.0f; }

	mForwardSpeed += mForwardAccel * deltatime;		//v = vo + at
	mStrafeSpeed += mStrafeAccel * deltatime;
	mRotSpeed += mRotAccel * deltatime;		//ω = ωo + bt
}
