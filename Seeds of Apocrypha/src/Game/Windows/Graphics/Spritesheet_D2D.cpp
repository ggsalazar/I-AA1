#include <iostream>
#include <wincodec.h>  // For WIC (Windows Imaging Component)
#include <d2d1helper.h>  // Helper for Direct2D
#include <wrl/client.h>
#include <string>
#include "Spritesheet_D2D.h"

using Microsoft::WRL::ComPtr;

bool Engine::Spritesheet_D2D::LoadFromFile(const std::string& file) {
    

    ComPtr<IWICImagingFactory> wic_factory;
    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic_factory)); // Create WIC factory
    if (FAILED(hr)) {
        std::cout << "Failed to create WIC Factory!" << std::endl;
        return false;
    }
    
    //Load the image file into a WIC Bitmap
    ComPtr<IWICBitmapDecoder> decoder;
    std::wstring file_w = std::wstring(file.begin(), file.end()) + L".png";
    LPCWSTR file_name = file_w.c_str();
    hr = wic_factory->CreateDecoderFromFilename(file_name, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) {
        std::cout << "Failed to load image into WIC Bitmap!" << std::endl;
        return false;
    }
    
    //Get the first frame of the image
    ComPtr<IWICBitmapFrameDecode> frame;
    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) {
        std::cout << "Failed to load first frame of image!" << std::endl;
        return false;
    }
    
    //Update the size of the texture
    hr = frame->GetSize(&size.x, &size.y);
    if (FAILED(hr)) {
        std::cout << "Failed to get image size!" << std::endl;
        return false;
    }
    
    //Convert IWICBitmapFrameDecode to IWICBitmapSource
    ComPtr<IWICFormatConverter> converter;
    hr = wic_factory->CreateFormatConverter(&converter);
    if (FAILED(hr)) {
        std::cout << "Failed to create format converter!" << std::endl;
        return false;
    }
    
    hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
    if (FAILED(hr)) {
        std::cout << "Failed to initialize converter!" << std::endl;
        return false;
    }
    
    //Convert the Bitmap Source into a Direct2D Bitmap
    hr = render_target->CreateBitmapFromWicBitmap(converter.Get(), nullptr, &bitmap);
    if (FAILED(hr)) {
        std::cout << "Failed to convert into D2D Bitmap!" << std::endl;
        return false;
    }

    return true;
}
