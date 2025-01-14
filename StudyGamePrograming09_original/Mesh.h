#pragma once
#include <vector>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	// Load/unload mesh
	bool Load(const std::string& fileName, class Renderer* renderer);
	void Unload();
	// このMeshに割り当てられた頂点配列を取得
	class VertexArray* GetVertexArray() { return mVertexArray; }
	// indexからテクスチャを取得
	class Texture* GetTexture(size_t index);
	// シェーダー名を取得
	const std::string& GetShaderName() const { return mShaderName; }
	// オブジェクト空間での境界球の半径を取得
	float GetRadius() const { return mRadius; }
	// Get specular power of mesh
	float GetSpecPower() const { return mSpecPower; }
private:
	// Meshのテクスチャ群
	std::vector<class Texture*> mTextures;
	// Meshの頂点配列
	class VertexArray* mVertexArray;
	// シェーダーの名前
	std::string mShaderName;
	// オブジェクト空間での境界球の半径
	float mRadius;
	// Specular power of surface
	float mSpecPower;
};