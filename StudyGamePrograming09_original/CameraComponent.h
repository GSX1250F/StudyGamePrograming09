#pragma once
#include "Component.h"
#include "Math.h"

//各種カメラのコンポーネント
class CameraComponent : public Component
{
public:
	CameraComponent(class Actor* owner, int updateOrder = 200);
protected:
	void SetViewMatrix(const Matrix4& view);
};
