#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <algorithm>
#include <thread>
#include <chrono>
#include "Renderer.h"
#include "Actor.h"
#include "CameraActor.h"
#include "MeshActors.h"
#include "SpriteActors.h"


Game::Game()
	: mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mTicksCount(0)
	, mWindowWidth(1024)
	, mWindowHeight(768)
{}

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

	LoadData();

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
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			mIsRunning = false;
		}
		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				mIsRunning = false;
			}
		}

		mUpdatingActors = true;
		for (auto actor : mActors)
		{
			actor->ProcessInput(event);
		}
		mUpdatingActors = false;
	}
}

void Game::UpdateGame()
{
	// �t���[�����[�g�����i62.5fps)
	if (SDL_GetTicks() - mTicksCount < 16) {
		int sleep = 16 - (SDL_GetTicks() - mTicksCount);
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));    // sleep�~���b�������~�߂�
	}

	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// �O�̃t���[������ 16ms �o�܂ő҂�.��sleep���Ȃ��̂�CPU�g�p�����オ��B
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;		// �f���^�^�C���̌v�Z
	if (deltaTime > 0.05f) { deltaTime = 0.05f; }			// �f���^�^�C�����ő�l�Ő�������
	mTicksCount = SDL_GetTicks();

	// �A�N�^�[���X�V
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// �҂��ɂȂ��Ă����A�N�^�[��mActors�Ɉړ�
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
}

void Game::GenerateOutput()
{
	mRenderer->Draw();
}

void Game::LoadData()
{
	// �J����
	Actor* a = new CameraActor(this);
	
	// ���b�V���A�N�^�[
	a = new MeshActors(this);
	// �X�v���C�g�`��̃A�N�^�[��
	a = new SpriteActors(this);

	// ����
	Vector3 color;
	// ����
	//color = Vector3(0.7f, 0.7f, 0.7f);
	//mRenderer->SetAmbientLight(color);
	// ���s����
	/*
	DirectionalLight dl;
	dl.mDirection = Vector3::Normalize(Vector3(1.0f, 1.0f, -1.0f));
	color = Vector3(0.7f, 0.0f, 0.0f);
	dl.mDiffuseColor = color;
	dl.mSpecColor = color;
	mRenderer->AddDirectionalLight(dl);
	dl.mDirection = Vector3::Normalize(Vector3(-1.0f, -1.0f, -1.0f));
	color = Vector3(0.0f, 0.7f, 0.0f);
	dl.mDiffuseColor = color;
	dl.mSpecColor = color;
	mRenderer->AddDirectionalLight(dl);
	*/
	// �_����
	PointLight pl;
	pl.mAttenuation = 0.9f;
	pl.mPosition = Vector3(1250.0f, 1250.0f, 500.0f);
	color = Vector3(1.0f, 1.0f, 0.0f);
	pl.mDiffuseColor = color;
	pl.mSpecColor = color;
	mRenderer->AddPointLight(pl);
	pl.mAttenuation = 0.9f;
	pl.mPosition = Vector3(1250.0f, -1250.0f, 500.0f);
	color = Vector3(1.0f, 0.0f, 0.0f);
	pl.mDiffuseColor = color;
	pl.mSpecColor = color;
	mRenderer->AddPointLight(pl);
	pl.mAttenuation = 0.9f;
	pl.mPosition = Vector3(-1250.0f, 1250.0f, 500.0f);
	color = Vector3(0.0f, 1.0f, 0.0f);
	pl.mDiffuseColor = color;
	pl.mSpecColor = color;
	mRenderer->AddPointLight(pl);
	pl.mAttenuation = 0.9f;
	pl.mPosition = Vector3(-1250.0f, -1250.0f, 500.0f);
	color = Vector3(0.0f, 0.0f, 1.0f);
	pl.mDiffuseColor = color;
	pl.mSpecColor = color;
	mRenderer->AddPointLight(pl);
}

void Game::UnloadData()
{
	// actors������
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	if (mRenderer)
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
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
	// �A�N�^�[�̍X�V���Ȃ�mPendingActors�ɒǉ�
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
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}