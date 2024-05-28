#include "InputComponent.h"
#include "Actor.h"

//OpenGL�p�̍��W�n�ɏC��

InputComponent::InputComponent(Actor* owner, int updateOrder) : MoveComponent(owner)
	, mForwardKey(0)
	, mBackwardKey(0)
	, mRightStrafeKey(0)
	, mLeftStrafeKey(0)
	, mClockwiseKey(0)
	, mCounterClockwiseKey(0)

{
}

InputComponent::~InputComponent()
{
}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	//�ÓT�����w��MoveComponent�̂��߂̌v�Z
	//MoveComponent�ɂ͑O�i����]�����̗͂̍ő�l������n��
	float forwardforce = 0.0f;
	if (keyState[mForwardKey])
	{
		forwardforce += mMaxForwardForce;
	}
	else if (keyState[mBackwardKey])
	{
		forwardforce -= mMaxForwardForce;
	}
	SetForwardForce(forwardforce);
	
	float strafeforce = 0.0f;
	if (keyState[mRightStrafeKey])
	{
		strafeforce += mMaxStrafeForce;
	}
	else if (keyState[mLeftStrafeKey])
	{
		strafeforce -= mMaxStrafeForce;
	}
	SetStrafeForce(strafeforce);

	float rotforce = 0.0f;
	if (keyState[mClockwiseKey])
	{
		rotforce -= mMaxRotForce;		//�p�x�́{������CCW
	}
	else if (keyState[mCounterClockwiseKey])
	{
		rotforce += mMaxRotForce;		//�p�x�́{������CCW
	}
	SetRotForce(rotforce);

}

