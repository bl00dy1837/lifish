#pragma once

#include <ostream>

namespace Game {

enum class Direction {
	UP, LEFT, DOWN, RIGHT, NONE
};

std::ostream& operator<<(std::ostream& stream, const Direction& dir) {
	switch (dir) {
	case Direction::UP: stream << "UP"; break;
	case Direction::LEFT: stream << "LEFT"; break;
	case Direction::DOWN: stream << "DOWN"; break;
	case Direction::RIGHT: stream << "RIGHT"; break;
	default: stream << "NONE"; break;
	}
	return stream;
}

inline Direction oppositeDirection(const Direction dir) {
	switch (dir) {
	case Direction::UP:    return Direction::DOWN;
	case Direction::DOWN:  return Direction::UP;
	case Direction::LEFT:  return Direction::RIGHT;
	case Direction::RIGHT: return Direction::LEFT;
	default:               return Direction::NONE;
	}
}

}