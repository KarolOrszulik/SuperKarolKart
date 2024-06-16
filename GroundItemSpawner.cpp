#include "GroundItemSpawner.h"
#include "Engine.h"
#include "GroundItem.h"

#include <iostream>

GroundItemSpawner::GroundItemSpawner(sf::Vector2f position, std::unique_ptr<GroundItem> prototype)
	: GameObject(position), m_prototype(std::move(prototype))
{
	m_prototype->setSpawner(this);
}

void GroundItemSpawner::update(float dt)
{
	m_timeToSpawn -= dt;

	if (m_timeToSpawn <= 0.f)
	{
		auto item = m_prototype->copy();
		item->setPosition(m_position);

		std::cout << "GroundItemSpawner " << this << " spawned item " << m_prototype.get()
			<< " at (" << item->getPosition().x << ", " << item->getPosition().y << ")" << std::endl;

		Engine::getInstance()->addObject(std::move(item));
		m_timeToSpawn = 1e6;
	}
}

void GroundItemSpawner::draw(sf::RenderTarget& window)
{
	//sf::RectangleShape shape({ 10.f, 10.f });
	//shape.setPosition(m_position);
	//shape.setFillColor(sf::Color::Yellow);
	//window.draw(shape);
}

void GroundItemSpawner::notifyObjectDeleted()
{
	m_timeToSpawn = SPAWN_INTERVAL;
}