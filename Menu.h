#pragma once

#include "UIElement.h"

class Menu
	: public UIElement
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void handleEvent(sf::Event& event) override;
	void addElement(std::shared_ptr<UIElement> element);

private:
	std::vector<std::shared_ptr<UIElement>> elements;
};

