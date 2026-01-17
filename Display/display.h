#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <windows.h>
#include <d2d1.h>

class DisplayModule {
private:
    ID2D1Factory* factory = nullptr;
    ID2D1HwndRenderTarget* renderTarget = nullptr;
    ID2D1Bitmap* bitmap = nullptr;
    HWND hwnd = NULL;

public:
    DisplayModule(HINSTANCE hInstance, int nCmdShow);
    ~DisplayModule();

    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void CreateWindowModule(HINSTANCE hInstance, int nCmdShow);

    void InitD2D();
    void CleanupD2D();

    void RenderFrame(UINT32* pixels);

};

#endif