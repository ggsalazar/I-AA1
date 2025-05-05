#pragma once
#include "../Core/Vector2.h"

namespace Engine {

class Window {
public:
	virtual ~Window() = default;

	virtual void PollEvents() = 0;

	virtual Vector2u ScreenSize() = 0;
	inline virtual Vector2u WinSize() const { return win_size; }

protected:
	Vector2u win_size;
};

}