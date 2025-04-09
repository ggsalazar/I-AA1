#pragma once
#include <d2d1.h>
#include "../../Engine/Graphics/Texture.h"

namespace Engine {
class Texture_D2D : public Texture {
public:
	Texture_D2D() {}
	~Texture_D2D() {}

	bool LoadFromFile(const std::string& file) override;
	inline Vector2u GetSize() const override { return size; }

	ID2D1Bitmap* GetBitmap() const { return bitmap; }

private:
	ID2D1Bitmap* bitmap = nullptr;
	Vector2u size = { 0, 0 };
};

}