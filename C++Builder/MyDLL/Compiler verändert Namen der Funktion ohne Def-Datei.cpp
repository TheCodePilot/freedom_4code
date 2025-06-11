
"
//MyTestDLL.cpp
#pragma hdrstop
#pragma argsused
#include <windows.h>

extern "C" __declspec(dllexport) int Add(int a, int b)
{
    return a + b;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}

//==========================

// MyTestDLL.cpp
#include <windows.h>

extern "C" __declspec(dllexport) int __cdecl Add(int a, int b)
{
    return a + b;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}

//==============================




//Im Code für die Test.exe

//...
typedef int (__cdecl *TAddNumbers)(int, int);
//...
void __fastcall TForm5::Button1Click(TObject *Sender)
{
    HINSTANCE hDLL = ExtractAndLoadMyDLL();
    if (!hDLL)
        return;

    TAddNumbers AddFunc = (TAddNumbers)GetProcAddress(hDLL, "AddNumbers");
    if (!AddFunc) {
        ShowMessage("Funktion nicht gefunden!");
        FreeLibrary(hDLL);
        return;
    }

    int result = AddFunc(42, 8);
    Label1->Caption = "42 + 8 = " + IntToStr(result);

	FreeLibrary(hDLL);
}
"


Er findet die Funktion der DLL nicht. 


//=========================================
//Lösung siehe link: https://chatgpt.com/c/6843fa2d-3e98-800d-85a0-c2b4be5f2ac6
//=========================================

🧩 Sehr wahrscheinlich: Der Exportname ist nicht „Add“, sondern verändert durch den Compiler
Obwohl du extern "C" und __cdecl benutzt hast, kann es trotzdem passieren, dass die Funktion nicht unter dem Namen "Add" exportiert wird.

🔍 Was du jetzt konkret machen solltest:
🔹 1. Starte die Windows-Eingabeaufforderung
🔹 2. Wechsle in das Verzeichnis mit deiner DLL:
cmd
Kopieren
Bearbeiten
cd Pfad\zu\deiner\MyTestDLL.dll
🔹 3. Nutze tdump, um den Exportnamen zu prüfen:
Falls du C++Builder hast, findest du tdump.exe z. B. unter:

java
Kopieren
Bearbeiten
C:\Program Files (x86)\Embarcadero\Studio\22.0\bin\tdump.exe
Dann führe aus:

c
Kopieren
Bearbeiten
tdump -ee MyTestDLL.dll > export.txt
notepad export.txt
🔹 4. Suche im Text nach:
txt
Kopieren
Bearbeiten
Exports
  ordinal:0001=??Add@@YAHHH@Z
oder:

txt
Kopieren
Bearbeiten
  ordinal:0001=_Add
Wenn dort nicht exakt "Add" steht, findest du den realen Namen. Den musst du dann bei GetProcAddress() verwenden.

🛠️ Lösung: Exportnamen explizit festlegen
Damit du dir all das sparst, kannst du den Namen fixieren:

🔧 Option A: .def-Datei verwenden
Erstelle eine Datei MyTestDLL.def mit folgendem Inhalt:

def
Kopieren
Bearbeiten
LIBRARY MyTestDLL
EXPORTS
    Add
Füge sie in dein DLL-Projekt ein (Projekt → „Weitere Dateien“)
→ Nun wird die Funktion garantiert als "Add" exportiert.

🧪 Danach
In deiner EXE bleibt alles gleich:

cpp
Kopieren
Bearbeiten
TAddNumbers AddFunc = (TAddNumbers)GetProcAddress(hDLL, "Add");
Und es funktioniert zuverlässig.

Wenn du möchtest:
Ich sende dir ein vollständig funktionierendes Testprojekt (EXE + DLL + .res) als ZIP, das:

die DLL sauber mit "Add" exportiert,

die .rc korrekt einbettet,

und GetProcAddress() erfolgreich verwendet.

Sag mir bitte nur:

Deine C++Builder-Version

32- oder 64-Bit Ziel

Dann bekommst du ein direkt lauffähiges Paket. Willst du das?








Du:
"cd Pfad\zu\deiner\MyTestDLL.dll " wo sie nun liegt oder das Projektverzeichnis für das erstellen der DLL ?


ChatGPT:
Gute Frage – hier ist die Klarstellung:

👉 Wechsle zu dem Ordner, wo die extrahierte DLL liegt, also die Version, die später von deiner EXE geladen wird.

🔍 Warum?
Du willst mit tdump genau die Version analysieren, die deine EXE auch zur Laufzeit verwendet.

Denn:

Vielleicht hast du die DLL zwar korrekt neu kompiliert,

aber nicht neu in die .res eingebettet,

oder die .res war nicht im Build enthalten,

oder es wurde eine alte Version extrahiert.

✅ Typischer Pfad, wenn du wie in deinem Code extrahierst:
txt
Kopieren
Bearbeiten
...\DeinProjekt\MyExtractOfDLL\MyTestDLL.dll
Dort führst du dann:

cmd
Kopieren
Bearbeiten
tdump -ee MyTestDLL.dll > export.txt
notepad export.txt
💡 Falls du willst, kann ich dir den genauen Exportnamen aus der Datei sagen, wenn du mir export.txt hochlädst oder den Inhalt (den Teil unter „Exports“) hier einfügst. So finden wir den Haken in Sekunden.







Du:
Hier das Ergebnis: "Turbo Dump  Version 6.6.1.0 Copyright (c) 1988-2020 Embarcadero Technologies, Inc.
                   Display of File mytestdll.dll

EXPORT ord:0001='_Add'
EXPORT ord:0002='___CPPdebugHook' " bedeutet dies das ich statt Add _Add angeben muß im Code? Richtig oder ?


ChatGPT:
✅ Ganz genau – du hast es richtig erkannt!
Der tatsächliche Exportname deiner Funktion ist:

text
Kopieren
Bearbeiten
_Add
Das bedeutet, GetProcAddress(hDLL, "Add") kann nicht funktionieren, weil der Exportname einen Unterstrich vorne hat.

🔧 Lösungsmöglichkeiten:
✅ Option 1 (Schnelllösung):
Ändere in deiner EXE:

cpp
Kopieren
Bearbeiten
TAddNumbers AddFunc = (TAddNumbers)GetProcAddress(hDLL, "_Add");
→ Dann wird die Funktion gefunden und korrekt ausgeführt.

✅ Option 2 (Bessere Lösung mit sauberem Namen):
Erstelle im DLL-Projekt eine .def-Datei, damit der Exportname wirklich "Add" ist:

📝 MyTestDLL.def:
def
Kopieren
Bearbeiten
LIBRARY MyTestDLL
EXPORTS
    Add
Dann:

Füge die .def-Datei ins DLL-Projekt ein (als zusätzliche Datei).

Kompiliere die DLL neu.

Erzeuge die .res-Datei neu (brcc32 MyDLL.rc).

Baue dein EXE-Projekt neu.

→ Danach kannst du wieder GetProcAddress(hDLL, "Add") verwenden, ohne Unterstrich.

📌 Warum passiert das überhaupt?
C++Builder hängt bei __cdecl-Funktionen unter Umständen ein _ vor den Exportnamen an, wenn keine .def-Datei verwendet wird. Das ist „klassisches Verhalten“ aus der Windows-API-Welt.

Wenn du möchtest, kann ich dir direkt eine ZIP mit beiden Varianten (mit und ohne .def) bauen, dann kannst du vergleichen.

✅ Aber: Dein Problem ist nun gelöst – einfach _Add verwenden oder .def nutzen. Super, dass du das so sauber nachvollzogen hast!







