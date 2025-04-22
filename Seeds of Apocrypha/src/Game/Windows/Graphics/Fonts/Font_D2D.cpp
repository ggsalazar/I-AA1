#include <fstream>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include "Font_D2D.h"
#include "CustomFontLoader.h"
#include "CustomFontCollectionLoader.h"

Font_D2D::Font_D2D(const string& filepath, IDWriteFactory* f) :
	factory(f) {
	//Read font file (.otf/.ttf)
	ifstream file(filepath + ".ttf", ios::binary | ios::ate);
	if (!file) file.open(filepath + ".otf", ios::binary | ios::ate);
	if (!file) throw runtime_error("Failed to load font file " + filepath);

	size_t size = file.tellg();
	file.seekg(0);
	vector<uint8_t> data(size);
	file.read(reinterpret_cast<char*>(data.data()), size);

	//Extract font name from filepath
	name = filesystem::path(filepath).stem().wstring();

	//Create and register font loader
	font_loader.Attach(new CustomFontLoader());
	HRESULT hr = factory->RegisterFontFileLoader(font_loader.Get());
	if (FAILED(hr)) throw runtime_error("Failed to register font file loader");

	//Add font to memory-based loader
	hr = font_loader->AddFont(name, data.data(), static_cast<uint32_t>(size));
	if (FAILED(hr)) throw runtime_error("Failed to add font to CustomFontLoader");

	//Create font collection from that font
	collection_loader.Attach(new CustomFontCollectionLoader(font_loader.Get()));
	hr = factory->RegisterFontCollectionLoader(collection_loader.Get());
	if (FAILED(hr)) throw runtime_error("Failed to register CustomFontColelctionLoader");
		
	hr = factory->CreateCustomFontCollection(collection_loader.Get(), name.c_str(), static_cast<UINT32>(name.size() * sizeof(wchar_t)), &font_collection);
	if (FAILED(hr)) throw runtime_error("Failed to create custom font collection");
}

IDWriteTextFormat* Font_D2D::GetFormat(const uint size) {
	auto it = formats.find(size);
	if (it != formats.end()) return it->second.Get();

	ComPtr<IDWriteTextFormat> format;

	//Create text format if it doesn't already exist
	HRESULT hr = factory->CreateTextFormat(
		name.c_str(),
		font_collection.Get(),
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		size, L"", &format);
	if (FAILED(hr)) throw runtime_error("Failed to create text format");

	formats[size] = format;
	return format.Get();
}