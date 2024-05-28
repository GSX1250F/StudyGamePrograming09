#include "AudioSystem.h"
#include <SDL_log.h>
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	:mGame(game)
	, mSystem(nullptr)
	, mLowLevelSystem(nullptr)
{
}

AudioSystem::~AudioSystem()
{
}

bool AudioSystem::Initialize()
{
	// エラーログの初期化
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, // エラーだけログをとる
		FMOD_DEBUG_MODE_TTY // stdoutに出力
	);

	// FMOD studioシステムのインスタンスを作る
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK)
	{
		SDL_Log("FMODシステムの作成に失敗しました: %s", FMOD_ErrorString(result));
		return false;
	}

	// FMOD studioシステムの初期化
	result = mSystem->initialize(
		512,						// 最大同時発音数
		FMOD_STUDIO_INIT_NORMAL,	// デフォルト設定
		FMOD_INIT_NORMAL,			// デフォルト設定
		//FMOD_INIT_CHANNEL_LOWPASS,	// ローパスフィルターを初期化する
		nullptr						// 通常はnullptr
	);
	if (result != FMOD_OK)
	{
		SDL_Log("FMODシステムの初期化に失敗しました: %s", FMOD_ErrorString(result));
		return false;
	}

	// low-levelシステムポインタを取得して保存する。これで初期化完了。
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	// どのFMOD Studioプロジェクトにあるデフォルトのバンクファイルをロード
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown()
{
	// すべてのバンクをアンロード
	UnloadAllBanks();
	// FMOD systemを解放
	if (mSystem)
	{
		mSystem->release();
	}
}

void AudioSystem::LoadBank(const std::string& name)
{
	// 多重読み込みの防止
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	// バンクをロード
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),					// ファイル名または空白
		FMOD_STUDIO_LOAD_BANK_NORMAL,	// 通常の読み込み
		&bank							// イベントへのポインタを保存
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK)
	{
		// バンクを連想配列に追加
		mBanks.emplace(name, bank);
		// ストリーミング以外のサンプルデータをすべてロード
		bank->loadSampleData();
		// このバンクにあるイベントの数を取得
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0)
		{
			// バンクにあるイベント記述子のリストを取得
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				// このイベントのパスを取得 (例　event:/Explosion2D)
				e->getPath(eventName, maxPathLength, nullptr);
				// 連想配列に追加
				mEvents.emplace(eventName, e);
			}
		}
		// このバンクにあるBusの数を取得
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0)
		{
			// このバンクにあるBusのリストを取得
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++)
			{
				FMOD::Studio::Bus* bus = buses[i];
				// このBusのパスを取得 (例 bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				// バスの連想配列へ追加
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	// ロードされていないものは無視
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	// 最初にこのバンクのすべてのイベントを消去
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0)
	{
		// このバンクのイベント記述子を取得
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		// イベントリストを取得
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			// このイベントのパスを取得
			e->getPath(eventName, 512, nullptr);
			// このイベントを消去
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}
	// このバンクにあるBusの数を取得
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0)
	{
		// このバンクにあるBusのリストを取得
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++)
		{
			FMOD::Studio::Bus* bus = buses[i];
			// このBusのパスを取得 (例 bus:/SFX)
			bus->getPath(busName, 512, nullptr);
			// このBusを消去
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end())
			{
				mBuses.erase(busi);
			}
		}
	}

	// サンプルデータとバンクをアンロード
	bank->unloadSampleData();
	bank->unload();
	// バンクの連想配列から消去
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	// すべてのバンクをアンロードしてmEventsとmBanksをクリアする
	for (auto& iter : mBanks)
	{
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name)
{
	unsigned int retID = 0;
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		// イベントの作成を確認
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			// イベントのインスタンスを作成
			event->start();
			// 次のIDを取得し、連想配列へ追加
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime)
{
	// ストップしたイベントインスタンスを検索
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances)
	{
		FMOD::Studio::EventInstance* e = iter.second;
		// イベントの状態を取得
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED)
		{
			// イベントを解放してIDを修了リストに追加
			e->release();
			done.emplace_back(iter.first);
		}
	}

	// 終了したイベントインスタンスを連想配列から消去
	for (auto id : done)
	{
		mEventInstances.erase(id);
	}

	// FMOD更新
	mSystem->update();
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in)
	{
		// 座標系の変換 (+x：前方向, +y：右方向, +z：上方向)
		// to FMOD (+z：前方向, +x：右方向, +y：上方向)
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix)
{
	// ベクトルを得るためのビュー行列の逆行列を計算
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	// 位置を設定。
	listener.position = VecToFMOD(invView.GetTranslation());
	// 逆ビューでは第３成分（GetZAxis）が前方向
	listener.forward = VecToFMOD(invView.GetZAxis());
	// 逆ビューでは第２成分（GetYAxis）が上方向
	listener.up = VecToFMOD(invView.GetYAxis());
	// 速度はゼロにセットする ドップラー効果を使うときは修正)
	listener.velocity = { 0.0f, 0.0f, 0.0f }; 
	//listener.velocity = { 0.0f, 0.0f, 300.0f };
	// FMODにおくる（0はリスナーが一人だけという意味）
	mSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const
{
	float retVal = 0.0f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string& name) const
{
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause)
{
	auto iter = mBuses.find(name);
	if (iter != mBuses.end())
	{
		iter->second->setPaused(pause);
	}
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id)
{
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end())
	{
		event = iter->second;
	}
	return event;
}
