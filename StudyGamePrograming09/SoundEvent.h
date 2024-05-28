#pragma once
#include <string>
#include "Math.h"

class SoundEvent
{
public:
	SoundEvent();
	// �Ή�����FMOD�C�x���g�C���X�^���X�����݂�����True��Ԃ�
	bool IsValid();
	// �C�x���g�����X�^�[�g����
	void Restart();
	// �C�x���g���X�g�b�v����
	void Stop(bool allowFadeOut = true);
	// �Z�b�^�[�֐�
	void SetPaused(bool pause);
	void SetVolume(float value);
	void SetPitch(float value);
	void SetParameter(const std::string& name, float value);
	// �Q�b�^�[�֐�
	bool GetPaused() const;
	float GetVolume() const;
	float GetPitch() const;
	float GetParameter(const std::string& name);
	// Positional
	bool Is3D() const;
	void Set3DAttributes(const Matrix4& worldTrans);
protected:
	// �R���X�g���N�^��protected�ō쐬���AAudioSystem��friend�N���X�Ƃ��Đ錾����B
	// AudioSystem�݂̂����̃R���X�g���N�^�ɃA�N�Z�X�\
	friend class AudioSystem;
	SoundEvent(class AudioSystem* system, unsigned int id);
private:
	class AudioSystem* mSystem;
	unsigned int mID;
};

