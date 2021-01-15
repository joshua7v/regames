#include <windows.h>

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

    CreateWindow(windowClass.lpszClassName,
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

    while (globalRunning) {
        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
            DispatchMessage(&msg);
        }
    }

    return 0;
}
