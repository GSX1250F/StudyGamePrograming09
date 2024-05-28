#pragma once
#include "CameraComponent.h"

// 一人称アクター(FPSActor)用のカメラコンポーネント
class FPSCamera : public CameraComponent
{
public:
	FPSCamera(class Actor* owner);

	void Update(float deltaTime) override;

	float GetPitch() const { return mPitch; }
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetMaxPitch() const { return mMaxPitch; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }
private:
	// ピッチ（上下方向回転）の角速度
	float mPitchSpeed;
	// 最大ピッチ角度
	float mMaxPitch;
	// 現在のピッチ
	float mPitch;
};
