#include "Projectile.h"
#include "Engine.h"

void Projectile::Update(float dt)
{
	Actor::Update(dt);

	lifetime -= dt;
	destroy = (lifetime <= 0);

	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	std::shared_ptr<nc::Texture> texture = (nc::string_tolower(tag) == "player") ? scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle04.png", scene->engine->Get<nc::Renderer>()) : scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle03.png", scene->engine->Get<nc::Renderer>());

	scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 1, 0.5f, texture, 50, transform.rotation, nc::DegToRad(30));
}
