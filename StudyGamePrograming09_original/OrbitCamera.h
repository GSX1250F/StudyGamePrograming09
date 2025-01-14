#pragma once
#include "CameraComponent.h"

// 軌道カメラコンポーネント。アクターの周りを回る。
class OrbitCamera : public CameraComponent
{
public:
	OrbitCamera(class Actor* owner);

	void Update(float deltaTime) override;

	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetYawSpeed() const { return mYawSpeed; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
private:
	// ターゲット=所有アクターとのオフセットベクトル
	Vector3 mOffset;
	// カメラの上方向ベクトル。ピッチとヨーの関係によっては上下が逆になる場合がある。カメラの上方向を更新する。
	Vector3 mUp;
	// ピッチの角速度
	float mPitchSpeed;
	// ヨーの角速度
	float mYawSpeed;
};
