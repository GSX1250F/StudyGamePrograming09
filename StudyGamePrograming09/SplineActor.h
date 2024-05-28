#pragma once
#include "Actor.h"

// �X�v���C���J�����p�A�N�^�[
class SplineActor : public Actor
{
public:
	SplineActor(class Game* game);

	void ActorInput(const uint8_t* keys) override;

	void RestartSpline();
private:
	class SplineCamera* mCameraComp;
};
