#pragma once
#include "Object/Actor.h"

class Enemy : public nc::Actor
{
public:
	Enemy(const nc::Transform& transform, std::shared_ptr<nc::Texture> texture, float speed, float fireRate) : nc::Actor{ transform, texture }, speed{ speed }, fireRate{ fireRate } 
	{
		fireTimer = fireRate;
	}

	void Update(float dt) override;
	void OnCollision(Actor* actor) override;

private:
	float speed{ 300 };
	float fireTimer{ 4 };
	float fireRate{ 1 };
};