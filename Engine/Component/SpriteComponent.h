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

		virtual bool Write(const rapidjson::Value& value) const override;
		virtual bool Read(const rapidjson::Value& value) override;

	public:
		std::shared_ptr<Texture> texture;
	};
}