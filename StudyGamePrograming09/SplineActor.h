#pragma once
#include "Actor.h"

// スプラインカメラ用アクター
class SplineActor : public Actor
{
public:
	SplineActor(class Game* game);

	void ActorInput(const uint8_t* keys) override;

	void RestartSpline();
private:
	class SplineCamera* mCameraComp;
};
