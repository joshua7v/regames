#include <windows.h>
#include <stdint.h>

#define true 1
#define false 0

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef i32 b32;

typedef struct Screen {
    int width;
    int height;
    int reslutionX;
    int reslutionY;
} Screen;

typedef struct MouseButton {
    b32 isDown;
} MouseButton;

typedef struct GameInput {
    union {
        MouseButton mouseButtons[5];
        struct {
            MouseButton leftButton;
            MouseButton rightButton;
            MouseButton middleButton;
            MouseButton xButton1;
            MouseButton xButton2;
        };
    };
    i32 mouseX;
    i32 mouseY;
} GameInput;

Screen screen = {600, 600, 6, 6};

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

void drawPixel(void *buffer, i32 x, i32 y, i32 color) {
    if (x < 0) { x = 0; }
    if (y < 0) { y = 0; }
    if (x > screen.reslutionX) { x = screen.reslutionX; }
    if (y > screen.reslutionY) { y = screen.reslutionY; }

    i32 *pixel = (i32 *)buffer;
    pixel[y * screen.reslutionX + x] = color;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {

    WNDCLASS windowClass = {0};
    windowClass.lpszClassName = "ReGamesWindowClass";
    windowClass.lpfnWndProc = windowProc;

    RegisterClass(&windowClass);

    RECT windowRect = {0};
    windowRect.right = screen.width;
    windowRect.bottom = screen.height;
    AdjustWindowRect(&windowRect, WS_VISIBLE | WS_OVERLAPPEDWINDOW, false);
    HWND window = CreateWindow(windowClass.lpszClassName,
                               "ReGamesWindow",
                               WS_VISIBLE | WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               windowRect.right - windowRect.left,
                               windowRect.bottom - windowRect.top,
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

    drawPixel(frameBuffer, 0, 0, 0xffff0000);
    drawPixel(frameBuffer, screen.reslutionX - 1, screen.reslutionY - 1, 0xff00ff00);

    HDC dc = GetDC(window);

    GameInput input = {0};
    POINT cursorPositon;
    while (globalRunning) {
        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            DispatchMessage(&msg);
        }

        GetCursorPos(&cursorPositon);
        ScreenToClient(window, &cursorPositon);

        input.leftButton.isDown = GetKeyState(VK_LBUTTON) & (1 << 15);

        input.mouseX = cursorPositon.x;
        input.mouseY = 600 - cursorPositon.y;

        if (input.leftButton.isDown) {
            drawPixel(frameBuffer,
                      (f32)input.mouseX / (f32)screen.width * screen.reslutionX,
                      (f32)input.mouseY / (f32)screen.height * screen.reslutionY,
                      0xffffff00);
        }

        char str[64];
        wsprintf(str, "%d - %d\n", cursorPositon.x, cursorPositon.y);
        OutputDebugString(str);

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
