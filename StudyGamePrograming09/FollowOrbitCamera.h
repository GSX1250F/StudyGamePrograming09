#pragma once
#include "CameraComponent.h"
class FollowOrbitCamera :public CameraComponent
{
public:
	FollowOrbitCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetYawSpeed() const { return mYawSpeed; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
private:
	Vector3 ComputeCameraPos() const;

	// 追従カメラの実際の位置
	Vector3 mActualPos;
	// 追従カメラの速度
	Vector3 mVelocity;
	// 追従アクター(FollowActor)との水平距離
	float mHorzDist;
	// 追従アクター(FollowActor)との垂直距離
	float mVertDist;
	// 注視点と追従アクター(FollowActor)との水平距離
	float mTargetDist;
	// スプリング機能のばね定数
	float mSpringConstant;
	// ターゲット=所有アクターとのオフセットベクトル
	Vector3 mOffset;
	// カメラの上方向ベクトル。ピッチとヨーの関係によっては上下が逆になる場合がある。カメラの上方向を更新する。
	Vector3 mUp;
	// ピッチの角速度
	float mPitchSpeed;
	// ヨーの角速度
	float mYawSpeed;
};

