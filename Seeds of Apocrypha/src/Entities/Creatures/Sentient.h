#pragma once
#include "Creature.h"

class Sentient : public Creature {
public:

	void SetLevel(uint new_level);
	uint GetLevel() const { return level; }

private:
	uint level = 0;
};