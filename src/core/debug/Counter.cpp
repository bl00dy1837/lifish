#include "Counter.hpp"
#include <exception>

using Game::Debug::Counter;

int Counter::inc(std::string name) {
	return ++counts[name]; 
}

void Counter::reset(std::string name) {
	set(name, 0);
}

void Counter::set(std::string name, int val) {
	counts[name] = val;
}

int Counter::get(std::string name) const {
	auto it = counts.find(name);
	if (it == counts.end())
		throw std::invalid_argument(name + ": get() called before creating key!");
	return it->second;
}

int Counter::safeGet(std::string name) const {
	auto it = counts.find(name);
	if (it == counts.end())
		return -1;
	return it->second;
}