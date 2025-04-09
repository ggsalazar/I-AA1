#pragma once
#include "../Core/Vector2.h"

namespace Engine {

class Spritesheet {
public:
	virtual ~Spritesheet() {}

	virtual bool LoadFromFile(const std::string& file) = 0;
	virtual Vector2u GetSize() const = 0;
	//virtual void Bind() const = 0; //Optional for GPU acceleration
};
}