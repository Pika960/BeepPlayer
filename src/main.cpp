//includes
#include "main.h"
#include "resource.h"

//prototypes
LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam);
void convertRelativeToAbsolute(char szFilePath[MAX_PATH],
    std::string const &filename);

//global values
bool      endRunningThread;
handle_t  threadHandle;
HWND      hwnd;

//winmain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    MSG        message;
    WNDCLASSEX WndClassEx;

    endRunningThread = false;

    const char szClassName[] = "UIFrame-Class";

    WndClassEx.cbSize        = sizeof(WndClassEx);
    WndClassEx.style         = CS_HREDRAW | CS_VREDRAW;
    WndClassEx.lpfnWndProc   = WndProc;
    WndClassEx.cbClsExtra    = 0;
    WndClassEx.cbWndExtra    = 0;
    WndClassEx.hInstance     = hInstance;
    WndClassEx.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(2002));
    WndClassEx.hIconSm       = LoadIcon(hInstance, MAKEINTRESOURCE(2002));
    WndClassEx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    WndClassEx.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    WndClassEx.lpszMenuName  = NULL;
    WndClassEx.lpszClassName = szClassName;

    if (!RegisterClassEx(&WndClassEx))
    {
        return 0;
    }

    hwnd = CreateWindow(szClassName, "BeepPlayer", WS_CAPTION | WS_SYSMENU |
        WS_MINIMIZEBOX | WS_VISIBLE, 100, 100, 410, 180, NULL, NULL,
        hInstance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    while (GetMessage(&message, NULL, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return static_cast<int>(message.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HMENU hMenu;
    HMENU hSubMenu;

    static HWND hPlayFile;
    static HWND hStatusMsg;

    switch (message)
    {
        case(WM_CREATE):
        {
            hPlayFile = CreateWindowEx(WS_EX_LEFT, "button", "Play File",
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 105, 10, 190, 40,
                hwnd, (HMENU)1,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            hStatusMsg = CreateWindowEx(WS_EX_CLIENTEDGE, "edit",
                "Status:\r\nApplication started", WS_CHILD | WS_VISIBLE |
                ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
                10, 70, 380, 40, hwnd, (HMENU)2,
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            hMenu = CreateMenu();

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING,
                ID_FILE_EXIT, "&Exit");
            AppendMenu(hMenu, MF_STRING | MF_POPUP,
                (UINT64)hSubMenu, "&File");

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING,
                ID_EXAMPLE_MARIO,  "&Super Mario");
            AppendMenu(hSubMenu, MF_STRING,
                ID_EXAMPLE_TETRIS, "&Tetris");
            AppendMenu(hSubMenu, MF_STRING,
                ID_EXAMPLE_ZELDA,  "&Zelda");
            AppendMenu(hMenu, MF_STRING | MF_POPUP,
                (UINT64)hSubMenu, "&Examples");

            hSubMenu = CreatePopupMenu();
            AppendMenu(hSubMenu, MF_STRING,
                ID_HELP_SHORTCUTS, "&Shortcuts");
            AppendMenu(hSubMenu, MF_SEPARATOR,
                0, NULL);
            AppendMenu(hSubMenu, MF_STRING,
                ID_HELP_ABOUT, "&About");
            AppendMenu(hSubMenu, MF_STRING,
                ID_HELP_LICENSE, "&License");
            AppendMenu(hMenu, MF_STRING | MF_POPUP,
                (UINT64)hSubMenu, "&Help");

            SetMenu(hwnd, hMenu);
        } break;

        case(WM_COMMAND):
        {
            switch (LOWORD(wParam))
            {
                case(1):
                {
                    SetDlgItemText(hwnd, 2,
                        "Status:\r\nTrying to load file.");

                    char path[MAX_PATH + 1] = {0};
                    OPENFILENAMEA openfilename = {sizeof(OPENFILENAMEA), 0, 0,
                        "Textfiles (*.txt)\0""*.txt\0\0", 0, 0, 0, path,
                        MAX_PATH, 0, 0, ".\\", "Choose your file...", 0};

                    if (GetOpenFileNameA(&openfilename) == FALSE)
                    {
                        SetDlgItemText(hwnd, 2,
                            "Status:\r\nLoading file failed.");
                    }

                    else
                    {
                        SetDlgItemText(hwnd, 2,
                            "Status:\r\nFile loaded successfull.");
                        loadFile(path);
                    }
                } break;

                case(ID_FILE_EXIT):
                {
                    SetDlgItemText(hwnd, 2,
                        "Status:\r\nApplication will be closed.");
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                } break;

                case(ID_EXAMPLE_MARIO):
                {
                    char szFilePath[MAX_PATH] = {0};
                    convertRelativeToAbsolute(szFilePath,
                        "examples\\mario.txt");

                    loadFile(szFilePath);
                } break;

                case(ID_EXAMPLE_TETRIS):
                {
                    char szFilePath[MAX_PATH] = {0};
                    convertRelativeToAbsolute(szFilePath,
                        "examples\\tetris.txt");

                    loadFile(szFilePath);
                } break;

                case(ID_EXAMPLE_ZELDA):
                {
                    char szFilePath[MAX_PATH] = {0};
                    convertRelativeToAbsolute(szFilePath,
                        "examples\\zelda.txt");

                    loadFile(szFilePath);
                } break;

                case(ID_HELP_SHORTCUTS):
                {
                    MessageBox(NULL, "Exit = ESC", "Shortcuts", MB_OK);
                } break;

                case(ID_HELP_ABOUT):
                {
                    std::string text = "BeepPlayer\r\nVersion 1.0.0.0\r\n";
                    text += "Copyright (c) 2020, Gabriel Daenner";

                    MessageBox(NULL, text.c_str(), "About", MB_OK);
                } break;

                case(ID_HELP_LICENSE):
                {
                    std::string text = "This application is licensed under ";
                    text += "the terms of the GNU GPLv3.\r\n";
                    text += "Visit https://www.gnu.org/licenses/gpl-3.0.txt ";
                    text += "for more informations.";

                    MessageBox(NULL, text.c_str(), "License", MB_OK);
                } break;
            }
        } break;

        case(WM_KEYDOWN):
        {
            switch (wParam)
            {
                case(VK_ESCAPE):
                {
                    SendMessage(hwnd, WM_CLOSE, 0, 0);
                } break;
            }
        } break;

        case(WM_CLOSE):
        {
            if (threadHandle != NULL)
            {
                endRunningThread = true;
            }
        } break;

        case(WM_DESTROY):
        {
            if (threadHandle != NULL)
            {
                WaitForSingleObject(threadHandle, INFINITE);
                CloseHandle(threadHandle);

                threadHandle = NULL;
            }

            PostQuitMessage(0);
            ExitProcess(0);

            return 0;
        } break;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void convertRelativeToAbsolute(char szFilePath[MAX_PATH],
    std::string const &filename)
{
    char szAppPath[MAX_PATH] = {0};
    GetModuleFileName(NULL, szAppPath, sizeof(szAppPath));
    *(strrchr(szAppPath, '\\') + 1) = 0;

    lstrcpy(szFilePath, szAppPath);
    lstrcat(szFilePath, filename.c_str());
}
