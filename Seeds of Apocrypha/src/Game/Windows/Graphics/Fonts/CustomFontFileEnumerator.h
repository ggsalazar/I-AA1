#pragma once
#include <dwrite.h>
#include <wrl.h>
#include <string>

using Microsoft::WRL::ComPtr;
using std::wstring;

class CustomFontLoader;

class CustomFontFileEnumerator : public IDWriteFontFileEnumerator {
public:
	CustomFontFileEnumerator(IDWriteFactory* factory, CustomFontLoader* loader, const wstring& font_name) :
		factory(factory), font_loader(loader), font_name(font_name) {}
	~CustomFontFileEnumerator() = default;

	///IUnknown
	IFACEMETHOD(QueryInterface)(REFIID riid, void** ppv_obj) override;
	ULONG __stdcall AddRef();
	ULONG __stdcall Release();

	//IDWriteFontFileEnumerator
	IFACEMETHOD(MoveNext)(BOOL* has_current_file) override;
	IFACEMETHOD(GetCurrentFontFile)(IDWriteFontFile** font_file) override;

private:
	ULONG ref_count = 1;
	bool has_moved = false;
	ComPtr<IDWriteFactory> factory;
	CustomFontLoader* font_loader;
	wstring font_name;
	ComPtr<IDWriteFontFile> curr_font_file;
};