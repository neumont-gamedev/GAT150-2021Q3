#include "SpriteAnimationComponent.h"
#include "Graphics/Renderer.h"
#include "Object/Actor.h"

namespace nc
{
	void SpriteAnimationComponent::Update()
	{
	}

	void SpriteAnimationComponent::Draw(Renderer* renderer)
	{
		renderer->Draw(texture, rect, owner->transform);
	}
}