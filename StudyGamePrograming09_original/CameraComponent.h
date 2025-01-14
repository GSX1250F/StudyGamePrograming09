#pragma once
#include "Component.h"
#include "Math.h"

//�e��J�����̃R���|�[�l���g
class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);
protected:
	void SetViewMatrix(const Matrix4& view);
};
