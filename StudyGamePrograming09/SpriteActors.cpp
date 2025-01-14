#include "SpriteActors.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Renderer.h"

SpriteActors::SpriteActors(Game* game) : Actor(game)
{
	Actor* a = new Actor(game);
	a->SetPosition(Vector3(-350.0f, -350.0f, 0.0f));
	SpriteComponent* sc = new SpriteComponent(a);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/HealthBar.png"));

	a = new Actor(game);
	a->SetPosition(Vector3(375.0f, -275.0f, 0.0f));
	a->SetScale(0.75f);
	sc = new SpriteComponent(a);
	sc->SetTexture(game->GetRenderer()->GetTexture("Assets/Radar.png"));
}