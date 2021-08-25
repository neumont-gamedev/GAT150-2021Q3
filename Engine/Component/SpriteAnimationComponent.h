#pragma once
#include "SpriteComponent.h"
#include <SDL.h>

namespace nc
{
	class Texture;

	class SpriteAnimationComponent : public SpriteComponent
	{
	public:
		void Update() override;
		void Draw(Renderer* renderer) override;

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		int frame;
		int fps;

		int numFramesX;
		int numFramesY;

		float frameTime;
		float frameTimer;

		SDL_Rect rect;
	};
}