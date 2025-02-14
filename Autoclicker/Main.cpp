#include <Windows.h>
#include <iostream>
#include <thread>
#include <atomic>

std::atomic<bool> isAutoClicking(false);
HWND hwndStartStopButton;

void AutoClick() {
    while (isAutoClicking) {
        // Get the current cursor position
        POINT p;
        if (GetCursorPos(&p)) {
            // Simulate a left mouse button down and up click
            mouse_event(MOUSEEVENTF_LEFTDOWN, static_cast<DWORD>(p.x), static_cast<DWORD>(p.y), 0, 0);
            mouse_event(MOUSEEVENTF_LEFTUP, static_cast<DWORD>(p.x), static_cast<DWORD>(p.y), 0, 0);
        }

        // Wait for 1 millisecond before the next click
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    UINT dataSize;
    RAWINPUT raw;

    switch (uMsg) {
    case WM_COMMAND:
        if ((HWND)lParam == hwndStartStopButton) {
            // This button is just for visual indication
        }
        else if ((int)wParam == 2) {
            PostQuitMessage(0);
        }
        break;
    case WM_INPUT:
        // Handle the Mouse4 button click
        dataSize = sizeof(raw);
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, &raw, &dataSize, sizeof(RAWINPUTHEADER));
        if (raw.data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) {
            isAutoClicking = !isAutoClicking;
            SetWindowText(hwndStartStopButton, isAutoClicking ? L"Active" : L"Inactive");
            if (isAutoClicking) {
                std::thread(AutoClick).detach();
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"AutoClickerWindow";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"AutoClicker",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    hwndStartStopButton = CreateWindow(
        L"BUTTON",
        L"Inactive",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        50,
        50,
        100,
        50,
        hwnd,
        NULL,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    CreateWindow(
        L"BUTTON",
        L"Exit",
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        160,
        50,
        100,
        50,
        hwnd,
        (HMENU)2,
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
    );

    ShowWindow(hwnd, nCmdShow);

    // Register raw input device for Mouse4 button
    RAWINPUTDEVICE rid;
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = hwnd;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
