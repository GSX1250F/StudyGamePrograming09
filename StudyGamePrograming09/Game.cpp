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
	// SDL初期化
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("SDLを初期化できません: %s", SDL_GetError());
		return false;
	}

	// レンダラー作成
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(mWindowWidth, mWindowHeight))
	{
		SDL_Log("レンダラーの初期化に失敗しました");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// AudioSystem作成
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("Audio Systemの初期化に失敗しました");
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
	// キューにイベントがあれば繰り返す
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				mIsRunning = false;
				break;
			// キーが押された最初に実行される
			case SDL_KEYDOWN:
				if (!event.key.repeat)
				{
					//キーが押されたままのとき
					HandleKeyPress(event.key.keysym.sym);
				}
				break;
			// マウスボタンが押されたときに実行
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
			// 画面中心をスタート位置として取得
			Vector3 screenPoint(0.0f, 0.0f, 0.0f);
			// 逆射影によってスタート位置のワールド座標を取得
			Vector3 start = mRenderer->Unproject(screenPoint);
			// 画面中心から少し先の位置をエンド位置として取得
			screenPoint.z = 0.9f;
			// 逆射影によってエンド位置のワールド座標を取得
			Vector3 end = mRenderer->Unproject(screenPoint);
			// sphereをセット
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
	// デルタタイムの計算
	// 前のフレームから 16ms 経つまで待つ
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// デルタタイムを最大値で制限する
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// すべてのアクターを更新
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	// 待ちアクターをmActorsに移動
	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// 死んだアクターを一時配列に追加
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// 死んだアクターを削除
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
	// アクター作成
	Actor* a = new Actor(this);
	// 立方体を作成
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -0.5f * Math::Pi);	//y軸中心に-90°回す
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, -0.75f*Math::Pi));		//さらにz軸中心に-135°回す
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	// 球体を作成
	a = new Actor(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

	// 床を作成（PlaneActor）
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++)		// 10個ずつ縦横に並べる
	{
		for (int j = 0; j < 10; j++)
		{
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// 左右壁を作成（PlaneActor）
	q = Quaternion(Vector3::UnitX, 0.5f*Math::Pi);	//x軸中心に90°回す
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	// 前後壁を作成（PlaneActor）
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, 0.5f*Math::Pi));	//さらにz軸中心に90°回す
	for (int i = 0; i < 10; i++)
	{
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	// 光源設定
	// 環境光
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(-0.250f, -0.433f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// その他のアクター
	// ライフバー
	a = new Actor(this);
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/HealthBar.png"));
	// レーダー
	a = new Actor(this);
	a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(mRenderer->GetTexture("Assets/Radar.png"));
	// 照準
	a = new Actor(this);
	a->SetScale(2.0f);
	mCrosshair = new SpriteComponent(a);
	mCrosshair->SetTexture(mRenderer->GetTexture("Assets/Crosshair.png"));

	// 音楽スタート
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

	// マウスの相対運動モードを有効にする
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//相対運動モードの初期化
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// それぞれのカメラアクター
	mFPSActor = new FPSActor(this);				//FPSカメラ
	mFollowActor = new FollowActor(this);		//追従カメラ
	mOrbitActor = new OrbitActor(this);			//軌道カメラ
	mSplineActor = new SplineActor(this);		//スプラインカメラ

	ChangeCamera('1');	//最初はFPSカメラモード

	// Sphereを逆射影のデモに使用
	mStartSphere = new Actor(this);
	mStartSphere->SetPosition(Vector3(10000.0f, 0.0f, 0.0f));	//画面外においておく？
	mStartSphere->SetScale(0.25f);
	mc = new MeshComponent(mStartSphere);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	mEndSphere = new Actor(this);
	mEndSphere->SetPosition(Vector3(10000.0f, 0.0f, 0.0f));		//画面外においておく？
	mEndSphere->SetScale(0.25f);
	mc = new MeshComponent(mEndSphere);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	mc->SetTextureIndex(1);
}

void Game::UnloadData()
{
	// アクターを消去
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
	// アクターを更新するとき、Pendingアクターに追加しておく
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
	// Pendingアクターである場合
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// 配列の最後に入れ替えて、消去
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// アクターにある場合
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// 配列の最後に入れ替えて、消去
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::ChangeCamera(int mode)
{
	// まずすべて無効化
	mFPSActor->SetState(Actor::EPaused);
	mFPSActor->SetVisible(false);
	mCrosshair->SetVisible(false);
	mFollowActor->SetState(Actor::EPaused);
	mFollowActor->SetVisible(false);
	mOrbitActor->SetState(Actor::EPaused);
	mOrbitActor->SetVisible(false);
	mSplineActor->SetState(Actor::EPaused);

	// モードに応じて有効化
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