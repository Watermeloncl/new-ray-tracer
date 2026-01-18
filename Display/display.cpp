#include <windows.h>
#include <d2d1.h>
#include <iostream>

#include "display.h"
#include "..\config.h"

DisplayModule::DisplayModule(HINSTANCE hInstance, int nCmdShow) {
    this->CreateWindowModule(hInstance, nCmdShow);

    this->InitD2D();
}

DisplayModule::~DisplayModule() {
    this->CleanupD2D();
}

LRESULT CALLBACK DisplayModule::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
            break;
        }        
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void DisplayModule::CreateWindowModule(HINSTANCE hInstance, int nCmdShow) {
    const wchar_t CLASS_NAME[]  = L"Ray Tracer Window Class";

    WNDCLASSW wc = { };

    wc.lpfnWndProc = DisplayModule::WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);

    RECT rect;
    if(SHOW_ON_SCREEN_FLAG) {
        rect = {0, 0, CLIENT_SCREEN_WIDTH, CLIENT_SCREEN_HEIGHT};
    } else {
        rect = {0, 0, MIN_CLIENT_SCREEN_WIDTH, MIN_CLIENT_SCREEN_HEIGHT};
    }

    AdjustWindowRectEx(&rect, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, FALSE, 0);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    this->hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"Ray Tracer",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,

        (screenWidth - (rect.right - rect.left)) / 2, (screenHeight - (rect.bottom - rect.top)) / 2, rect.right - rect.left, rect.bottom - rect.top,

        NULL,   
        NULL,
        hInstance,
        NULL
        );

    if (hwnd == NULL) {
        std::cout << "Failed to create window handle." << std::endl;
        return;
    }

    ShowWindow(hwnd, nCmdShow);
}

void DisplayModule::InitD2D() {
    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);

    RECT rc;
    GetClientRect(hwnd, &rc);

    D2D1_RENDER_TARGET_PROPERTIES props = 
        D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
                                     D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                                                       D2D1_ALPHA_MODE_IGNORE));

    D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps =
        D2D1::HwndRenderTargetProperties(hwnd,
                                         D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top),
                                         D2D1_PRESENT_OPTIONS_NONE); // VSync enabled

    factory->CreateHwndRenderTarget(&props, &hwndProps, &renderTarget);

    // Create initial bitmap
    D2D1_BITMAP_PROPERTIES bmpProps =
        D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
                                                 D2D1_ALPHA_MODE_IGNORE));
    
    if(SHOW_ON_SCREEN_FLAG) {
        renderTarget->CreateBitmap(D2D1::SizeU(CLIENT_SCREEN_WIDTH, CLIENT_SCREEN_HEIGHT), bmpProps, &bitmap);
    } else {
        renderTarget->CreateBitmap(D2D1::SizeU(MIN_CLIENT_SCREEN_WIDTH, MIN_CLIENT_SCREEN_HEIGHT), bmpProps, &bitmap);
    }
}

void DisplayModule::CleanupD2D() {
    if (bitmap) bitmap->Release();
    if (renderTarget) renderTarget->Release();
    if (factory) factory->Release();
}

void DisplayModule::RenderFrame(UINT32* pixels) {
    if(SHOW_ON_SCREEN_FLAG) {
        D2D1_RECT_U rect = {0, 0, (UINT32)CLIENT_SCREEN_WIDTH, (UINT32)CLIENT_SCREEN_HEIGHT};
        this->bitmap->CopyFromMemory(&rect, pixels, CLIENT_SCREEN_WIDTH * 4);
    } else {
        D2D1_RECT_U rect = {0, 0, (UINT32)MIN_CLIENT_SCREEN_WIDTH, (UINT32)MIN_CLIENT_SCREEN_HEIGHT};
        this->bitmap->CopyFromMemory(&rect, pixels, MIN_CLIENT_SCREEN_WIDTH * 4);
    }

    this->renderTarget->BeginDraw();
    this->renderTarget->Clear(D2D1::ColorF(0, 0, 0));
    this->renderTarget->DrawBitmap(this->bitmap);
    this->renderTarget->EndDraw(); // BLOCKS for VSync
}

