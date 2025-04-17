#pragma once
#include <dwrite.h>
#include <wrl.h>
#include <unordered_map>
#include "../../../../Engine/Graphics/Font.h"

using Microsoft::WRL::ComPtr;
using namespace std;

class CustomFontLoader;
class CustomFontCollectionLoader;

namespace Engine {

class Font_D2D : public Font {
public:
	Font_D2D(const string& filepath, IDWriteFactory* f);
	
	inline IDWriteFactory* GetFactory() const { return factory.Get(); }
	IDWriteTextFormat* GetFormat(const uint size);
	inline IDWriteFontFace* GetFace() const { return font_face.Get(); }
	inline IDWriteFontCollection* GetFCollection() const { return font_collection.Get(); }

private:
	ComPtr<IDWriteFactory> factory;
	wstring name;
	std::unordered_map<uint, ComPtr<IDWriteTextFormat>> formats;
	ComPtr<IDWriteFontFace> font_face;
	ComPtr<IDWriteFontCollection> font_collection;
	ComPtr<CustomFontLoader> font_loader;
	ComPtr<CustomFontCollectionLoader> collection_loader;
};
}