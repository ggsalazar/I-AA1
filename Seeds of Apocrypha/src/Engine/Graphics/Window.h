#pragma once
#include "../Core/Vector2.h"

namespace Engine {

class Window {
public:
	virtual ~Window() = default;

	virtual bool Create(const char* title, Vector2u size) = 0;
	virtual void Destroy() = 0;

	virtual void PollEvents() = 0;

	virtual void Clear() = 0;
	virtual Vector2u ScreenSize() = 0;

	virtual Vector2u GetSize() const = 0;
};

}