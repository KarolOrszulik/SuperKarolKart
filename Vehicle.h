#pragma once

#include "GameObject.h"
#include "Track.h"
#include "PowerUp.h"
#include "BowlingBall.h"
class Vehicle : public GameObject
{
public:
	struct Input;

	Vehicle(Track* track, sf::Vector2f position = { 0.0f, 0.0f });

	//void applyAccelerator(float accelerator);
	//void applySteering(float steering);
	//void applyUse();
	//void applySkill();
	void applyInput(const Input& input) { m_input = input; }

	void setPowerUp(std::unique_ptr<PowerUp> powerUp) { m_powerUp = std::move(powerUp); }

	size_t getCompletedLaps() const { return m_completedLaps; }
	float getAngle() const { return m_angle; }
	sf::Vector2f getUnitVector() const { return { std::cos(m_angle), std::sin(m_angle) }; }

	virtual float getSpeed() const = 0;

	void update(float dt);
	void draw(sf::RenderTarget& window);

	void setSpeedMultiplier(float speedMultiplier, float time);
	void setSteeringMultiplier(float steeringMultiplier, float time);

	struct Input
	{
		float accelerator = 0.0f;
		float steering = 0.0f;
		bool use = false;
		bool skill = false;
	};
protected:

	virtual void handleMovement(float dt) = 0;
	void handleCheckpoints();				//
	void handleGroundItems();				//
	void handleClearMultiplier(float dt);	// te 4 chyba powinny byæ private
	void handleItemUse();					// TODO: rozpatrzeæ powy¿szy komentarz

	Track* m_track = nullptr;
	size_t m_nextCheckpoint = 0;
	size_t m_completedLaps = 0;

	float m_angle = 0.0f;
	
	float m_speedMultiplier = 1.0f;
	float m_timeToClearMultiplier = 0.0f;

	Input m_input;
	//float m_acceleratorInput = 0.0f;
	//float m_steeringInput = 0.0f;
	//bool m_use = false;
	//bool m_skill = false;

	float m_steeringMultiplier = 1.0f;
	float m_timeToClearSteeringMultiplier = 0.0f;

	std::unique_ptr<PowerUp> m_powerUp;

	sf::Texture m_texture;
	virtual int getTextureOffset() = 0;
};

