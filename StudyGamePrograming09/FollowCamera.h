#pragma once
#include "CameraComponent.h"

//追従カメラコンポーネント
class FollowCamera : public CameraComponent
{
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }
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
};
