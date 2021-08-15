#include "Game.h"
#include "Actors/Player.h"

void Game::Initialize()
{
	// create engine
	engine = std::make_unique<nc::Engine>();
	engine->Startup();
	engine->Get<nc::Renderer>()->Create("GAT150", 800, 600);

	// create scene
	scene = std::make_unique<nc::Scene>();
	scene->engine = engine.get();

	nc::SetFilePath("../Resources");

	// get font from resource system
	int size = 24;
	std::shared_ptr<nc::Font> font = engine->Get<nc::ResourceSystem>()->Get<nc::Font>("fonts/arial.ttf", &size);

	// create font texture
	textTexture = std::make_shared<nc::Texture>(engine->Get<nc::Renderer>());
	// set font texture with font surface
	textTexture->Create(font->CreateSurface("hello world", nc::Color{ 1, 1, 1 }));
	// add font texture to resource system
	engine->Get<nc::ResourceSystem>()->Add("textTexture", textTexture);

	engine->Get<nc::AudioSystem>()->AddAudio("explosion", "audio/explosion.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("music", "audio/music.mp3");
	//musicChannel = engine->Get<nc::AudioSystem>()->PlayAudio("music", 1, 1, true);

	//std::shared_ptr<nc::Texture> texture = engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("sf2.png", engine->Get<nc::Renderer>());

	//for (size_t i = 0; i < 10; i++)
	//{
	//	nc::Transform transform{ nc::Vector2{nc::RandomRange(0, 800), nc::RandomRange(0, 600)}, nc::RandomRange(0, 360), 1.0f };
	//	std::unique_ptr<nc::Actor> actor = std::make_unique<nc::Actor>(transform, texture);
	//	scene->AddActor(std::move(actor));
	//}

	// game
	engine->Get<nc::AudioSystem>()->AddAudio("player_fire", "player_fire.wav");
	engine->Get<nc::AudioSystem>()->AddAudio("enemy_fire", "enemy_fire.wav");

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
		UpdateStartLevel(engine->time.deltaTime);
		state = eState::Game;
		break;
	case Game::eState::Game:
		//if (scene->GetActors<Enemy>().size() == 0)
		//{
		//	state = eState::GameOver;
		//}
		break;
	case Game::eState::GameOver:
		break;
	default:
		break;
	}

	if (engine->Get<nc::InputSystem>()->GetKeyState(SDL_SCANCODE_ESCAPE) == nc::InputSystem::eKeyState::Pressed)
	{
		quit = true;
	}

	if (engine->Get<nc::InputSystem>()->GetButtonState((int)nc::InputSystem::eMouseButton::Left) == nc::InputSystem::eKeyState::Pressed)
	{
		nc::Vector2 position = engine->Get<nc::InputSystem>()->GetMousePosition();
		// create particles
		engine->Get<nc::ParticleSystem>()->Create(position, 20, 3, engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("particle01.png", engine->Get<nc::Renderer>()), 100);
		engine->Get<nc::AudioSystem>()->PlayAudio("explosion", 1, nc::RandomRange(0.2f, 2.0f));
		musicChannel.SetPitch(nc::RandomRange(0.2f, 2.0f));
	}

	scene->Update(engine->time.deltaTime);
}

void Game::Draw()
{
	engine->Get<nc::Renderer>()->BeginFrame();

	switch (state)
	{
	case Game::eState::Title:
		//graphics.SetColor(nc::Color::red);
		//graphics.DrawString(380, 300 + static_cast<int>(std::sin(stateTimer * 6.0f) * 10.0f), "VECTREX");
		//graphics.SetColor(nc::Color::green);
		//graphics.DrawString(340, 360, "Press Space to Start");
		break;
	case Game::eState::StartGame:
		break;
	case Game::eState::StartLevel:
		break;
	case Game::eState::Game:
		break;
	case Game::eState::GameOver:
		//graphics.SetColor(nc::Color::red);
		//graphics.DrawString(380, 300, "GAME OVER");
		break;
	default:
		break;
	}

	//graphics.SetColor(nc::Color::white);
	//graphics.DrawString(30, 20, std::to_string(score).c_str());
	//graphics.DrawString(750, 20, std::to_string(lives).c_str());


	nc::Transform t;
	t.position = { 30, 30 };
	engine->Get<nc::Renderer>()->Draw(textTexture, t);

	engine->Draw(engine->Get<nc::Renderer>());
	scene->Draw(engine->Get<nc::Renderer>());

	engine->Get<nc::Renderer>()->EndFrame();
}

void Game::UpdateTitle(float dt)
{
	//if (Core::Input::IsPressed(VK_SPACE))
	//{
	//	stateFunction = &Game::UpdateStartLevel;
	//	//state = eState::StartGame;
	//}
}

void Game::UpdateStartLevel(float dt)
{
	scene->AddActor(std::make_unique<Player>(nc::Transform(nc::Vector2(400.0f, 300.0f), 0.0f, 1.0f), engine->Get<nc::ResourceSystem>()->Get<nc::Texture>("spaceShips_008.png", engine->Get<nc::Renderer>()), 300.0f));
	//for (size_t i = 0; i < 2; i++)
	//{
	//	scene->AddActor(std::make_unique<Enemy>(nc::Transform{ nc::Vector2{nc::RandomRange(0.0f, 800.0f), nc::RandomRange(0.0f, 600.0f)}, nc::RandomRange(0.0f, nc::TwoPi), 3.0f }, engine->Get<nc::ResourceSystem>()->Get<nc::Shape>("enemy.txt"), 100.0f));
	//}
}

void Game::OnAddPoints(const nc::Event& event)
{
	score += std::get<int>(event.data);
}

void Game::OnPlayerDead(const nc::Event& event)
{
	lives--;
	std::cout << std::get<std::string>(event.data) << std::endl;
	state = eState::GameOver;
}
