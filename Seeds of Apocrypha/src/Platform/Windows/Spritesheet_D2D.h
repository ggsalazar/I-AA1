#pragma once
#include <d2d1.h>
#include "../../Engine/Graphics/Spritesheet.h"

namespace Engine {

class Spritesheet_D2D : public Spritesheet {
public:
	Spritesheet_D2D(ID2D1HwndRenderTarget* r_t) : render_target(r_t) {}
	~Spritesheet_D2D() {}

	bool LoadFromFile(const std::string& file) override;
	inline Vector2u GetSize() const override { return size; }

	ID2D1Bitmap* GetBitmap() const { return bitmap; }

private:
	ID2D1HwndRenderTarget* render_target;
	ID2D1Bitmap* bitmap = nullptr; //Was formerly using ComPtr, stopped because it was being fucky
	Vector2u size = { 0,0 };
};

}