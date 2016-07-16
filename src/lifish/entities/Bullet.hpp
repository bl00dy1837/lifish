#pragma once

#include "Component.hpp"
#include "Direction.hpp"
#include "Attack.hpp"

namespace Game {

class BulletPresets;

/**
 * A bullet travels until it impacts with another Entity which is not transparent to it.
 */
class Bullet : public Game::Entity {
protected:
	/** The actual size of this bullet in pixels */
	unsigned short size;

	/** The position this bullet was shot from (used for range) */
	const sf::Vector2f origin;

	/** The Entity that shot this bullet */
	const Game::Entity *const source;

	/** The damage dealt to the impacted Entity */
	unsigned short damage;

	/** How many pixels does this bullet travel; -1 means infinite. */
	float range;

	unsigned short nMotionFrames = 1,  // up to 2
		       nDestroyFrames = 4; // up to 5

	/** This constructor is used by BossBullet */
	//Bullet(const sf::Vector2f& pos, const std::string& texture_name);

	void _destroy();

public:
	constexpr static float BASE_SPEED = 200.f;

	/** Constructs a Bullet without a source (must specify the position) */
	explicit Bullet(const sf::Vector2f& pos, const Game::Attack& attack);

	/** Constructs a Bullet with a source Entity (using that Entity's position) */
	explicit Bullet(const Game::Entity *const source, const Game::Attack& attack);

	const Game::Entity* getSource() const { return source; }

	//bool hits(const sf::Vector2f& pos) const;

	unsigned short getDamage() const { return damage; }

	//unsigned short getSize() const { return size; }
};

}