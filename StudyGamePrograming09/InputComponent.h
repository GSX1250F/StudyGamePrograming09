#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	//�R���X�g���N�^�BUpdate Order�͑��߁B
	InputComponent(class Actor* owner, int updateOrder = 10);
	~InputComponent();

	// ���͏����i�I�[�o�[���C�h�j
	void ProcessInput(const uint8_t* keyState) override;

	// �v���C�x�[�g�֐��̃Q�b�^�[�E�Z�b�^�[
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackwardKey(int key) { mBackwardKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }
	void SetRightStrafeKey(int key) { mRightStrafeKey = key; }
	void SetLeftStrafeKey(int key) { mLeftStrafeKey = key; }

	int GetForwardKey() { return mForwardKey; }
	int GetBackwardKey() { return mBackwardKey; }
	int GetClockwiseKey() { return mClockwiseKey; }
	int GetCounterClockwiseKey() { return mCounterClockwiseKey; }
	int GetRightStrafeKey() { return mRightStrafeKey; }
	int GetLeftStrafeKey() { return mLeftStrafeKey; }

	void SetMaxForwardForce(float power) { mMaxForwardForce = power; }
	void SetMaxRotForce(float power) { mMaxRotForce = power; }
	void SetMaxStrafeForce(float power) { mMaxStrafeForce = power; }
	float GetMaxForwardForce() { return mMaxForwardForce; }
	float GetMaxRotForce() { return mMaxRotForce; }
	float GetMaxStrafeForce() { return mMaxStrafeForce; }


private:
	// �O�i�E��ނ̂��߂̃L�[
	int mForwardKey;
	int mBackwardKey;

	// ���i�E�E�i�̂��߂̃L�[
	int mRightStrafeKey;
	int mLeftStrafeKey;

	//��]�^���̃L�[
	int mClockwiseKey;
	int mCounterClockwiseKey;
	
	// �O�i�E��]�E���i�����̗͂̍ő�l
	float mMaxForwardForce;
	float mMaxStrafeForce;
	float mMaxRotForce;
};

