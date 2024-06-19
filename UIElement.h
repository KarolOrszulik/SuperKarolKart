#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class UIElement :
	public sf::Drawable,
	public sf::Transformable
{
public:
	UIElement() = default;
	virtual ~UIElement() = default;

	enum class Origin
	{
		TOP_LEFT,
		MID_LEFT,
		BOT_LEFT,
		TOP_CENTER,
		MID_CENTER,
		BOT_CENTER,
		TOP_RIGHT,
		MID_RIGHT,
		BOT_RIGHT
	};

	sf::Color getBgColor() const { return m_background.getFillColor(); };
	sf::Vector2f getSize() const { return m_background.getSize(); };
	float getWidth() const { return m_background.getSize().x; };
	float getHeight() const{ return m_background.getSize().y; };
	

	void setBgColor(const sf::Color& color)
		{ m_background.setFillColor(color); };

	void setBgOutlineColor(const sf::Color& color)
		{ m_background.setOutlineColor(color); };

	void setBgOutlineThickness(float thickness)
		{ m_background.setOutlineThickness(thickness); };

	void setWidth(float width)
		{ m_background.setSize(sf::Vector2f{ width, getHeight() }); };

	void setHeight(float height)
		{ m_background.setSize(sf::Vector2f{ getWidth(), height }); };

	void setSize(sf::Vector2f size);
	void setOrigin(Origin origin);

	bool contains(sf::Vector2f point) const;
	bool contains(sf::Vector2i point) const;

	void centerHorizontally(float parentWidth);

	void centerVertically(float parentHeight);
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void handleEvent(sf::Event& event) = 0;

protected:
	sf::Vector2i translateRelativeToElement(sf::Vector2i globalPoint) const;

private:
	Origin m_origin = Origin::TOP_LEFT;
	sf::RectangleShape m_background;
};