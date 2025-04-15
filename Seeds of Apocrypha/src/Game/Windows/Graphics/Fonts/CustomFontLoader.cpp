#include "CustomFontLoader.h"

//Stream that wraps the memory font data
class CustomFontStream : public IDWriteFontFileStream {
public:
	CustomFontStream(const void* data, uint32_t size) :
		data_(static_cast<const uint8_t*>(data)), size(size) {
	}

	//IUnknown
	IFACEMETHOD(QueryInterface)(REFIID iid, void** ppv_obj) override {
		if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontFileStream)) {
			*ppv_obj = this;
			++ref_count;
			return S_OK;
		}
		*ppv_obj = nullptr;
		return E_NOINTERFACE;
	}

	IFACEMETHOD_(ULONG, AddRef)() override { return ++ref_count; } //Here because it has to be

	IFACEMETHOD_(ULONG, Release)() override {
		if (--ref_count == 0) delete this;
		return ref_count;
	}

	//IDWriteFontFileStream
	IFACEMETHOD(ReadFileFragment)(
		const void** fragment_start,
		UINT64 file_offset, UINT64 fragment_size,
		void** fragment_context) override {

		if (file_offset + fragment_size > size) return E_FAIL;

		*fragment_start = data_ + static_cast<size_t>(file_offset);
		*fragment_context = nullptr;
		return S_OK;
	}

	IFACEMETHOD_(void, ReleaseFileFragment)(void*) override {}

	IFACEMETHOD(GetFileSize)(UINT64* file_size) override {
		*file_size = size;
		return S_OK;
	}

	IFACEMETHOD(GetLastWriteTime)(UINT64* last_write_time) override {
		*last_write_time = 0;
		return E_NOTIMPL;
	}

private:
	const uint8_t* data_;
	uint32_t size;
	ULONG ref_count = 1;
};


//CustomFontLoader ------------------------------------------------------

HRESULT CustomFontLoader::AddFont(const std::wstring& name, const void* data, uint32_t size) {
	if (!data || size == 0) return E_INVALIDARG;
	if (fonts.find(name) != fonts.end()) return E_FAIL;

	const uint8_t* byte_data = static_cast<const uint8_t*>(data);
	fonts[name] = std::vector<uint8_t>(byte_data, byte_data + size);

	return S_OK;
}

const void* CustomFontLoader::GetFontData(const std::wstring& name, uint32_t& size) const {
	auto it = fonts.find(name);
	if (it == fonts.end()) {
		size = 0;
		return nullptr;
	}

	size = static_cast<uint32_t>(it->second.size());
	return it->second.data();
}

HRESULT STDMETHODCALLTYPE CustomFontLoader::CreateStreamFromKey(
	void const* font_file_ref_key,
	UINT32 font_file_ref_key_size,
	OUT IDWriteFontFileStream** font_file_stream) {

	std::wstring name(reinterpret_cast<const wchar_t*>(font_file_ref_key), font_file_ref_key_size / sizeof(wchar_t));

	uint32_t size;
	const void* data = GetFontData(name, size);
	if (!data) return E_FAIL;

	*font_file_stream = new CustomFontStream(data, size);
	return S_OK;
}

//IUnknown
ULONG CustomFontLoader::AddRef() { return ++ref_count; } //Here because it has to be
ULONG CustomFontLoader::Release() {
	if (--ref_count == 0) delete this;
	return ref_count;
}

HRESULT CustomFontLoader::QueryInterface(REFIID iid, void** ppv_obj) {
	if (iid == __uuidof(IUnknown) || iid == __uuidof(IDWriteFontFileLoader)) {
		*ppv_obj = static_cast<IDWriteFontFileLoader*>(this);
		++ref_count;
		return S_OK;
	}
	*ppv_obj = nullptr;
	return E_NOINTERFACE;
}