#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	//コンストラクタ。Update Orderは早め。
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltatime) override;

	// 各パラメータのセッター＆ゲッター
	// 速度
	float GetForwardSpeed() { return mForwardSpeed; }	// 前進
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	float GetStrafeSpeed() { return mStrafeSpeed; }		// 横進
	void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }
	float GetRotSpeed() const { return mRotSpeed; }		// 回転
	void SetRotSpeed(float speed) { mRotSpeed = speed; }

	// 古典物理に必要なパラメータのセッター＆ゲッター
	//前進方向にかかる力
	float GetFowardForce() { return mForwardForce; }
	void SetForwardForce(float force) { mForwardForce = force; }
	//横進方向にかかる力
	float GetStrafeForce() { return mStrafeForce; }
	void SetStrafeForce(float force) { mStrafeForce = force; }
	//回転方向の力
	float GetRotForce() { return mRotForce; }
	void SetRotForce(float force) { mRotForce = force; }
	//重心移動の抵抗率
	float GetMoveResist() { return mMoveResist; }
	void SetMoveResist(float resist) { mMoveResist = resist; }
	//回転移動の抵抗率
	float GetRotResist() { return mRotResist; }
	void SetRotResist(float resist) { mRotResist = resist; }
	// 質量
	float GetMass() { return mMass; }
	void SetMass(float mass) { mMass = mass; }
	// トルク
	float GetTorque() { return mTorque; }
	void SetTorque(float torque) { mTorque = torque; }
	// 慣性モーメント
	float GetImoment() { return mImoment; }
	void SetImoment(float imoment) { mImoment = imoment; }


private:
	// 移動パラメータ
	float mForwardSpeed;	// 前進移動速度
	float mStrafeSpeed;		// 横進移動速度
	float mRotSpeed;		// 回転速度

	// 古典物理パラメータ
	float mMass;			// 質量
	float mForwardForce;	// 前進方向の重心にかかる力
	float mStrafeForce;		// 横進方向の重心にかかる力
	float mRotForce;		// 回転方向の力F +方向はCCW  ※temporary　回転はz軸周りだけ
	float mTorque;			// トルク=回転方向の力 * 半径 = 慣性モーメント * 回転加速度  ※temporary　回転はz軸周りだけ
	float mForwardAccel;	// 前進方向の重心加速度	=重心にかかる力 / 質量 
	float mStrafeAccel;		// 横進方向の重心加速度	=重心にかかる力 / 質量 
	float mRotAccel;		// 回転加速度  ※temporary　回転はz軸周りだけ
	float mImoment;			// 慣性モーメント
	float mMoveResist;		// 重心速度抵抗率(%)
	float mRotResist;		// 回転速度抵抗率(%)
};

