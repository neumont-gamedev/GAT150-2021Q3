#include "Game.h"
#include "Actors/Player.h"
#include "Actors/Enemy.h"
#include "Actors/Projectile.h"

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

	// get font from resource system
	int size = 24;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("fonts/arcadeclassic.ttf", &size);

	std::shared_ptr<nc::Texture> titleTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	titleTexture->Create(font->CreateSurface("VECTREX", nc::Color{ 1, 1, 1 }));
	engine->Get<nc::ResourceSystem>()->Add("titleTexture", titleTexture);

	std::shared_ptr<nc::Texture> gameoverTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	gameoverTexture->Create(font->CreateSurface("GAME  OVER", nc::Color{ 1, 1, 1 }));
	engine->Get<nc::ResourceSystem>()->Add("gameoverTexture", gameoverTexture);

	std::shared_ptr<nc::Texture> scoreTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	scoreTexture->Create(font->CreateSurface("0000", nc::Color{ 1, 1, 1 }));
	engine->Get<nc::ResourceSystem>()->Add("scoreTexture", scoreTexture);

	engine->Get<nc::AudioSystem>()->AddAudio("music", "audio/music.mp3");
	musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true);

	//scene->AddActor(std::make_unique<nc::Actor>(nc::Transform(nc::Vector2(400.0f, 300.0f), 0.0f, 1.0f), engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spacebg.png", engine->Get<nc::Renderer>()), 300.0f));

	// game
	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("player_fire", "audio/player_fire.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("enemy_fire", "audio/enemy_fire.wav");

	engine->Get<nc::EventSystem>()->Subscribe("AddPoints", std::bind(&Game::OnAddPoints, this, std::placeholders::_1));
	engine->Get<nc::EventSystem>()->Subscribe("PlayerDead", std::bind(&Game::OnPlayerDead, this, std::placeholders::_1));
}

void Game::Shutdown()
{
	scene->RemoveAllActors();
	engine->Shutdown();
}

void Game::Update()
{
	engine->Update();
	stateTimer += engine->time.deltaTime;

	switch (state)
	{
	case Game::eState::Title:
		if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_SPACE) == nc::InputSystem::eKeyState::Pressed)
		{
			state = eState::StartGame;
		}
		break;
	case Game::eState::StartGame:
		score = 0;
		lives = 3;
		state = eState::StartLevel;
		break;
	case Game::eState::StartLevel:
		UpdateStartLevel();
		state = eState::Game;
		break;
	case Game::eState::Game:
		spawnTimer -= engine->time.deltaTime;
		if (spawnTimer <= 0)
		{
			spawnTimer = spawnTime;
			spawnTime -= 0.1f;
			spawnTime = std::max(1.0f, spawnTime);

			nc::Actor* actor = scene->GetActor<Player>();
			if (actor)
			{
				//float angle = nc::RandomRange(0, nc::TwoPi);
				//nc::Vector2 position = actor->transform.position + nc::Vector2::Rotate(nc::Vector2{ 300, 0 }, angle);
				nc::Vector2 position;
				if (nc::RandomRangeInt(0, 2) == 0)
				{
					position.x = (nc::RandomRangeInt(0, 2) == 0) ? 0.0f : 800.0f;
					position.y = nc::RandomRange(0, 600);
				}
				else
				{
					position.x = nc::RandomRange(0, 800);
					position.y = (nc::RandomRangeInt(0, 2) == 0) ? 0.0f : 600.0f;
				}
				scene->AddActor(std::make_unique<Enemy>(nc::Transform{ position, nc::RandomRange(0.0f, nc::TwoPi), 1.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spaceShips_004.png", engine->Get<nc::Renderer>()), 100.0f, 2.0f + spawnTime));
			}


		}
		break;
	case Game::eState::PlayerDead:
		if (stateTimer >= 2)
		{
			scene->RemoveAllActors();
			state = eState::StartLevel;
		}
		break;
	case Game::eState::GameOver:
		if (stateTimer >= 5)
		{
			scene->RemoveAllActors();
			state = eState::Title;
		}
		break;
	default:
		break;
	}

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	// background
	{
		nc::Transform t;
		t.position = { 400, 300 };
		t.scale = { 0.5f, 0.5f };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spacebg.png", engine->Get<nc::Renderer>()), t);
	}

	switch (state)
	{
	case Game::eState::Title:
	{
		nc::Transform t;
		t.position = { 400, 300 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("titleTexture"), t);
	}
		break;
	case Game::eState::GameOver:
	{
		nc::Transform t;
		t.position = { 400, 300 };
		engine->Get<nc::Renderer>()->Draw(engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("gameoverTexture"), t);
	}
		break;
	default:
		break;
	}

	// score text
	nc::Transform t;
	t.position = { 50, 20 };
	std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("scoreTexture");
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("fonts/arcadeclassic.ttf");
	texture->Create(font->CreateSurface(std::to_string(score), nc::Color{ 1, 1, 1 }));
	engine->Get<nc::Renderer>()->Draw(texture, t);

	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::UpdateStartLevel()
{
	scene->AddActor(std::make_unique<Player>(nc::Transform(nc::Vector2(400.0f, 300.0f), 0.0f, 1.0f), engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spaceShips_008.png", engine->Get<nc::Renderer>()), 300.0f));
	for (size_t i = 0; i < 2; i++)
	{
		nc::Vector2 position;
		if (nc::RandomRangeInt(0, 2) == 0)
		{
			position.x = (nc::RandomRangeInt(0, 2) == 0) ? 0.0f : 800.0f;
			position.y = nc::RandomRange(0, 600);
		}
		else
		{
			position.x = nc::RandomRange(0, 800);
			position.y = (nc::RandomRangeInt(0, 2) == 0) ? 0.0f : 600.0f;
		}
		//float angle = nc::RandomRange(0, nc::TwoPi);
		//nc::Vector2 position = nc::Vector2::Rotate(nc::Vector2{ 300, 0 }, angle);

		scene->AddActor(std::make_unique<Enemy>(nc::Transform{ position, nc::RandomRange(0.0f, nc::TwoPi), 1.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spaceShips_004.png", engine->Get<nc::Renderer>()), 100.0f, 5.0f));
	}
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives--;
	stateTimer = 0;
	state = (lives == 0)? eState::GameOver : eState::PlayerDead;
}
