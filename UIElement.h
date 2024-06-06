#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


// MUSI BYÆ DZIEDZICZONE PO Drawable i Transformable!!!
// I DO TEGO AGREGOWAÆ RectangleShape
class UIElement :
	public sf::Drawable,
	public sf::Transformable
{
public:
	UIElement() = default;
	virtual ~UIElement() = default;

	sf::Color getBackgroundColor() const;
	sf::Vector2f getSize() const;
	float getWidth() const;
	float getHeight() const;
	

	void setBackgroundColor(const sf::Color& color);
	void setSize(sf::Vector2f size);
	void setWidth(float width);
	void setHeight(float height);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual void handleEvents(sf::Event& event) = 0;

private:
	sf::RectangleShape m_background;
};