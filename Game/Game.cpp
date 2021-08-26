#include "Game.h"
#include "GameComponent/PlayerComponent.h"

void Game::Initialize()
{
	// create engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600);

	// register classes
	REGISTER_CLASS(PlayerComponent);

	// create scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SeedRandom(static_cast<unsigned int>(time(nullptr)));
	nc::SetFilePath("../Resources");

	rapidjson::Document document;
	bool success = nc::json::Load("scene.txt", document);
	assert(success);

	scene->Read(document);
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
