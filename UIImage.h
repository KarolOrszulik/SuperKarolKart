#pragma once

#include "UIButton.h"
#include <optional>

class UIImage : public UIButton
{
public:
	UIImage(Style normal, const sf::Texture& texture);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void assignTexture(const sf::Texture& texture);
	void expandImageToSize();
	const sf::Texture& getTexture();

	void applyPadding(unsigned padding) override;

private:
	sf::Sprite m_sprite;
	unsigned m_padding = 0;
};

