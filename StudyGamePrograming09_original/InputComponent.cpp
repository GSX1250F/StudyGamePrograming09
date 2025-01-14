#include "InputComponent.h"
#include "Actor.h"

//OpenGL—p‚ÌÀ•WŒn‚ÉC³

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
	//ŒÃ“T•¨—Šw‚ÅMoveComponent‚Ì‚½‚ß‚ÌŒvZ
	//MoveComponent‚É‚Í‘Oi‚©‰ñ“]•ûŒü‚Ì—Í‚ÌÅ‘å’l‚¾‚¯‚ğ“n‚·
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
		rotforce -= mMaxRotForce;		//Šp“x‚Ì{•ûŒü‚ÍCCW
	}
	else if (keyState[mCounterClockwiseKey])
	{
		rotforce += mMaxRotForce;		//Šp“x‚Ì{•ûŒü‚ÍCCW
	}
	SetRotForce(rotforce);

}

