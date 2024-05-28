#pragma once
#include "Component.h"
#include <cstddef>

class MeshComponent : public Component
{
public:
	MeshComponent(class Actor* owner);
	~MeshComponent();
	// このメッシュコンポーネントで描画
	virtual void Draw(class Shader* shader);
	// メッシュ・テクスチャをメッシュコンポーネントで使用されているインデックスで設定
	virtual void SetMesh(class Mesh* mesh) { mMesh = mesh; }
	void SetTextureIndex(size_t index) { mTextureIndex = index; }

	void SetVisible(bool visible) { mVisible = visible; }
	bool GetVisible() const { return mVisible; }
protected:
	class Mesh* mMesh;
	size_t mTextureIndex;
	bool mVisible;
};
