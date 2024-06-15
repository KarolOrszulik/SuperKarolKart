#pragma once

#include "UIElement.h"

class Menu
	: public UIElement
{
public:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void addElement(std::unique_ptr<UIElement> element);

	virtual void handleEvent(sf::Event& event) override;

protected:
	
private:
	std::vector<std::shared_ptr<UIElement>> elements;
};

