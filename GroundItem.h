#pragma once

#include "GameObject.h"
#include "Vehicle.h"
#include "GroundItemSpawner.h"


class GroundItem : public GameObject
{
public:
	GroundItem(const sf::Vector2f& velocity, bool singleUse) 
		: m_velocity(velocity), m_singleUse(singleUse) {};
	virtual void interactWithVehicle(Vehicle&);
	virtual std::unique_ptr<GroundItem> copy() const = 0;

	void setSpawner(GroundItemSpawner* spawner) { m_spawner = spawner; }

	bool isSingleUse() const { return m_singleUse; }

	~GroundItem() override;

protected:
	sf::Vector2f m_velocity;

private:
	GroundItemSpawner* m_spawner = nullptr;
	bool m_singleUse = false;
};

