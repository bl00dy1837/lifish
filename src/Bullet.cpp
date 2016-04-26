#include "Bullet.hpp"
#include "Moving.hpp"
#include "Sounded.hpp"
#include "Drawable.hpp"
#include "Killable.hpp"
#include "Game.hpp"
#include "utils.hpp"

using Game::Bullet;

//Bullet::Bullet(const sf::Vector2f& _pos, const std::string& texture_name) :
	//Game::MovingEntity(_pos, texture_name),
	//Game::Sounded({ Game::getAsset("test", "bossbullet_hit.ogg"),
			//Game::getAsset("test", "bossbullet_shot.ogg") }),
	//size(0),
	//range(-1),
	//damage(0),
	//shift(0.f, 0.f)
//{
	//// TODO
//}

Bullet::Bullet(const sf::Vector2f& _pos, const Game::Direction dir, unsigned short id,
		float _speed, unsigned short _damage, short _range)
	: Game::Entity(pos)
	, range(_range)
	, origin(_pos)
	, damage(_damage)
	, shift(0.f, 0.f)
	, direction(dir)
{
	addComponent(new Game::Moving(this, BASE_SPEED * _speed));
	addComponent(new Game::Sounded(this, {
		Game::getAsset("test", std::string("bullet") + Game::to_string(id) + std::string("_hit.ogg")),
		Game::getAsset("test", std::string("bullet") + Game::to_string(id) + std::string("_shot.ogg"))
	}));
	auto animated = addComponent(new Game::Animated(this, Game::getAsset("test", "bullets.png")));
	addComponent(new Game::Drawable(this, animated));
	addComponent(new Game::Killable(this, [this] () { _destroy(); }));

	// Bullets have a variable number of frames, up to 13:
	// motion frames: 1 ~ 8 (max 8 / directional per direction)
	// destroy frames: 0 ~ 5
	// TODO: refactor
	BulletPresets::setup(*this, id);

	unsigned short d = 0;
	switch (dir) {
	case Game::Direction::DOWN: 
		d = 0;
		pos.x += (TILE_SIZE - size) / 2;
		pos.y += TILE_SIZE;
		break;
	case Game::Direction::UP:    
		d = directionality == 4 ? 1 : 0;
		pos.x += (TILE_SIZE - size) / 2;
		pos.y -= size;
		break;
	case Game::Direction::RIGHT:
		d = directionality == 4 ? 2 : directionality == 2 ? 1 : 0; 
		pos.y += (TILE_SIZE - size) / 2;
		pos.x += TILE_SIZE;
		break;
	case Game::Direction::LEFT: 
		d = directionality == 4 ? 3 : directionality == 2 ? 1 : 0;
		pos.y += (TILE_SIZE - size) / 2;
		pos.x -= size;
		break;
	default: 
		break;
	}

	auto& a_move = animated->addAnimation("move");
	auto& a_destroy = animated->addAnimation("destroy");

	// Since the bullet cannot change direction, we only need to setup these 2 animations.
	// The motion animation has up to 8 / directionality frames (but can also have less),
	// while the destroy animation can have from 0 to 5 frames.
	// Format for the spritesheet is:
	// 	- if directionality == 1, [motion frames] [destroy frames]
	//	- if == 2, [up/down frames] [left/right frames] [destroy frames]
	//	- if == 4, [down] [up] [right] [left] [destroy]
	unsigned short j = 0;
	for (unsigned short i = 0; i < nMotionFrames && i < 8 / directionality; ++i)
		a_move.addFrame(sf::IntRect(
				(nMotionFrames * d + j++) * TILE_SIZE, 
				(id-1) * TILE_SIZE, 
				TILE_SIZE, 
				TILE_SIZE));

	// destroy animations are non-directional
	for (unsigned short i = j; i < j + nDestroyFrames && i < j + 5; ++i)
		a_destroy.addFrame(sf::IntRect(
				(nMotionFrames * directionality + i) * TILE_SIZE, 
				(id-1) * TILE_SIZE, 
				TILE_SIZE, 
				TILE_SIZE));

	auto& animatedSprite = animated->getSprite();
	animatedSprite.setPosition(pos);
	animatedSprite.setAnimation(animations[0]);
	animatedSprite.setLooped(true);
	animatedSprite.setFrameTime(sf::seconds(0.10));
	animatedSprite.play();

	/*
	switch (direction) {
	case Direction::UP:
		shift.y -= speed;
		break;
	case Direction::LEFT:
		shift.x -= speed;
		break;
	case Direction::DOWN:
		shift.y += speed;
		break;
	case Direction::RIGHT:
		shift.x += speed;
		break;
	case Direction::NONE:
		return;
	}
	*/
}

//void Bullet::move() {
	//if (direction == Game::Direction::NONE) {
		//animatedSprite.update(frameClock.restart());
		//return;
	//}
	//if (range > 0 && (Game::abs(pos.x - origin.x) > Game::TILE_SIZE * range 
				//|| Game::abs(pos.y - origin.y) > Game::TILE_SIZE * range)) {
		//destroy();
		//return;
	//}

	//sf::Time frameTime = frameClock.restart();

	//if (!colliding) {
		//animatedSprite.move(shift * frameTime.asSeconds());
		//pos = animatedSprite.getPosition();
		//// Ensure we are always aligned at least for one frame for
		//// each tile we step in (this may not be the case if FPS are too low)
		//_ensureAlign();
	//} 
	//animatedSprite.update(frameTime);
//}

//bool Bullet::hits(const sf::Vector2f& epos) const {
	//sf::FloatRect me(pos.x, pos.y, size, size);
	//sf::FloatRect it(epos.x, epos.y, TILE_SIZE, TILE_SIZE);
	//return me.intersects(it);
//}

void Bullet::_destroy() {
	auto& animatedSprite = get<Game::Animated>()->getSprite();
	if (nDestroyFrames > 0) {
		switch (direction) {
		case Game::Direction::UP: case Game::Direction::DOWN:
			pos.x = Game::aligned(pos).x;
			break; 
		default:
			pos.y = Game::aligned(pos).y;
			break; 
		}
		animatedSprite.setPosition(pos);
		animatedSprite.play(animations[ANIM_DEATH]);
	}
	animatedSprite.setLooped(false);
	direction = Game::Direction::NONE;
}

void Game::BulletPresets::setup(Game::Bullet& b, unsigned short id) {
	switch (id) {
	case 1:
		// shot
		b.directionality = 1;
		b.size = 7;
		break;
	case 2:
		// fireball
		b.directionality = 1;
		b.size = 13;
		break;
	case 3:
		// MG shot
		b.directionality = 4;
		b.nMotionFrames = 1;
		b.nDestroyFrames = 5;
		b.size = 10;
		break;
	case 4:
		// lightbolt
		b.directionality = 1;
		b.size = 13;
		break;
	case 5:
		// flame
		b.directionality = 2;
		b.nMotionFrames = 5;
		b.nDestroyFrames = 0;
		b.size = TILE_SIZE;
		break;
	case 6:
		// plasma
		b.directionality = 1;
		b.size = 12;
		break;
	case 7:
		// magma
		b.directionality = 4;
		b.size = 26;
		break;
	default:
		break;
	}
}
