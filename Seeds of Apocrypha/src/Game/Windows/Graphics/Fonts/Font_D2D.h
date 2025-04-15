#pragma once
#include <dwrite.h>
#include <wrl.h>
#include "../../../../Engine/Graphics/Font.h"

using Microsoft::WRL::ComPtr;

class CustomFontLoader;
class CustomFontCollectionLoader;

namespace Engine {

class Font_D2D : public Font {
public:
	Font_D2D(const std::string& filepath, const float char_size, IDWriteFactory* factory);

	inline IDWriteTextFormat* GetFormat() const { return text_format.Get(); }
	inline IDWriteFontFace* GetFace() const { return font_face.Get(); }
	inline IDWriteFontCollection* GetFCollection() const { return font_collection.Get(); }

private:
	ComPtr<IDWriteTextFormat> text_format;
	ComPtr<IDWriteFontFace> font_face;
	ComPtr<IDWriteFontCollection> font_collection;
	ComPtr<CustomFontLoader> font_loader;
	ComPtr<CustomFontCollectionLoader> collection_loader;
};
}