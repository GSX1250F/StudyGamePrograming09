#pragma once
#include <string>
#include "Math.h"

class SoundEvent
{
public:
	SoundEvent();
	// 対応するFMODイベントインスタンスが存在したらTrueを返す
	bool IsValid();
	// イベントをリスタートする
	void Restart();
	// イベントをストップする
	void Stop(bool allowFadeOut = true);
	// セッター関数
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	// ゲッター関数
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);
	// Positional
	bool Is3D() const;
	void Set3DAttributes(const Matrix4& worldTrans);
protected:
	// コンストラクタをprotectedで作成し、AudioSystemのfriendクラスとして宣言する。
	// AudioSystemのみがこのコンストラクタにアクセス可能
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	class AudioSystem* mSystem;
	unsigned int mID;
};

