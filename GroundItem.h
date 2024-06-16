#pragma once

#include "GameObject.h"
#include "Vehicle.h"
#include "GroundItemSpawner.h"


class GroundItem : public GameObject
{
public:
	virtual void interactWithVehicle(Vehicle&) = 0;
	virtual std::unique_ptr<GroundItem> copy() const = 0;

	void setSpawner(GroundItemSpawner* spawner) { m_spawner = spawner; }

	~GroundItem() override { m_spawner->notifyObjectDeleted(); }

private:
	GroundItemSpawner* m_spawner;
};

