#include <windows.h>
#include <string>
#include <sstream>
#include <ctime>

#define ID_EDIT       1
#define ID_BUTTON     2
#define ID_FEEDBACK   3
#define ID_RESTART    4
#define ID_CLOSE      5

const wchar_t g_szClassName[] = L"GuessingGameGUI";
int randomNumber = 0;
int attempts = 0;
HFONT hFont;

std::wstring CheckGuess(const std::wstring& inputText) {
    try {
        int guess = std::stoi(inputText);
        attempts++;

        if (guess > randomNumber)
            return L"Too high! Try again.";
        else if (guess < randomNumber)
            return L"Too low! Try again.";
        else {
            std::wstringstream msg;
            msg << L"Correct! You guessed it in " << attempts << L" attempts.";
            return msg.str();
        }
    }
    catch (...) {
        return L"Invalid input. Please enter a number.";
    }
}

void GenerateRandomNumber() {
    srand((unsigned int)time(0));
    randomNumber = rand() % 100 + 1;
    attempts = 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hEdit, hButton, hFeedback, hRestartBtn, hCloseBtn;

    switch (msg) {
    case WM_CREATE:
        GenerateRandomNumber();

        hEdit = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
            30, 30, 150, 25, hwnd, (HMENU)ID_EDIT, NULL, NULL);

        hButton = CreateWindowW(L"BUTTON", L"Guess", WS_CHILD | WS_VISIBLE,
            200, 30, 80, 25, hwnd, (HMENU)ID_BUTTON, NULL, NULL);

        hFeedback = CreateWindowW(L"STATIC", L"Enter a number between 1-100", WS_CHILD | WS_VISIBLE,
            30, 70, 300, 25, hwnd, (HMENU)ID_FEEDBACK, NULL, NULL);

        hRestartBtn = CreateWindowW(L"BUTTON", L"Restart", WS_CHILD | WS_VISIBLE,
            30, 110, 100, 25, hwnd, (HMENU)ID_RESTART, NULL, NULL);

        hCloseBtn = CreateWindowW(L"BUTTON", L"Close", WS_CHILD | WS_VISIBLE,
            140, 110, 100, 25, hwnd, (HMENU)ID_CLOSE, NULL, NULL);

        // Set font
        hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

        SendMessage(hEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hButton, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hFeedback, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hRestartBtn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hCloseBtn, WM_SETFONT, (WPARAM)hFont, TRUE);

        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BUTTON: {
            wchar_t inputText[100];
            GetWindowTextW(hEdit, inputText, 100);
            std::wstring result = CheckGuess(inputText);
            SetWindowTextW(hFeedback, result.c_str());
            break;
        }
        case ID_RESTART:
            GenerateRandomNumber();
            SetWindowTextW(hEdit, L"");
            SetWindowTextW(hFeedback, L"Game restarted! Enter a number between 1-100.");
            break;
        case ID_CLOSE:
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORBTN:
    case WM_CTLCOLOREDIT: {
        HDC hdcStatic = (HDC)wParam;
        SetBkColor(hdcStatic, RGB(255, 255, 255));
        SetTextColor(hdcStatic, RGB(0, 0, 0));
        return (INT_PTR)CreateSolidBrush(RGB(240, 240, 240));
    }

    case WM_DESTROY:
        DeleteObject(hFont);
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = g_szClassName;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(g_szClassName, L"Guessing Game",
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 380, 220,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
