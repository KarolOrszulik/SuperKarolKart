#pragma once

#include "UIElement.h"

class Menu
	: public UIElement
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void addElement(std::unique_ptr<UIElement> element);

private:
	std::vector<std::unique_ptr<UIElement>> elements;
};

