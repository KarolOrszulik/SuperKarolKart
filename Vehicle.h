#pragma once

#include "GameObject.h"
#include "Track.h"

class Vehicle : public GameObject
{
public:
	Vehicle(Track* track, sf::Vector2f position = { 0.0f, 0.0f })
		: GameObject(position), m_track(track) {}

	void applyAccelerator(float accelerator);
	void applySteering(float steering);
	void applyUse();
	void applySkill();

	size_t getCompletedLaps() const { return m_completedLaps; }
	float getAngle() const { return m_angle; }

	void update(float dt) override;
	void draw(sf::RenderTarget& window) override;

	void setSpeedMultiplier(float speedMultiplier, float time);

private:
	float m_acceleration = 0.0f;
	float m_steering = 0.0f;
	float m_angle = 0.0f;
	Track* m_track = nullptr;
	size_t m_nextCheckpoint = 0;
	size_t m_completedLaps = 0;
	float m_speedMultiplier = 1.0f;
	float m_timeToClearMultiplier = 0.0f;
	bool m_use = false;
	bool m_skill = false;
};

