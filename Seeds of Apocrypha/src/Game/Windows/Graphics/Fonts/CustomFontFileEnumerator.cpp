#include "CustomFontFileEnumerator.h"
#include "CustomFontLoader.h"

HRESULT CustomFontFileEnumerator::QueryInterface(REFIID riid, void** ppv_obj) {
    if (__uuidof(IDWriteFontFileEnumerator) == riid || __uuidof(IUnknown) == riid) {
        *ppv_obj = this;
        ++ref_count;
        return S_OK;
    }

    *ppv_obj = nullptr;
    return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE CustomFontFileEnumerator::AddRef() { return InterlockedIncrement(&ref_count); }

ULONG STDMETHODCALLTYPE CustomFontFileEnumerator::Release() {
    ULONG new_count = InterlockedDecrement(&ref_count);
    if (new_count == 0) delete this;
    return new_count;
}

HRESULT CustomFontFileEnumerator::MoveNext(BOOL* has_current_file) {
    if (has_moved || font_loader == nullptr) {
        *has_current_file = FALSE;
        return S_OK;
    }

    //Tell DirectWrite how to get the in-memory font file
    HRESULT hr = factory->CreateCustomFontFileReference(
        font_name.c_str(),
        static_cast<UINT32>(font_name.size() * sizeof(wchar_t)),
        font_loader,
        &curr_font_file
    );

    *has_current_file = SUCCEEDED(hr);
    has_moved = true;
    return hr;
}

HRESULT CustomFontFileEnumerator::GetCurrentFontFile(IDWriteFontFile** font_file) {
    if (!curr_font_file) return E_FAIL;

    *font_file = curr_font_file.Get();
    (*font_file)->AddRef();
    return S_OK;
}