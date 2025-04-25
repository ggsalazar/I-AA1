#pragma once
#include <d2d1.h>
#include <iostream>
#include <wincodec.h>  // For WIC (Windows Imaging Component)
#include <d2d1helper.h>  // Helper for Direct2D
#include <wrl/client.h>
#include "../../../Engine/Graphics/Sprite.h"

using Microsoft::WRL::ComPtr;
using namespace std;

class Sprite_D2D : public Engine::Sprite {
    friend class Renderer_D2D;
public:

    //Don't forget to add Entity* when finished implementing!
    //Sprite_D2D(unique_ptr<Spritesheet>& s, Entity* e, const Info& i = {}) :
    Sprite_D2D(const string& f, ID2D1DeviceContext* d_c, const Info& i = {}) :
        Sprite(i), device_context(d_c) {
        if (!LoadSheetFromFile(f))
            throw runtime_error("Could not open Spritesheet " + f + "!");
    }

    bool LoadSheetFromFile(const string& file) override {

        ComPtr<IWICImagingFactory> wic_factory;
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wic_factory)); // Create WIC factory
        if (FAILED(hr)) {
            cout << "Failed to create WIC Factory!" << endl;
            return false;
        }

        //Load the image file into a WIC Bitmap
        ComPtr<IWICBitmapDecoder> decoder;
        wstring file_w = wstring(file.begin(), file.end()) + L".png";
        LPCWSTR file_name = file_w.c_str();
        hr = wic_factory->CreateDecoderFromFilename(file_name, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
        if (FAILED(hr)) {
            cout << "Failed to load image into WIC Bitmap!" << endl;
            return false;
        }

        //Get the first frame of the image
        ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) {
            cout << "Failed to load image!" << endl;
            return false;
        }

        //Update the size of the texture
        hr = frame->GetSize(&info.sheet_size.x, &info.sheet_size.y);
        if (FAILED(hr)) {
            cout << "Failed to get image size!" << endl;
            return false;
        }

        //Convert IWICBitmapFrameDecode to IWICBitmapSource
        ComPtr<IWICFormatConverter> converter;
        hr = wic_factory->CreateFormatConverter(&converter);
        if (FAILED(hr)) {
            cout << "Failed to create format converter!" << endl;
            return false;
        }

        hr = converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.f, WICBitmapPaletteTypeMedianCut);
        if (FAILED(hr)) {
            cout << "Failed to initialize converter!" << endl;
            return false;
        }

        //Convert the Bitmap Source into a Direct2D Bitmap
        hr = device_context->CreateBitmapFromWicBitmap(converter.Get(), nullptr, &bitmap);
        if (FAILED(hr)) {
            cout << "Failed to convert sheet into D2D Bitmap!" << endl;
            return false;
        }

        return true;
    }

    inline ID2D1Bitmap* GetBitmap() const { return bitmap; }
private:
    ID2D1DeviceContext* device_context;
    ID2D1Bitmap* bitmap = nullptr; //Was formerly using ComPtr, stopped because it was being fucky

};