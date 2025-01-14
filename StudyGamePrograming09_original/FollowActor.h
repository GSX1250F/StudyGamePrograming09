#pragma once
#include "Actor.h"

// �Ǐ]�J�����p�̃A�N�^�[
class FollowActor : public Actor
{
public:
	FollowActor(class Game* game);

	void ActorInput(const uint8_t* keys) override;

	void SetVisible(bool visible);
private:
	class MoveComponent* mMoveComp;
	//class FollowCamera* mCameraComp;
	class FollowOrbitCamera* mCameraComp;
	class MeshComponent* mMeshComp;
};