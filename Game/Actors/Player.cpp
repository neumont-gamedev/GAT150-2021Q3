#include "Player.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Engine.h"
#include <memory>

Player::Player(const nc::Transform& transform, std::shared_ptr<nc::Texture> texture, float speed) : nc::Actor{ transform, texture }, speed{ speed } 
{
}

void Player::Initialize()
{
	std::unique_ptr locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ -20, 0 };
	locator->transform.localRotation = nc::DegToRad(180);
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 15, 16 };
	AddChild(std::move(locator));

	locator = std::make_unique<Actor>();
	locator->transform.localPosition = nc::Vector2{ 15, -16 };
	AddChild(std::move(locator));
}

void Player::Update(float dt)
{
	Actor::Update(dt);


	// movement
	float thrust = 0;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_A) == nc::InputSystem::eKeyState::Held) transform.rotation -= 5 * dt;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_D) == nc::InputSystem::eKeyState::Held) transform.rotation += 5 * dt;
	if (scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_W) == nc::InputSystem::eKeyState::Held) thrust = speed;

	// acceleration
	nc::Vector2 acceleration;
	acceleration += nc::Vector2::Rotate(nc::Vector2::right, transform.rotation) * thrust;
	velocity += acceleration * dt;
	transform.position += velocity * dt;

	velocity *= 0.985f; // damping

	transform.position.x = nc::Wrap(transform.position.x, 0.0f, 800.0f);
	transform.position.y = nc::Wrap(transform.position.y, 0.0f, 600.0f);

	// fire
	fireTimer -= dt;
	if (fireTimer <= 0 && scene->engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
	{
		fireTimer = fireRate;

		{
			nc::Transform t = children[1]->transform;
			t.scale = 1;
			
			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spaceMissiles_001.png", scene->engine->Get<nc::Renderer>()), 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}

		{
			nc::Transform t = children[2]->transform;
			t.scale = 1;

			std::unique_ptr<Projectile> projectile = std::make_unique<Projectile>(t, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spaceMissiles_001.png", scene->engine->Get<nc::Renderer>()), 600.0f);
			projectile->tag = "Player";
			scene->AddActor(std::move(projectile));
		}

		scene->engine->Get<nc::AudioSystem>()->PlayAudio("player_fire");
	}

	scene->engine->Get<nc::ParticleSystem>()->Create(children[0]->transform.position, 2, 1, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", scene->engine->Get<nc::Renderer>()), 50, children[0]->transform.rotation, nc::DegToRad(30));
}

void Player::OnCollision(Actor* actor)
{
	if (dynamic_cast<Enemy*>(actor) || (dynamic_cast<Projectile*>(actor) && nc::string_tolower(actor->tag) == "enemy"))
	{
		actor->destroy = true;
		destroy = true;
		scene->engine->Get<nc::ParticleSystem>()->Create(transform.position, 10, 1, scene->engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle02.png", scene->engine->Get<nc::Renderer>()), 100);
		scene->engine->Get<nc::AudioSystem>()->PlayAudio("explosion");

		nc::Event event;
		event.name = "PlayerDead";
		scene->engine->Get<nc::EventSystem>()->Notify(event);
	}
}