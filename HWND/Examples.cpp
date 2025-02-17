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
        MB_OK | MB_ICONINFORMATION       // OK-Button und Hinweis-Icon
    );

    return 0;
}
