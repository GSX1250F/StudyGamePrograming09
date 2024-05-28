#include "SplineCamera.h"
#include "Actor.h"

// スプラインカメラコンポーネント
Vector3 Spline::Compute(size_t startIdx, float t) const
{
	// startIdxが境界外？
	if (startIdx >= mControlPoints.size())
	{
		return mControlPoints.back();
	}
	else if (startIdx == 0)
	{
		return mControlPoints[startIdx];
	}
	else if (startIdx + 2 >= mControlPoints.size())
	{
		return mControlPoints[startIdx];
	}

	// p0からp3までの制御店を取得
	Vector3 p0 = mControlPoints[startIdx - 1];
	Vector3 p1 = mControlPoints[startIdx];
	Vector3 p2 = mControlPoints[startIdx + 1];
	Vector3 p3 = mControlPoints[startIdx + 2];
	// Catmull-Romの方程式によって位置を計算する
	Vector3 position = 0.5f * ((2.0f * p1) + (-1.0f * p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t * t +
		(-1.0f * p0 + 3.0f * p1 - 3.0f * p2 + p3) * t * t * t);
	return position;
}

SplineCamera::SplineCamera(Actor* owner):CameraComponent(owner)
	, mIndex(1)
	, mT(0.0f)
	, mSpeed(0.5f)
	, mPaused(true)
{
}

void SplineCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
	// tの値を更新
	if (!mPaused)
	{
		mT += mSpeed * deltaTime;
		// 必要ならば次の制御点に進む。ただし、スピードが速すぎて１フレームに複数の制御点を飛び越さないことが前提
		if (mT >= 1.0f)
		{
			// まだ経路を進むのに十分な数の点があるか
			if (mIndex < mPath.GetNumPoints() - 3)
			{
				mIndex++;
				mT = mT - 1.0f;
			}
			else
			{
				// 経路をたどり終わったので停止する
				mPaused = true;
			}
		}
	}

	// カメラの位置を、現在のインデックスとtから求める。
	Vector3 cameraPos = mPath.Compute(mIndex, mT);
	// 注視点はわずかなデルタだけ先の位置
	Vector3 target = mPath.Compute(mIndex, mT + 0.01f);
	// スプラインを上下逆にしないことを前提とする。
	const Vector3 up = Vector3::UnitZ;
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}

void SplineCamera::Restart()
{
	mIndex = 1;
	mT = 0.0f;
	mPaused = false;
}
