#pragma once
#include "UIWidget.h"
#include <vector>
#include <memory>

namespace nc
{
	class Engine;
	class UIWidget;
	class Renderer;

	class UIScreen
	{
	public:
		void Update(float dt);
		void Draw(Renderer* renderer);

		void AddWidget(std::unique_ptr<UIWidget> widget);

		template<typename T>
		T* GetWidget(const std::string& name);

	public:
		Engine* engine{ nullptr };

	private:
		std::vector<std::unique_ptr<UIWidget>> widgets;
	};

	template<typename T>
	inline T* UIScreen::GetWidget(const std::string& name)
	{
		for (auto& widget : widgets)
		{
			if (istring_compare(widget->name, name)) return dynamic_cast<T*>(widget.get());
		}

		return nullptr;
	}
}

