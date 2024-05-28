#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	//�R���X�g���N�^�BUpdate Order�͑��߁B
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltatime) override;

	// �e�p�����[�^�̃Z�b�^�[���Q�b�^�[
	// ���x
	float GetForwardSpeed() { return mForwardSpeed; }	// �O�i
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	float GetStrafeSpeed() { return mStrafeSpeed; }		// ���i
	void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }
	float GetRotSpeed() const { return mRotSpeed; }		// ��]
	void SetRotSpeed(float speed) { mRotSpeed = speed; }

	// �ÓT�����ɕK�v�ȃp�����[�^�̃Z�b�^�[���Q�b�^�[
	//�O�i�����ɂ������
	float GetFowardForce() { return mForwardForce; }
	void SetForwardForce(float force) { mForwardForce = force; }
	//���i�����ɂ������
	float GetStrafeForce() { return mStrafeForce; }
	void SetStrafeForce(float force) { mStrafeForce = force; }
	//��]�����̗�
	float GetRotForce() { return mRotForce; }
	void SetRotForce(float force) { mRotForce = force; }
	//�d�S�ړ��̒�R��
	float GetMoveResist() { return mMoveResist; }
	void SetMoveResist(float resist) { mMoveResist = resist; }
	//��]�ړ��̒�R��
	float GetRotResist() { return mRotResist; }
	void SetRotResist(float resist) { mRotResist = resist; }
	// ����
	float GetMass() { return mMass; }
	void SetMass(float mass) { mMass = mass; }
	// �g���N
	float GetTorque() { return mTorque; }
	void SetTorque(float torque) { mTorque = torque; }
	// �������[�����g
	float GetImoment() { return mImoment; }
	void SetImoment(float imoment) { mImoment = imoment; }


private:
	// �ړ��p�����[�^
	float mForwardSpeed;	// �O�i�ړ����x
	float mStrafeSpeed;		// ���i�ړ����x
	float mRotSpeed;		// ��]���x

	// �ÓT�����p�����[�^
	float mMass;			// ����
	float mForwardForce;	// �O�i�����̏d�S�ɂ������
	float mStrafeForce;		// ���i�����̏d�S�ɂ������
	float mRotForce;		// ��]�����̗�F +������CCW  ��temporary�@��]��z�����肾��
	float mTorque;			// �g���N=��]�����̗� * ���a = �������[�����g * ��]�����x  ��temporary�@��]��z�����肾��
	float mForwardAccel;	// �O�i�����̏d�S�����x	=�d�S�ɂ������ / ���� 
	float mStrafeAccel;		// ���i�����̏d�S�����x	=�d�S�ɂ������ / ���� 
	float mRotAccel;		// ��]�����x  ��temporary�@��]��z�����肾��
	float mImoment;			// �������[�����g
	float mMoveResist;		// �d�S���x��R��(%)
	float mRotResist;		// ��]���x��R��(%)
};

