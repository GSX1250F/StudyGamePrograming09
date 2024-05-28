#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner),
	mForwardSpeed(0.0f),				// �O�i�ړ����x
	mStrafeSpeed(0.0f),					// ���i�ړ����x
	mRotSpeed(0.0f),					// ��]���x
	mMass(1.0f),						// ����
	mForwardForce(0.0f),				// �O�i�����̏d�S�ɂ������
	mStrafeForce(0.0f),					// ���i�����̏d�S�ɂ������
	mForwardAccel(0.0f),				// �O�i�����̏d�S�����x	=�d�S�ɂ������ / ����
	mStrafeAccel(0.0f),					// ���i�����̏d�S�����x	=�d�S�ɂ������ / ����
	mRotForce(0.0f),	// ��]�����̗�F +������CCW
	mRotAccel(0.0f),	// ��]�����x
	mMoveResist(0.0f),					// �d�S���x��R��(%)
	mRotResist(0.0f),					// ��]���x��R��(%)
	mTorque(0.0f),		// �g���N=��]�����̗� * ���a = �������[�����g * ��]�����x
	mImoment(0.0f)						// �������[�����g
{
}

MoveComponent::~MoveComponent()
{
}

void MoveComponent::Update(float deltatime)
{
	// Actor�̈ʒu�ƕ������X�V
	if (!Math::NearZero(mForwardSpeed)) 
	{
		mOwner->SetPosition(mOwner->GetPosition() + mForwardSpeed * mOwner->GetForward() * deltatime);		//�O�i�@x = xo + vt
	}
	if (!Math::NearZero(mStrafeSpeed))
	{
		mOwner->SetPosition(mOwner->GetPosition() + mStrafeSpeed * mOwner->GetStrafe() * deltatime);		//�O�i�@x = xo + vt
	}
	if (!Math::NearZero(mRotSpeed))
	{
		// Temporary +Z������̉�]�݂̂��s���B
		Quaternion rot = mOwner->GetRotation();
		float angle = mRotSpeed * deltatime;
		// ��]��ǉ�������N�H�[�^�j�I�����쐬
		Quaternion inc(Vector3::UnitZ, angle);
		// ���Ƃ�rot�Ƒ����̃N�H�[�^�j�I��������
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}
	
	// ���x���X�V
	if (!Math::NearZero(mMass))
	{
		//�d�S�����x�̌v�Z�@F=ma  a=F*(1/m)
		mForwardAccel = mForwardForce * (1.0f / mMass);
		mStrafeAccel = mStrafeForce * (1.0f / mMass);
		//��R�� = ����*��R�W��    ���� = -����*��R�W��/����
		float movedecel = Math::Sqrt(mForwardSpeed * mForwardSpeed + mStrafeSpeed * mStrafeSpeed)* mMoveResist * 0.01f * (1 / mMass);
		mForwardAccel -= movedecel * mOwner->GetForward().Length();
		mStrafeAccel -= movedecel * mOwner->GetStrafe().Length();
	}
	else { mForwardAccel = 0.0f; mStrafeAccel = 0.0f; }
	// Temporary +Z������̉�]�݂̂��s���B
	// �������[�����g�v�Z	 ��3�����ɂ����ẮA��l���x�̋��Ƃ���B I=0.4*����*���a^2
	mImoment = 0.4f * mMass * mOwner->GetRadius() * mOwner->GetRadius();
	if (!Math::NearZero(mImoment))
	{
		// �g���N�v�Z�@�@�g���N=��]�����̗� * ���a
		mTorque = mRotForce * mOwner->GetRadius();
		// ��]�����x�̌v�Z�@��]�����x = �g���N / �������[�����g
		mRotAccel = mTorque / mImoment;		//��]�����x�̌v�Z Fr=Ia  a=Fr/I
		//��R�� = ����*��R�W��    ���� = -����*��R�W��*���a/�������[�����g
		float rotdecel = mRotSpeed * mOwner->GetRadius() * mRotResist / mImoment;
		mRotAccel -= rotdecel;
	}
	else { mRotAccel = 0.0f; }

	mForwardSpeed += mForwardAccel * deltatime;		//v = vo + at
	mStrafeSpeed += mStrafeAccel * deltatime;
	mRotSpeed += mRotAccel * deltatime;		//�� = ��o + bt
}
