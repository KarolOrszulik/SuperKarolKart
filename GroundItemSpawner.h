#pragma once

#include "GameObject.h"
class GroundItem; // nie mo¿na daæ #include "GroundItem.h" bo to jest cykliczna zale¿noœæ z GroundItem.h

class GroundItemSpawner : public GameObject
{
public:
	GroundItemSpawner(sf::Vector2f position, std::unique_ptr<GroundItem> prototype);

	void update(float dt) override;
	void draw(sf::RenderTarget& window) override;

	void notifyObjectDeleted();

private:
	std::filesystem::path getTexturePath() const override { return "assets/notexture.png"; }

	static constexpr float SPAWN_INTERVAL = 3.0f;

	const std::unique_ptr<GroundItem> m_prototype;
	float m_timeToSpawn = 0.0f;
};

