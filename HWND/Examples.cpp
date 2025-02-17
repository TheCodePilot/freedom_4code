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
