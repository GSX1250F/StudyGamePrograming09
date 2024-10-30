#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
	enum State
	{
		EActive,		//稼働中
		EPaused,		//更新停止中
		EDead			//削除対象
	};

	Actor(class Game* game);
	virtual ~Actor();

	// ゲームから呼び出されるProcess Input(オーバーライド不可)
	void ProcessInput(const uint8_t* keyState);
	// アクター独自の入力処理(オーバーライド可能)
	virtual void ActorInput(const uint8_t* keyState);

	// ゲームから呼び出される更新関数(オーバーライド不可)
	void Update(float deltaTime);
	// アクターが持つ全コンポーネントを更新(オーバーライド不可)
	void UpdateComponents(float deltaTime);
	// アクター独自の更新処理(オーバーライド可能)
	virtual void UpdateActor(float deltaTime);
	// Add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	class Game* GetGame() { return mGame; }
	// ワールド変換
	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	// 全座標系
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }
	float GetRadius() { return mRadius * mScale; }	//拡大率を考慮
	void SetRadius(float radius) { mRadius = radius; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }

	// ワールド座標系
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true;}
	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeWorldTransform = true;}
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }		//前方単位ベクトル
	Vector3 GetRightward() const { return Vector3::Transform(Vector3::UnitY, mRotation); }		//右方単位ベクトル
	Vector3 GetUpward() const { return Vector3::Transform(Vector3::UnitZ, mRotation); }			//上方単位ベクトル

private:
	class Game* mGame;
	std::vector<class Component*> mComponents;
	Matrix4 mWorldTransform;
	bool mRecomputeWorldTransform;

	// 全座標系
	State mState;			// アクターの状態
	float mScale;			// 拡大率
	float mRadius;			// 半径（拡大率は無視）
	// ワールド座標系
	Vector3 mPosition;		//画面上の位置
	Quaternion mRotation;	//回転（クォータニオン）
};