#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	MoveComponent(class Actor* owner, int updateOrder = 10);
	~MoveComponent();

	void Update(float deltatime) override;

	// �S���W�n
	float GetMass() { return mMass; }
	void SetMass(float v) { mMass = v; }		// ����
	float GetImoment() const;		// �������[�����g
	void SetMoveResist(float v) { mResist = v; }		//�ړ��̒�R��
	void SetRotResist(float v) { mRotResist = v; }		//��]�̒�R��

	// ���[���h���W�n
	void SetVelocity(Vector3 value) { mVelocity = value; }		// ���x
	Vector3 GetVelocity() { return mVelocity; }
	void SetRotSpeed(Vector3 value) { mRotSpeed = value; }	// ��]���x���[�����g�x�N�g��
	Vector3 GetRotSpeed() { return mRotSpeed; }
	Vector3 GetAccel() const;		// �����x
	Vector3 GetRotAccel() const;		// ��]�����x���[�����g�x�N�g��
	void SetForce(Vector3 value) { mForce = value; }		//��	
	void SetRotForce(Vector3 value) { mRotForce = value; }		//��]�̓��[�����g�x�N�g��
	Vector3 GetTorque() const;		// �g���N(=��]�́~���a�j�̃��[�����g�x�N�g��


private:
	float mMass;			// ����
	float mResist;		// �d�S���x��R��(%)
	float mRotResist;		// ��]���x��R��(%)

	// ���[���h���W�n
	Vector3 mVelocity;		// ���x
	Vector3 mRotSpeed;		// ��]���x���[�����g�x�N�g��
	Vector3 mForce;		// �d�S�ɂ������
	Vector3 mRotForce;		// ��]�����̗̓��[�����g�x�N�g��

};