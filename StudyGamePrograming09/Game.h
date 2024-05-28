#pragma once
#include <SDL.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "SoundEvent.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }

	void SetRunning(bool isrunning) { mIsRunning = isrunning; }

	//�Q�[���E�B���h�E�̑傫��
	int mWindowWidth;
	int mWindowHeight;

private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// ���ׂẴA�N�^�[
	std::vector<class Actor*> mActors;
	// ���ׂĂ̑҂��A�N�^�[
	std::vector<class Actor*> mPendingActors;

	class Renderer* mRenderer;
	class AudioSystem* mAudioSystem;

	Uint32 mTicksCount;
	bool mIsRunning;
	// �A�N�^�[���X�V����
	bool mUpdatingActors;

	// Game-specific code
	class FPSActor* mFPSActor;
	class FollowActor* mFollowActor;
	class OrbitActor* mOrbitActor;
	class SplineActor* mSplineActor;
	class Actor* mStartSphere;
	class Actor* mEndSphere;
	class SpriteComponent* mCrosshair;
	SoundEvent mMusicEvent;
	SoundEvent mReverbSnap;
	void ChangeCamera(int mode);
};