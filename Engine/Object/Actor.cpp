#include "Actor.h"
#include <algorithm>

namespace nc
{
	void Actor::Update(float dt)
	{
		transform.Update();
		std::for_each(children.begin(), children.end(), [](auto& child) { child->transform.Update(child->parent->transform.matrix);  });
	}

	void Actor::Draw()
	{
	}

	float Actor::GetRadius()
	{
		return 0;
	}

	void Actor::AddChild(std::unique_ptr<Actor> actor)
	{
		actor->parent = this;
		children.push_back(std::move(actor));
	}
}