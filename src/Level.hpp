#pragma once
/**
 * The Level class contains the template of a level, with all the
 * static information about it.
 */
#include <string>
#include <array>
#include <SFML/Graphics.hpp>
#include "Drawable.hpp"
#include "Game.hpp"
#include "FixedEntity.hpp"
#include "MovingEntity.hpp"
#include "EntityType.hpp"
#include "LevelSet.hpp"
#include "utils.hpp"

namespace Game {

class LevelSet;

class Level : public Game::Drawable {
	/** Types of bg texture tiles */
	enum : unsigned short {
		TILE_REGULAR = 0,
		TILE_UPPER_LEFT = 1,
		TILE_UPPER_RIGHT = 2,
		TILE_LOWER_LEFT = 3,
		TILE_LOWER_RIGHT = 4,
		TILE_UPPER = 5,
		TILE_LOWER = 6,
		TILE_LEFT = 7,
		TILE_RIGHT = 8,
	};
	/** The background texture */
	sf::Texture bgTexture;
	/** The sprites for the background tiles */
	std::array<sf::Sprite, 9> bgTiles;
	/** Time before "Hurry Up" */
	unsigned int time;
	/** The tileset to use */
	unsigned short tileset = 1;
	/** This level's static (initial) tilemap */
	Matrix<EntityType, LEVEL_HEIGHT, LEVEL_WIDTH> tiles;
	/** (Optional) LevelSet this level belongs to */
	const LevelSet *levelSet;

	/** Loads the texture from the file <texture_name> (or from memory, if
	 *  texture has already been cached)
	 */
	bool _loadTexture(const std::string& texture_name);
	/** Loads the content of bgTiles (bgTexture must already be set) */
	void _loadTiles();
	/** Whether this level has been initialized or not */
	bool initialized = false;
public:
	/** Constructs a level without a specified time and tileset. init() must
	 *  be called before using this level. Optionally, can specify a LevelSet
	 *  this Level belongs to.
	 */
	Level(const LevelSet *levelSet = nullptr);
	// XXX: testing constructor
	Level(const std::string& texture_name);
	virtual ~Level();

	/** Loads the appropriate bgTexture, fills the bgTiles and makes this level
	 *  usable. Must be called either after constructing it without parameters or 
	 * after changing them. Returns false if there were errors, true otherwise.
	 */
	bool init();

	bool isInitialized() const { return initialized; }

	/** Draws this level's background in the target window */
	void draw(sf::RenderTarget& window);

	unsigned short getTileset() const { return tileset; }
	void setTileset(unsigned short _tileset) { tileset = _tileset; }

	unsigned int getTime() const { return time; }
	void setTime(unsigned int _time) { time = _time; }

	/** Given a string representation of the level, sets its static tilemap
	 *  by filling the <entities> vector.
	 */
	EntityType getTile(unsigned short left, unsigned short top) const;
	bool setTilemap(const std::string& tilemap);

	void printInfo() const;
	void printTilemap() const;
};

}