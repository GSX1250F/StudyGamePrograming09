#pragma once
#include "Actor.h"
#include "SoundEvent.h"

// 一人称アクター
class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class MeshComponent* mMeshComp;
	class FPSCamera* mCameraComp;
	class Actor* mFPSModel;		//FPSモデル（体や腕など）FPSActorにオフセット位置をもつ
	SoundEvent mFootstep;
	float mLastFootstep;
};

