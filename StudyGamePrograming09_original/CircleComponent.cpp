#include "CircleComponent.h"
#include "Actor.h"

CircleComponent::CircleComponent(Actor* owner) : Component(owner)
{
}

float CircleComponent::GetRadius() const
{
	return mOwner->GetRadius();
}

const Vector3& CircleComponent::GetCenter() const
{
	return  mOwner->GetPosition();
}

bool Intersect(const CircleComponent& a, const CircleComponent& b)
{
	// �Q��CircleComponent�̒��S�ԋ�����2����v�Z
	Vector3 diff = a.GetCenter() - b.GetCenter();
	float distSq = diff.LengthSq();

	// �Q��CircleComponent�̔��a�̘a��2����v�Z 
	float sumRadiusSq = (a.GetRadius() + b.GetRadius())* (a.GetRadius() + b.GetRadius());

	// ���S�ԋ��� <= ���a�̘a �̂Ƃ��A�Փ˂����Ɣ���
	if (distSq <= sumRadiusSq)
	{ return true; } else{ return false; }
}
