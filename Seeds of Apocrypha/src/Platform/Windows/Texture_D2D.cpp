#include <iostream>
#include <wincodec.h>  // For WIC (Windows Imaging Component)
#include <d2d1helper.h>  // Helper for Direct2D
#include <wrl/client.h>
#include "Texture_D2D.h"

bool Engine::Texture_D2D::LoadFromFile(const std::string& file) {
    Microsoft::WRL::ComPtr<IWICImagingFactory> wic_factory;
    HRESULT hr = CoCreateInstance(LSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic_factory)); // Create WIC factory
    if (FAILED(hr)) {
        std::cout << "Failed to create WIC Factory!" << std::endl;
        return false;
    }
    
    //Load the image file into a WIC Bitmap
    IWICBitmapDecoder* decoder;
    hr = wic_factory->CreateDecoderFromFilename(LPCWSTR(file.c_str()), nullptr, IID_PPV_ARGS(&decoder));
    if (FAILED(hr)) {
        std::cout << "Failed to load image into WIC Bitmap!" << std::endl;
        return false;
    }

    IWICBitmapFrameDecode* frame;
    decoder->GetFrame(0, &frame);

    // Convert to 32bpp
    IWICFormatConverter* converter;
    wic_factory->CreateFormatConverter(&converter);
    converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, ...);

    // Create Direct2D Bitmap
    render_target->CreateBitmapFromWicBitmap(converter, nullptr, &bitmap);

    // Release all COM objects when done

    return false;
}
