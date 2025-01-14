#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner),	
	mUpdateOrder(updateOrder)
{
	// �A�N�^�[�̔z��ɒǉ�
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}
