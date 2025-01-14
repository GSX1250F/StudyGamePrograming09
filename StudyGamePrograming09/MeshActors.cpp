#include "MeshActors.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"

MeshActors::MeshActors(Game* game) :Actor(game)
{
	Actor* a;
	std::string filename;
	// サイコロ
	filename = "Assets/Dice.gpmesh";		// BasicMesh	
	a = new Dice(game, filename);
	a->SetPosition(Vector3(700.0f, -500.0f, 0.0f));
	
	filename = "Assets/Dice2.gpmesh";		// LambertMesh
	a = new Dice(game, filename);
	a->SetPosition(Vector3(700.0f, 0.0f, 0.0f));

	filename = "Assets/Dice3.gpmesh";		// PhongMesh
	a = new Dice(game, filename);
	a->SetPosition(Vector3(700.0f, 500.0f, 0.0f));

	// 球
	filename = "Assets/Sphere.gpmesh";		// BasicMesh	
	a = new Sphere(game, filename);
	a->SetPosition(Vector3(-500.0f, -700.0f, 0.0f));

	filename = "Assets/Sphere2.gpmesh";		// LambertMesh
	a = new Sphere(game, filename);
	a->SetPosition(Vector3(0.0f, -700.0f, 0.0f));

	filename = "Assets/Sphere3.gpmesh";		// PhongMesh
	a = new Sphere(game, filename);
	a->SetPosition(Vector3(500.0f, -700.0f, 0.0f));

	// レーシングカー
	filename = "Assets/RacingCar.gpmesh";		// BasicMesh	
	a = new RacingCar(game, filename);
	a->SetPosition(Vector3(500.0f, 700.0f, -100.0f));

	filename = "Assets/RacingCar2.gpmesh";		// LambertMesh
	a = new RacingCar(game, filename);
	a->SetPosition(Vector3(0.0f, 700.0f, -100.0f));

	filename = "Assets/RacingCar3.gpmesh";		// PhongMesh
	a = new RacingCar(game, filename);
	a->SetPosition(Vector3(-500.0f, 700.0f, -100.0f));

	// 壁と床
	filename = "Assets/Plane3.gpmesh";		// PhongMesh
	const float start = -1250.0f;
	const float size = 250.0f;
	// 10個ずつ縦横に並べる
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			a = new Plane(game, filename);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// 左右壁を作成
	Quaternion q = Quaternion(Vector3::UnitX, -0.5f * Math::Pi);
	for (int i = 0; i < 10; i++)
	{
		a = new Plane(game, filename);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);
	}
	q = Quaternion(Vector3::UnitX, 0.5f * Math::Pi);
	for (int i = 0; i < 10; i++)
	{
		a = new Plane(game, filename);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	// 前後壁を作成
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, 0.5f * Math::Pi));
	for (int i = 0; i < 10; i++)
	{
		a = new Plane(game, filename);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);
	}
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi));
	for (int i = 0; i < 10; i++)
	{
		a = new Plane(game, filename);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}
}

Dice::Dice(Game* game, std::string filename) :Actor(game)
{
	SetScale(100.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh(filename));
	mMoveComp = new MoveComponent(this);
}

void Dice::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Vector3 axis = Vector3::Normalize(Vector3(1.0f, -1.0f, 1.0f));
	float rotSpeedMax = 30.0f * Math::Pi;
	float rotSpeed = rotSpeedMax * deltaTime;
	mMoveComp->SetRotSpeed(rotSpeed * axis);
}

Sphere::Sphere(Game* game, std::string filename) :Actor(game)
{
	Quaternion q(Vector3::UnitX, Math::ToRadians(-23.5f));
	SetRotation(q);
	SetScale(5.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh(filename));
	mMoveComp = new MoveComponent(this);
}

void Sphere::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	Vector3 axis = Vector3(0.0f, Math::Cos(Math::ToRadians(66.5f)), Math::Sin(Math::ToRadians(66.5f)));
	float rotSpeedMax = 5.0f * Math::Pi;
	float rotSpeed = rotSpeedMax * deltaTime;
	mMoveComp->SetRotSpeed(rotSpeed * axis);
}

RacingCar::RacingCar(Game* game, std::string filename) :Actor(game)
{
	SetScale(1.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh(filename));
	mMoveComp = new MoveComponent(this);
}

void RacingCar::UpdateActor(float deltaTime)
{
	float w = Math::Pi * 0.25f;
	/*
	float r = Math::Sqrt(GetPosition().x * GetPosition().x + GetPosition().y * GetPosition().y);
	float x = GetPosition().x * Math::Cos(w * deltaTime) - GetPosition().y * Math::Sin(w * deltaTime);
	float y = GetPosition().y * Math::Cos(w * deltaTime) + GetPosition().x * Math::Sin(w * deltaTime);
	SetPosition(Vector3(x, y, -100.0f));
	*/
	Quaternion q = GetRotation();
	Quaternion inc = Quaternion(Vector3::UnitZ, w * deltaTime);
	q = Quaternion::Concatenate(inc, q);
	SetRotation(q);
}

Plane::Plane(Game* game, std::string filename) : Actor(game)
{
	SetScale(10.0f);
	MeshComponent* mc = new MeshComponent(this);
	mc->SetMesh(game->GetRenderer()->GetMesh(filename));
}