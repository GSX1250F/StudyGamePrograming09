#pragma once
#include "CameraComponent.h"
#include <vector>

// �X�v���C���J�����R���|�[�l���g
struct Spline
{
	// �X�v���C���̂��߂̐���_
	// (�Ȑ��ɑ�����_��n�Ȃ��n+2�̓_���K�v)
	std::vector<Vector3> mControlPoints;
	// startIdx=P1�̋�Ԃ�t�̒l�Ɋ�Â��Ĉʒu���v�Z����
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
	// �J���������ǂ�X�v���C���O��
	Spline mPath;
	// ���݂̐���_��t�̒l
	size_t mIndex;
	float mT;
	// t�̒l�̑��x
	float mSpeed;
	// �O�����J���������ǂ�邩�ǂ���
	bool mPaused;
};
