#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

// FMOD�w�b�_�[�̑O���錾
namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class AudioSystem
{
public:
	AudioSystem(class Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();

	// �o���N�̃��[�h�^�A�����[�h
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void Update(float deltaTime);

	// 3D�|�W�V���i���I�[�f�B�I�̂��߂̊֐�
	void SetListener(const Matrix4& viewMatrix);
	// Bus����
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	// Tracks the next ID to use for event instances
	static unsigned int sNextID;

	class Game* mGame;
	// ���[�h���ꂽ�o���N�̘A�z�z��B�o���N�̃t�@�C�������L�[�Ƃ���B
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	// �C�x���g������EventDescription�ւ̘A�z�z��BFMOD���C�x���g�Ɋ��蓖�Ă����O�i�p�X�`���j�Ƃ���B
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	// �����Ȃ���������C�x���g�C���X�^���X�ւ̘A�z�z��
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	// Bus�̘A�z�z��
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
	// FMOD studio system
	FMOD::Studio::System* mSystem;
	// FMOD Low-level system (�K�v���̂��߂�)
	FMOD::System* mLowLevelSystem;
};

