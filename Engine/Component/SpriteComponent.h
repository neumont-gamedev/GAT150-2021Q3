#pragma once
#include "GraphicsComponent.h"
#include <memory>

namespace nc
{
	class Texture;

	class SpriteComponent : public GraphicsComponent
	{
	public:
		virtual void Update() override;
		virtual void Draw(Renderer* renderer) override;

	public:
		std::shared_ptr<Texture> texture;
	};
}