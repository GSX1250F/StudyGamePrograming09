#pragma once
#include "CameraComponent.h"
#include <vector>

// スプラインカメラコンポーネント
struct Spline
{
	// スプラインのための制御点
	// (曲線に属する点がn個ならばn+2個の点が必要)
	std::vector<Vector3> mControlPoints;
	// startIdx=P1の区間でtの値に基づいて位置を計算する
	Vector3 Compute(size_t startIdx, float t) const;
	size_t GetNumPoints() const { return mControlPoints.size(); }
};

class SplineCamera : public CameraComponent
{
public:
	SplineCamera(class Actor* owner);

	void Update(float deltaTime) override;
	
	void Restart();

	void SetSpeed(float speed) { mSpeed = speed; }
	void SetSpline(const Spline& spline) { mPath = spline; }
	void SetPaused(bool pause) { mPaused = pause; }
private:
	// カメラがたどるスプライン軌道
	Spline mPath;
	// 現在の制御点とtの値
	size_t mIndex;
	float mT;
	// tの値の速度
	float mSpeed;
	// 軌道をカメラがたどれるかどうか
	bool mPaused;
};
