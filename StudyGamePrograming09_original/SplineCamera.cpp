#include "SplineCamera.h"
#include "Actor.h"

// �X�v���C���J�����R���|�[�l���g
Vector3 Spline::Compute(size_t startIdx, float t) const
{
	// startIdx�����E�O�H
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

	// p0����p3�܂ł̐���X���擾
	Vector3 p0 = mControlPoints[startIdx - 1];
	Vector3 p1 = mControlPoints[startIdx];
	Vector3 p2 = mControlPoints[startIdx + 1];
	Vector3 p3 = mControlPoints[startIdx + 2];
	// Catmull-Rom�̕������ɂ���Ĉʒu���v�Z����
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
	// t�̒l���X�V
	if (!mPaused)
	{
		mT += mSpeed * deltaTime;
		// �K�v�Ȃ�Ύ��̐���_�ɐi�ށB�������A�X�s�[�h���������ĂP�t���[���ɕ����̐���_���щz���Ȃ����Ƃ��O��
		if (mT >= 1.0f)
		{
			// �܂��o�H��i�ނ̂ɏ\���Ȑ��̓_�����邩
			if (mIndex < mPath.GetNumPoints() - 3)
			{
				mIndex++;
				mT = mT - 1.0f;
			}
			else
			{
				// �o�H�����ǂ�I������̂Œ�~����
				mPaused = true;
			}
		}
	}

	// �J�����̈ʒu���A���݂̃C���f�b�N�X��t���狁�߂�B
	Vector3 cameraPos = mPath.Compute(mIndex, mT);
	// �����_�͂킸���ȃf���^������̈ʒu
	Vector3 target = mPath.Compute(mIndex, mT + 0.01f);
	// �X�v���C�����㉺�t�ɂ��Ȃ����Ƃ�O��Ƃ���B
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
