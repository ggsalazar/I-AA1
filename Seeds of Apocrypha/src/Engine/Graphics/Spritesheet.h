#pragma once
#include "../Core/Vector2.h"

namespace Engine {

class Spritesheet {
	friend class Sprite;
public:
	virtual ~Spritesheet() {};

	virtual bool LoadFromFile(const std::string& file) = 0;
	inline virtual Vector2u GetSize() const { return size; }
	//virtual void Bind() const = 0; //Optional for GPU acceleration

protected:
	Vector2u size = { 0, 0 };
};
}