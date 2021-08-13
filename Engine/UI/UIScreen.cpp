#include "UIScreen.h"
#include "Object/Scene.h"
#include <algorithm>

namespace nc
{
	void UIScreen::Update(float dt)
	{
		// update widgets
		std::for_each(widgets.begin(), widgets.end(), [dt](auto& widget) { widget->Update(dt); });
	}

	void UIScreen::Draw(Renderer* renderer)
	{
		std::for_each(widgets.begin(), widgets.end(), [renderer](auto& widget) { widget->Draw(renderer); });
	}

	void UIScreen::AddWidget(std::unique_ptr<UIWidget> widget)
	{
		widget->screen = this;
		widgets.push_back(std::move(widget));
	}
}