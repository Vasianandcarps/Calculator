#include <windows.h>
#include <string>
#include <stdexcept>
#include <bitset>
#include <sstream>
#include <iomanip>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void ToggleBinaryMode(HWND hwnd);
void ToggleDecimalMode(HWND hwnd);
void ToggleHexadecimalMode(HWND hwnd);
void About(HWND hwnd);
std::string CalculateHexExpression(const std::string& expression);

HWND hEdit;
HWND hButtons[22];
HWND hClearAllButton;
bool binaryMode = false;
bool decimalMode = true;
bool hexadecimalMode = false;
#define ID_HELP_ABOUT 101
#define ID_MODE_TOGGLE 102
#define ID_CLEAR_ALL_BUTTON 103
#define ID_BINARY_MODE 104
#define ID_DECIMAL_MODE 105
#define ID_HEXADECIMAL_MODE 106
#define ID_EDIT 107

std::string buffer = "";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const char g_szClassName[] = "myWindowClass";

    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Calculator",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 330,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    HMENU hMenu = CreateMenu();
    HMENU hSubMenu = CreatePopupMenu();
    AppendMenu(hSubMenu, MF_STRING, ID_HELP_ABOUT, "&About");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Help");

    hSubMenu = CreatePopupMenu();

    AppendMenu(hSubMenu, MF_STRING, ID_BINARY_MODE, "&Binary Calculator");
    AppendMenu(hSubMenu, MF_STRING, ID_DECIMAL_MODE, "&Decimal Calculator");
    AppendMenu(hSubMenu, MF_STRING, ID_HEXADECIMAL_MODE, "&Hexadecimal Calculator");
    AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Mode");

    SetMenu(hwnd, hMenu);

    hClearAllButton = CreateWindowEx(
        0, "BUTTON", "Clear All",
        WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
        10, 230, 190, 30,
        hwnd, (HMENU)ID_CLEAR_ALL_BUTTON, GetModuleHandle(NULL), NULL);
    ToggleDecimalMode(hwnd);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

void ToggleBinaryMode(HWND hwnd) {
    binaryMode = true;
    decimalMode = false;
    hexadecimalMode = false;
    SetWindowText(hEdit, "");
    MessageBox(hwnd, "Switched to Binary Calculator Mode", "Mode Change", MB_OK | MB_ICONINFORMATION);
    RECT rc;
    GetWindowRect(hwnd, &rc);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 240, 330, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void ToggleDecimalMode(HWND hwnd) {
    binaryMode = false;
    decimalMode = true;
    hexadecimalMode = false;
    SetWindowText(hEdit, "");
    MessageBox(hwnd, "Switched to Decimal Calculator Mode", "Mode Change", MB_OK | MB_ICONINFORMATION);
    RECT rc;
    GetWindowRect(hwnd, &rc);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 240, 330, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void ToggleHexadecimalMode(HWND hwnd) {
    binaryMode = false;
    decimalMode = false;
    hexadecimalMode = true;
    SetWindowText(hEdit, "");
    MessageBox(hwnd, "Switched to Hexadecimal Calculator Mode", "Mode Change", MB_OK | MB_ICONINFORMATION);
    RECT rc;
    GetWindowRect(hwnd, &rc);
    SetWindowPos(hwnd, NULL, rc.left, rc.top, 240, 430, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void About(HWND hwnd) {
    MessageBox(hwnd, "Calculator", "About", MB_OK | MB_ICONINFORMATION);
}

bool isValidHexadecimal(const std::string& str) {
    std::string hexDigits = "0123456789ABCDEF";
    return str.find_first_not_of(hexDigits) == std::string::npos;
}

std::string CalculateHexExpression(const std::string& expression) {
    std::stringstream ss(expression);
    std::string operand1, operand2, resultStr;
    char op;
    ss >> operand1 >> op >> operand2;

    unsigned int num1, num2, result;

    std::stringstream ss1, ss2;
    ss1 << std::hex << operand1;
    ss1 >> num1;

    ss2 << std::hex << operand2;
    ss2 >> num2;

    switch (op) {
        case '+':
            result = num1 + num2;
            break;
        case '-':
            result = num1 - num2;
            break;
        case '*':
            result = num1 * num2;
            break;
        case '/':
            if (num2 != 0)
                result = num1 / num2;
            else
                throw std::invalid_argument("Division by zero!");
            break;
        default:
            throw std::invalid_argument("Invalid operator");
    }

    std::stringstream ssResult;
    ssResult << std::hex << result;
    ssResult >> resultStr;
    return resultStr;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        hEdit = CreateWindowEx(
            WS_EX_CLIENTEDGE,
            "EDIT",
            "",
            WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY,
            10, 10, 200, 30,
            hwnd, (HMENU)ID_EDIT, GetModuleHandle(NULL), NULL);

        for (int i = 0; i < 22; ++i) {
            int x, y;
            char buttonText[2];
            if (i < 10) {
                x = (i % 4) * 50 + 10;
                y = (i / 4) * 40 + 70;
                buttonText[0] = '0' + i;
            }
            else if (i < 16) {
                x = (i % 4) * 50 + 10;
                y = (i / 4) * 40 + 70;
                switch (i) {
                case 10:
                    buttonText[0] = '+';
                    break;
                case 11:
                    buttonText[0] = '-';
                    break;
                case 12:
                    buttonText[0] = '*';
                    break;
                case 13:
                    buttonText[0] = '/';
                    break;
                case 14:
                    buttonText[0] = '=';
                    break;
                case 15:
                    buttonText[0] = '<';
                    break;
                }
            }
            else {
                x = ((i - 16) % 4) * 50 + 10;
                y = ((i - 16) / 4) * 40 + 270;
                buttonText[0] = 'A' + (i - 16);
            }
            buttonText[1] = '\0';
            hButtons[i] = CreateWindowEx(
                0, "BUTTON", buttonText,
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
                x, y, 40, 30,
                hwnd, (HMENU)(i + 1), GetModuleHandle(NULL), NULL);
        }
        break;

    case WM_CHAR:
        if ((wParam >= '0' && wParam <= '9') ||
            (wParam >= 'A' && wParam <= 'F') ||
            (wParam >= 'a' && wParam <= 'f') ||
            wParam == '.') { // Allow input of the decimal point '.'
            char ch = static_cast<char>(wParam);
            SendMessage(hEdit, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(&ch));
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_CLEAR_ALL_BUTTON:
            if (HIWORD(wParam) == BN_CLICKED) {
                SetWindowText(hEdit, "");
                buffer = "";
            }
            break;
        case ID_MODE_TOGGLE:
            if (HIWORD(wParam) == 0) {
                if (binaryMode)
                    ToggleDecimalMode(hwnd);
                else if (decimalMode)
                    ToggleHexadecimalMode(hwnd);
                else
                    ToggleBinaryMode(hwnd);
            }
            break;

        case ID_BINARY_MODE:
            if (HIWORD(wParam) == 0) {
                ToggleBinaryMode(hwnd);
                SetWindowText(hEdit, buffer.c_str());
            }
            break;

        case ID_DECIMAL_MODE:
            if (HIWORD(wParam) == 0) {
                ToggleDecimalMode(hwnd);
                SetWindowText(hEdit, buffer.c_str());
            }
            break;

        case ID_HEXADECIMAL_MODE:
            if (HIWORD(wParam) == 0) {
                ToggleHexadecimalMode(hwnd);
                SetWindowText(hEdit, buffer.c_str());
            }
            break;

        case ID_HELP_ABOUT:
            About(hwnd);
            break;
        }
        if (HIWORD(wParam) == BN_CLICKED) {
            for (int i = 0; i < 22; ++i) {
                if ((HWND)lParam == hButtons[i]) {
                    char buttonText[2];
                    GetWindowText(hButtons[i], buttonText, 2);
                    std::string text(buttonText);
                    if (text == "=") {
                        buffer = "";
                        char inputBuffer[256];
                        GetWindowText(hEdit, inputBuffer, 256);
                        std::string expression(inputBuffer);
                        try {
                            if (hexadecimalMode) {
                                std::string result = CalculateHexExpression(expression);
                                SetWindowText(hEdit, result.c_str());
                            } else {
                                double result = 0.0; // Initialize the result
                                size_t pos = 0;
                                char op;
                                while ((pos = expression.find_first_of("+-*/", pos)) != std::string::npos) {
                                    op = expression[pos];
                                    std::string operand1 = expression.substr(0, pos);
                                    std::string operand2 = expression.substr(pos + 1);
                                    double num1 = std::stod(operand1); // Convert to double
                                    double num2 = std::stod(operand2); // Convert to double
                                    switch (op) {
                                    case '+':
                                        result = num1 + num2;
                                        break;
                                    case '-':
                                        result = num1 - num2;
                                        break;
                                    case '*':
                                        result = num1 * num2;
                                        break;
                                    case '/':
                                        if (num2 != 0)
                                            result = num1 / num2;
                                        else
                                            throw std::invalid_argument("Division by zero!");
                                        break;
                                    }
                                    expression = std::to_string(result);
                                    pos = 0;
                                }
                                SetWindowText(hEdit, expression.c_str());
                            }
                        }
                        catch (std::invalid_argument& e) {
                            MessageBox(hwnd, e.what(), "Error", MB_OK | MB_ICONERROR);
                        }
                        catch (std::out_of_range& e) {
                            MessageBox(hwnd, "Number is out of range!", "Error", MB_OK | MB_ICONERROR);
                        }
                    }
                    else if (text == "<") {
                        int len = GetWindowTextLength(hEdit);
                        if (len > 0) {
                            SendMessage(hEdit, EM_SETSEL, len - 1, len);
                            SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)"");
                        }
                        if (!buffer.empty()) {
                            buffer.pop_back();
                        }
                    }
                    else {
                        buffer += text;
                        SendMessage(hEdit, EM_REPLACESEL, TRUE, (LPARAM)text.c_str());
                    }
                    break;
                }
            }
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
