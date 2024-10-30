#include "MoveComponent.h"
#include "Actor.h"
#include "Math.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	: Component(owner)
	, mVelocity(Vector3::Zero)
	, mRotSpeed(Vector3::Zero)
	, mMass(1.0f)
	, mForce(Vector3::Zero)
	, mRotForce(Vector3::Zero)
	, mResist(0.0f)
	, mRotResist(0.0f)
{}

MoveComponent::~MoveComponent() {}

void MoveComponent::Update(float deltatime)
{
	if (!Math::NearZero(mVelocity.Length())) {
		// �ʒu���X�V
		mOwner->SetPosition(mOwner->GetPosition() + mVelocity * deltatime);
	}
	if (!Math::NearZero(mRotSpeed.Length())) {
		// �������X�V
		Quaternion rot = mOwner->GetRotation();
		// �N�H�[�^�j�I�������B��]���x���[�����g�x�N�g���̑傫�����p���x�̑傫��
		Vector3 axis = mRotSpeed;
		axis.Normalize();	// ��]���B���K������B
		float angle = deltatime * mRotSpeed.Length();	//�p�x�ω���
		Quaternion inc(axis, angle);
		// ���Ƃ�rot�Ƒ����̃N�H�[�^�j�I��������
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}

	// ���x�Ɗp���x���X�V
	SetVelocity(mVelocity + GetAccel() * deltatime);	//v = vo + at
	SetRotSpeed(mRotSpeed + GetRotAccel() * deltatime);		//�� = ��o + bt
}

Vector3 MoveComponent::GetAccel() const
{
	if (!Math::NearZero(mMass))
	{
		Vector3 accel = mForce * (1 / mMass);    //�d�S�����x�̌v�Z�@F=ma  a=F*(1/m)
		accel -= mVelocity * mResist * 0.01f * (1 / mMass);
		return accel;
	}
	else
	{
		return Vector3::Zero;
	}
}

Vector3 MoveComponent::GetRotAccel() const
{
	if (!Math::NearZero(GetImoment()))
	{
		// ��]�����x�̌v�Z�@��]�����x = �g���N / �������[�����g
		Vector3 accel = GetTorque() * (1.0f / GetImoment());		//��]�����x�̌v�Z Fr=Ia  a=Fr/I
		//��R�� = ����*��R�W��    ���� = -����*��R�W��*���a/�������[�����g
		accel -= mRotSpeed * mOwner->GetRadius() * mRotResist * (1.0f / GetImoment());
		return accel;
	}
	else
	{
		return Vector3::Zero;
	}
}

float MoveComponent::GetImoment() const
{
	// �������[�����g�v�Z�@��2�����ɂ����ẮA��l���x�̉~�Ƃ���B I=0.5*����*���a^2
	return 0.5f * mMass * mOwner->GetRadius() * mOwner->GetRadius();
}

Vector3 MoveComponent::GetTorque() const
{
	// �g���N�v�Z�@�g���N=��]�����̗� * ���a
	return mRotForce * mOwner->GetRadius();
}