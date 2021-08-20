#include "Game.h"

//int global = 10;

void Game::Initialize()
{
	// create engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600);

	// create scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");

	// actors
	auto actor = nc::ObjectFactory::Instance().Create<nc::Actor>("Actor");
	actor->transform = nc::Transform{ nc::Vector2{400, 300}, 0, 1 };
	{
		auto component = nc::ObjectFactory::Instance().Create<nc::SpriteAnimationComponent>("SpriteAnimationComponent");
		component->texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sparkle.png", engine->Get<nc::Renderer>());
		component->fps = 16;
		component->numFramesX = 8;
		component->numFramesY = 8;
		actor->AddComponent(std::move(component));
	}
	{
		auto component = nc::ObjectFactory::Instance().Create<nc::PhysicsComponent>("PhysicsComponent");
		//component->ApplyForce(nc::Vector2::right * 200);
		actor->AddComponent(std::move(component));
	}
	scene->AddActor(std::move(actor));
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	engine->Get<nc::Renderer>()->EndFrame();
}
