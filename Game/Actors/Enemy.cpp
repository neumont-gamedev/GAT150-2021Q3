#include "Enemy.h"
#include "Projectile.h"
#include "Player.h"
#include "Engine.h"

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	// ai
	if (scene->GetActor<Player>())
	{
		nc::Vector2 direction = scene->GetActor<Player>()->transform.position - transform.position;
		nc::Vector2 forward = nc::Vector2::Rotate(nc::Vector2::right, transform.rotation);

		float turnAngle = nc::Vector2::SignedAngle(forward, direction.Normalized());
		transform.rotation = transform.rotation + turnAngle * (3 * dt);
		float angle = nc::Vector2::Angle(forward, direction.Normalized());

		fireTimer -= dt;
		if (fireTimer <= 0 && angle <= nc::DegToRad(10))
		{
			fireTimer = fireRate;

			nc::Transform t = transform;
			t.scale = 1.0f;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spaceMissiles_001.png", scene->engine->Get<nc::Renderer>()), 300.0f);
			projectile->tag = "Enemy";
			scene->AddActor(std::move(projectile));

			scene->engine->Get<nc::AudioSystem>()->PlayAudio("enemy_fire");
		}
	}

	// transform
	transform.position += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * speed * dt;
	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	// effects
	scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 2, 1, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>()), 50, transform.rotation, nc::DegToRad(30));
	//scene->engine->Get<nc::ParticleSystem>()->Create(children[0]->transform.position, 2, 1, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>()), 50, children[0]->transform.rotation, nc::DegToRad(30));
}

void Enemy::OnCollision(Actor* actor)
{
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Player")
	{
		actor->destroy = true;

		destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 10, 1, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle02.png", scene->engine->Get<nc::Renderer>()), 100);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "AddPoints";
		event.data = 300;
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}
