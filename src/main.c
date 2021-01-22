#include <windows.h>

typedef struct Screen {
    int width;
    int height;
    int reslutionX;
    int reslutionY;
} Screen;

Screen screen = {600, 600, 600, 600};

BOOL globalRunning = TRUE;

LRESULT CALLBACK windowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;
    switch (msg) {
        case WM_CLOSE: {
            globalRunning = FALSE;
            PostQuitMessage(0);
        } break;
        default: {
            result = DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }

    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    WNDCLASS windowClass = {0};
    windowClass.lpszClassName = "ReGamesWindowClass";
    windowClass.lpfnWndProc = windowProc;

    RegisterClass(&windowClass);

    HWND window = CreateWindow(windowClass.lpszClassName,
                               "ReGamesWindow",
                               WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               0,
                               0,
                               0,
                               0);

    int bytesPerPixel = 4;
    BITMAPINFO bitmap = {0};
    bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
    bitmap.bmiHeader.biWidth = screen.reslutionX;
    bitmap.bmiHeader.biHeight = screen.reslutionY;
    bitmap.bmiHeader.biPlanes = 1;
    bitmap.bmiHeader.biBitCount = 8 * bytesPerPixel;
    bitmap.bmiHeader.biCompression = BI_RGB;

    void *frameBuffer = VirtualAlloc(0,
                                     screen.reslutionX * screen.reslutionY * bytesPerPixel,
                                     MEM_RESERVE | MEM_COMMIT,
                                     PAGE_READWRITE);

    int *pixel = (int *)frameBuffer;
    for (int y = 0; y < screen.reslutionY; y++) {
        for (int x = 0; x < screen.reslutionX; x++) {
            unsigned char red = (unsigned char)((float)x / (float)screen.reslutionX * 255.0f);
            unsigned char blue = (unsigned char)((float)y / (float)screen.reslutionY * 255.0f);
            *pixel++ = (red << 16) | (blue << 0);
        }
    }

    HDC dc = GetDC(window);

    while (globalRunning) {
        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            DispatchMessage(&msg);
        }

        StretchDIBits(dc,
                      0,
                      0,
                      screen.width,
                      screen.height,
                      0,
                      0,
                      screen.reslutionX,
                      screen.reslutionY,
                      frameBuffer,
                      &bitmap,
                      DIB_RGB_COLORS,
                      SRCCOPY);
    }

    return 0;
}
