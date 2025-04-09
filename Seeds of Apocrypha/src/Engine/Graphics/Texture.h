#pragma once
#include "../Core/Vector2.h"

namespace Engine {

class Texture {
public:
	virtual ~Texture() {}

	virtual bool LoadFromFile(const std::string& file) = 0;
	virtual Vector2u GetSize() const = 0;
	virtual void Bind() const = 0; //Optional for GPU acceleration
};
}