#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <windows.h>  // Einbinden der Windows.h Header-Datei

#pragma comment(lib, "user32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
    Application->Initialize();
    Application->Title = L"Meine Anwendung";

    // Erstellen der MessageBox mit dem Handle des Hauptfensters
    MessageBox(
        Application->MainForm->Handle,   // Handle des Fensters
        L"MeinText",                     // Nachrichtentext
        L"MeineÜberschrift",             // Überschrift der Nachricht
        MB_OK | MB_ICONINFORMATION       // OK-Button und Hinweis-Icon (1)
    );

    return 0;
}


//=========================================================
/*
MB_ICONERROR: Zeigt ein Fehler-Icon (rotes Kreuz).

MB_ICONHAND: Zeigt ebenfalls ein Fehler-Icon (identisch mit MB_ICONERROR).

MB_ICONSTOP: Zeigt ebenfalls ein Fehler-Icon (identisch mit MB_ICONERROR).

MB_ICONQUESTION: Zeigt ein Fragezeichen-Icon.

MB_ICONWARNING: Zeigt ein Warn-Icon (gelbes Dreieck mit Ausrufezeichen).

MB_ICONEXCLAMATION: Zeigt ebenfalls ein Warn-Icon (identisch mit MB_ICONWARNING).

MB_ICONINFORMATION: Zeigt ein Informations-Icon (blaues "i").

MB_ICONASTERISK: Zeigt ebenfalls ein Informations-Icon (identisch mit MB_ICONINFORMATION).
*/    

IDD_MYDIALOG DIALOGEX 0, 0, 186, 95
STYLE DS_SETFONT | DS_MODALFRAME | DS_FIXEDSYS | WS_POPUP | WS_CAPTION | WS_SYSMENU
CAPTION "MeineÜberschrift"
FONT 8, "MS Shell Dlg"
BEGIN
    DEFPUSHBUTTON   "MeinOK",IDOK,129,7,50,14
    ICON            IDI_INFORMATION,IDC_STATIC,7,7,20,20
    LTEXT           "MeinText",IDC_STATIC,7,35,172,8
END

#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include <tchar.h>

#pragma comment(lib, "user32.lib")
#pragma resource "*.res"

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
    Application->Initialize();
    Application->Title = L"Meine Anwendung";

    // Erstellen und Anzeigen des benutzerdefinierten Dialogs
    DialogBox(
        GetModuleHandle(NULL),   // Handle des Moduls
        MAKEINTRESOURCE(IDD_MYDIALOG),  // Ressourcen-ID des Dialogs
        NULL,    // Handle des Elternfensters
        DialogProc  // Dialogprozedur
    );

    return 0;
}
    
