#include "Game.h"
#include <algorithm>
#include "Renderer.h"
#include "AudioSystem.h"
#include "Actor.h"
#include "Mesh.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "AudioComponent.h"
#include "FPSActor.h"
#include "FollowActor.h"
#include "OrbitActor.h"
#include "SplineActor.h"

Game::Game():mRenderer(nullptr),
	mAudioSystem(nullptr),
	mIsRunning(true),
	mUpdatingActors(false),
	mWindowWidth(1024),
	mWindowHeight(768)
{
}

bool Game::Initialize()
{
	// SDL������
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDL���������ł��܂���: %s", SDL_GetError());
		return false;
	}

	// �����_���[�쐬
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(mWindowWidth, mWindowHeight))
	{
		SDL_Log("�����_���[�̏������Ɏ��s���܂���");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// AudioSystem�쐬
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Audio System�̏������Ɏ��s���܂���");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;

}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	// �L���[�ɃC�x���g������ΌJ��Ԃ�
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
			// �L�[�������ꂽ�ŏ��Ɏ��s�����
			case SDL_KEYDOWN:
				if (!event.key.repeat)
				{
					//�L�[�������ꂽ�܂܂̂Ƃ�
					HandleKeyPress(event.key.keysym.sym);
				}
				break;
			// �}�E�X�{�^���������ꂽ�Ƃ��Ɏ��s
			case SDL_MOUSEBUTTONDOWN:
				HandleKeyPress(event.button.button);
				break;
			default:
				break;
		}
	}

	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::HandleKeyPress(int key)
{
	switch (key)
	{
		case '-':
		{
			// Reduce master volume
			float volume = mAudioSystem->GetBusVolume("bus:/");
			volume = Math::Max(0.0f, volume - 0.1f);
			mAudioSystem->SetBusVolume("bus:/", volume);
			break;
		}
		case '^':
		{
			// Increase master volume
			float volume = mAudioSystem->GetBusVolume("bus:/");
			volume = Math::Min(1.0f, volume + 0.1f);
			mAudioSystem->SetBusVolume("bus:/", volume);
			break;
		}
		/*
		case 'e':
			// Play explosion
			mAudioSystem->PlayEvent("event:/Explosion2D");
			break;
		case 'm':
			// Toggle music pause state
			mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
			break;
		case 'r':
			// Stop or start reverb snapshot
			if (!mReverbSnap.IsValid())
			{
				mReverbSnap = mAudioSystem->PlayEvent("snapshot:/WithReverb");
			}
			else
			{
				mReverbSnap.Stop();
			}
			break;
		*/
		case '1':
		case '2':
		case '3':
		case '4':
			ChangeCamera(key);
			break;
		case SDL_BUTTON_LEFT:
		{
			// ��ʒ��S���X�^�[�g�ʒu�Ƃ��Ď擾
			Vector3 screenPoint(0.0f, 0.0f, 0.0f);
			// �t�ˉe�ɂ���ăX�^�[�g�ʒu�̃��[���h���W���擾
			Vector3 start = mRenderer->Unproject(screenPoint);
			// ��ʒ��S���班����̈ʒu���G���h�ʒu�Ƃ��Ď擾
			screenPoint.z = 0.9f;
			// �t�ˉe�ɂ���ăG���h�ʒu�̃��[���h���W���擾
			Vector3 end = mRenderer->Unproject(screenPoint);
			// sphere���Z�b�g
			mStartSphere->SetPosition(start);
			mEndSphere->SetPosition(end);
			break;
		}
		default:
			break;
	}
}

void Game::UpdateGame()
{
	// �f���^�^�C���̌v�Z
	// �O�̃t���[������ 16ms �o�܂ő҂�
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// �f���^�^�C�����ő�l�Ő�������
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// ���ׂẴA�N�^�[���X�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// �҂��A�N�^�[��mActors�Ɉړ�
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// ���񂾃A�N�^�[���ꎞ�z��ɒǉ�
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// ���񂾃A�N�^�[���폜
	for (auto actor : deadActors)
	{
		delete actor;
	}

	// Update Audio System
	mAudioSystem->Update(deltaTime);
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	// �A�N�^�[�쐬
	Actor* a = new Actor(this);
	// �����̂��쐬
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -0.5f * Math::Pi);	//y�����S��-90����
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, -0.75f*Math::Pi));		//�����z�����S��-135����
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	// ���̂��쐬
	a = new Actor(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

	// �����쐬�iPlaneActor�j
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)		// 10���c���ɕ��ׂ�
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// ���E�ǂ��쐬�iPlaneActor�j
	q = Quaternion(Vector3::UnitX, 0.5f*Math::Pi);	//x�����S��90����
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	// �O��ǂ��쐬�iPlaneActor�j
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, 0.5f*Math::Pi));	//�����z�����S��90����
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	// �����ݒ�
	// ����
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(-0.250f, -0.433f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// ���̑��̃A�N�^�[
	// ���C�t�o�[
	a = new Actor(this);
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));
	// ���[�_�[
	a = new Actor(this);
	a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));
	// �Ə�
	a = new Actor(this);
	a->SetScale(2.0f);
	mCrosshair = new SpriteComponent(a);
	mCrosshair->SetTexture(mRenderer->GetTexture("Assets/Crosshair.png"));

	// ���y�X�^�[�g
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

	// �}�E�X�̑��Ή^�����[�h��L���ɂ���
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//���Ή^�����[�h�̏�����
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// ���ꂼ��̃J�����A�N�^�[
	mFPSActor = new FPSActor(this);				//FPS�J����
	mFollowActor = new FollowActor(this);		//�Ǐ]�J����
	mOrbitActor = new OrbitActor(this);			//�O���J����
	mSplineActor = new SplineActor(this);		//�X�v���C���J����

	ChangeCamera('1');	//�ŏ���FPS�J�������[�h

	// Sphere���t�ˉe�̃f���Ɏg�p
	mStartSphere = new Actor(this);
	mStartSphere->SetPosition(Vector3(10000.0f, 0.0f, 0.0f));	//��ʊO�ɂ����Ă����H
	mStartSphere->SetScale(0.25f);
	mc = new MeshComponent(mStartSphere);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	mEndSphere = new Actor(this);
	mEndSphere->SetPosition(Vector3(10000.0f, 0.0f, 0.0f));		//��ʊO�ɂ����Ă����H
	mEndSphere->SetScale(0.25f);
	mc = new MeshComponent(mEndSphere);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	mc->SetTextureIndex(1);
}

void Game::UnloadData()
{
	// �A�N�^�[������
	// Because ~Actor calls RemoveActor, have to use a different style loop
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	if(mRenderer)
	{
		mRenderer->UnloadData();
	}
}

void Game::Shutdown()
{
	UnloadData();
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	if (mAudioSystem)
	{
		mAudioSystem->Shutdown();
	}
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// �A�N�^�[���X�V����Ƃ��APending�A�N�^�[�ɒǉ����Ă���
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	// Pending�A�N�^�[�ł���ꍇ
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// �z��̍Ō�ɓ���ւ��āA����
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// �A�N�^�[�ɂ���ꍇ
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// �z��̍Ō�ɓ���ւ��āA����
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::ChangeCamera(int mode)
{
	// �܂����ׂĖ�����
	mFPSActor->SetState(Actor::EPaused);
	mFPSActor->SetVisible(false);
	mCrosshair->SetVisible(false);
	mFollowActor->SetState(Actor::EPaused);
	mFollowActor->SetVisible(false);
	mOrbitActor->SetState(Actor::EPaused);
	mOrbitActor->SetVisible(false);
	mSplineActor->SetState(Actor::EPaused);

	// ���[�h�ɉ����ėL����
	switch (mode)
	{
	case '1':
	default:
		mFPSActor->SetState(Actor::EActive);
		mFPSActor->SetVisible(true);
		mCrosshair->SetVisible(true);
		break;
	case '2':
		mFollowActor->SetState(Actor::EActive);
		mFollowActor->SetVisible(true);
		break;
	case '3':
		mOrbitActor->SetState(Actor::EActive);
		mOrbitActor->SetVisible(true);
		break;
	case '4':
		mSplineActor->SetState(Actor::EActive);
		mSplineActor->RestartSpline();
		break;
	}
}