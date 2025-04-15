#include "CustomFontCollectionLoader.h"
#include "CustomFontLoader.h"
#include "CustomFontFileEnumerator.h"

HRESULT CustomFontCollectionLoader::CreateEnumeratorFromKey(IDWriteFactory* factory, void const* collection_key, UINT32 collection_key_size, OUT IDWriteFontFileEnumerator** font_file_enumerator) {

	if (!collection_key || collection_key_size % sizeof(wchar_t) != 0) return E_INVALIDARG;

	//Get the font name from the key
	const wchar_t* name = reinterpret_cast<const wchar_t*>(collection_key);
	std::wstring font_name(name, collection_key_size / sizeof(wchar_t));

	if (font_name.empty()) return E_INVALIDARG;

	*font_file_enumerator = new (std::nothrow) CustomFontFileEnumerator(factory, font_loader, font_name);
	if (!*font_file_enumerator) return E_OUTOFMEMORY;

	return S_OK;
}

HRESULT CustomFontCollectionLoader::QueryInterface(REFIID riid, void** ppv_obj) {

	if (__uuidof(IDWriteFontCollectionLoader) == riid || __uuidof(IUnknown) == riid) {
		*ppv_obj = this;
		++ref_count;
		return S_OK;
	}

	*ppv_obj = nullptr;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CustomFontCollectionLoader::AddRef() { return InterlockedIncrement(&ref_count); }

ULONG STDMETHODCALLTYPE CustomFontCollectionLoader::Release() {
	ULONG new_count = InterlockedDecrement(&ref_count);
	if (new_count == 0) delete this;
	return new_count;
}