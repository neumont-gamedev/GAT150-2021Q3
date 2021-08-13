#pragma once
#include "Object/Object.h"
#include "Math/Transform.h"

namespace nc
{
	class UIScreen;
	class Renderer;

	class UIWidget : public Object
	{
	public:
		UIWidget() {}
		UIWidget(const std::string& name) : Object{name} {}

		virtual void Update(float dt) = 0;
		virtual void Draw(Renderer * renderer) = 0;

	public:
		Transform transform;
		Vector2 anchor;

		UIScreen* screen{ nullptr };
	};
}
