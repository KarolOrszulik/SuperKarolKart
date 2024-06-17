#pragma once

#include "GameObject.h"
#include "Track.h"

class Vehicle : public GameObject
{
public:
	Vehicle(Track* track, sf::Vector2f position = { 0.0f, 0.0f });

	void applyAccelerator(float accelerator);
	void applySteering(float steering);
	void applyUse();
	void applySkill();

	size_t getCompletedLaps() const { return m_completedLaps; }
	float getAngle() const { return m_angle; }

	void update(float dt);
	void draw(sf::RenderTarget& window);

	void setSpeedMultiplier(float speedMultiplier, float time);

protected:
	void handleCheckpoints();
	void handleGroundItems();
	void handleClearMultiplier(float dt);
	void handleItemUse();
	virtual void handleMovement(float dt) = 0;

	Track* m_track = nullptr;
	size_t m_nextCheckpoint = 0;
	size_t m_completedLaps = 0;

	float m_angle = 0.0f;
	
	float m_speedMultiplier = 1.0f;
	float m_timeToClearMultiplier = 0.0f;
	
	float m_acceleratorInput = 0.0f;
	float m_steeringInput = 0.0f;
	bool m_use = false;
	bool m_skill = false;

	sf::Texture m_texture;
	virtual int getTextureOffset() = 0;
};

