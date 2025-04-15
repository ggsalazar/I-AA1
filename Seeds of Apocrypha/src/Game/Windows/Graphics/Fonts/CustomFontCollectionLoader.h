#pragma once
#include <dwrite.h>
#include <wrl.h>
#include <unordered_map>
#include <string>

using Microsoft::WRL::ComPtr;
using std::wstring;

class CustomFontLoader;

class CustomFontCollectionLoader : public IDWriteFontCollectionLoader {
public:
	explicit CustomFontCollectionLoader(CustomFontLoader* loader) : font_loader(loader) {}
	~CustomFontCollectionLoader() = default;

	//IDWriteFontCollectionLoader
	IFACEMETHOD(CreateEnumeratorFromKey)(
		IDWriteFactory* factory,
		void const* collection_key,
		UINT32 collection_key_size,
		OUT IDWriteFontFileEnumerator** font_file_enumerator) override;

	//IUnknown
	IFACEMETHOD(QueryInterface)(REFIID riid, void** ppv_obj) override;
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

private:
	ULONG ref_count = 1;
	CustomFontLoader* font_loader = nullptr;
};