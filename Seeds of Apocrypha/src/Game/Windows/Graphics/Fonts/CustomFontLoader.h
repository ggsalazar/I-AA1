#pragma once
#include <dwrite.h>
#include <wrl.h>
#include <unordered_map>
#include <string>

using std::string;
using std::vector;

class CustomFontLoader : public IDWriteFontFileLoader {
public:
	CustomFontLoader() {}
	virtual ~CustomFontLoader() = default;

	HRESULT AddFont(const std::wstring& name, const void* data, uint32_t size);
	const void* GetFontData(const std::wstring& name, uint32_t& size) const;

	HRESULT STDMETHODCALLTYPE CreateStreamFromKey(
		void const* font_file_ref_key,
		UINT32 font_file_ref_key_size,
		OUT IDWriteFontFileStream** font_file_stream) override;

	//Required IUnknown methods
	ULONG STDMETHODCALLTYPE AddRef() override;
	ULONG STDMETHODCALLTYPE Release() override;
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppv_obj) override;

private:
	ULONG ref_count = 1;
	std::unordered_map<std::wstring, vector<uint8_t>> fonts;
	IDWriteFactory* write_factory = nullptr;
};