#pragma once
#include <string>
#include "Actor.h"
class MeshActors : public Actor
{
public:
	MeshActors(class Game* game);
};

class Dice : public Actor
{
public:
	Dice(class Game* game, std::string meshfile);
	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* mMoveComp;
};

class Sphere : public Actor
{
public:
	Sphere(class Game* game, std::string meshfile);
	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* mMoveComp;
};

class RacingCar : public Actor
{
public:
	RacingCar(class Game* game, std::string meshfile);
	void UpdateActor(float deltaTime) override;

private:
	class MoveComponent* mMoveComp;
};

class Plane : public Actor
{
public:
	Plane(class Game* game, std::string meshfile);
};