#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "rscs.h"


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/

    RECT winRect;
    SetRect(&winRect, 0, 0, 391, 307); // On met comme taille initiale celle du Bitmap
    AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, false);

    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("PwaytHe"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           winRect.right-winRect.left,  /* The programs width */
           winRect.bottom-winRect.top,  /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP bmp1(LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(baynlEuhbItmHappeuNn)));
    static HBITMAP bmp2(LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(baynlEuhbItmHappedEuu)));
    static bool TouchePressee=false;
    static POINT winSize;
    switch (message) {/* handle the messages */
    // System Messages
    case WM_CREATE:

        break;
    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    case WM_PAINT: {
        //BITMAP bm; Le nom est très mal choisi. C'est juste une structure contenant des infos...
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbmOld; // On pourra s'en passer
        hbmOld = (HBITMAP)SelectObject(hdcMem, TouchePressee?bmp2:bmp1);
        //GetObject(bmp2, sizeof(bm), &bm);

        StretchBlt(hdc, 0, 0, winSize.x, winSize.y, hdcMem, 0, 0, 391, 307, SRCCOPY); // Faut essayer avec SRCINVERT

        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);
        }break;
    case WM_ERASEBKGND:
        return false; // Permet d'éviter le scintillement quand on redimensionne
        break;

    // Input
    case WM_KEYDOWN:
        TouchePressee=true;
        RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE);
        break;
    case WM_KEYUP:
        TouchePressee=false;
        RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE);
        break;
    case WM_SIZE:
        winSize.x=LOWORD(lParam);
        winSize.y=HIWORD(lParam);
        RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE);
        break;
    case WM_SIZING:{
        RECT winRect=*(RECT*)lParam, marginRect;
        SetRect(&marginRect, 0, 0, 0, 0);
        AdjustWindowRect(&marginRect, WS_OVERLAPPEDWINDOW, false);
        winSize.x=winRect.right-winRect.left-(marginRect.right-marginRect.left);
        winSize.y=winRect.bottom-winRect.top-(marginRect.bottom-marginRect.top);
        RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE);
        //MessageBox(hwnd, "Taille : " + winSize.x, "Pouet", MB_ICONHAND);
        }break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
