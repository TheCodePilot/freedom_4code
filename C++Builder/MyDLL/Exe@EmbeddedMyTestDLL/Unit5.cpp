//---------------------------------------------------------------------------

#include <vcl.h>

#include <windows.h>
#include <fstream>
#pragma hdrstop
#include <SysUtils.hpp>  // Für FileExists und DeleteFile

#include "Unit5.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma resource "MyDLL.res"

TForm5 *Form5;

typedef int (__cdecl *TAddNumbers)(int, int);

//Neu
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner), hDLL(NULL)
{
	folderPath = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\";
	dllPath = folderPath + "MyTestDLL.dll";

    cleanupTimer = new TTimer(this);
	cleanupTimer->Interval = 1000; // alle 1 Sekunde
	cleanupTimer->Enabled = false;
	cleanupTimer->OnTimer = TryDeleteDLLAndFolder;
}

//Neu
__fastcall TForm5::~TForm5()
{
}

void __fastcall TForm5::TryDeleteDLLAndFolder(TObject *Sender)
{
    // Falls Datei noch existiert: wiederholen
    if (FileExists(dllPath)) {
        try {
			DeleteFile(dllPath); // Versuch, löschen
        }
        catch (...) {
            // Ignorieren, nächste Runde
        }
//return; // Abbrechen, Timer bleibt an
    }

    // Datei ist nicht mehr da → Ordner prüfen
    if (DirectoryExists(folderPath)) {
        // Prüfen ob leer
        TSearchRec sr;
        bool isEmpty = true;

        if (FindFirst(folderPath + "*.*", faAnyFile, sr) == 0) {
            do {
                String name = sr.Name;
                if (name != "." && name != "..") {
					isEmpty = false;
					DeleteFile(dllPath);

                    break;
                }
            } while (FindNext(sr) == 0);
            FindClose(sr);
        }

        // Wenn leer, Ordner löschen
        if (isEmpty) {
            try {
                RemoveDir(folderPath);
                cleanupTimer->Enabled = false;
            }
            catch (...) {
                // ggf. noch gesperrt, nächster Versuch später
            }
        }
    } else {
        cleanupTimer->Enabled = false; // nichts mehr zu tun
    }
}


HINSTANCE TForm5::ExtractAndLoadMyDLL()
{
	HRSRC hRes = FindResource(NULL, L"MYDLL", RT_RCDATA);
	if (!hRes) {
		ShowMessage("Eingebettete DLL nicht gefunden");
		return NULL;
	}

	HGLOBAL hGlob = LoadResource(NULL, hRes);
	DWORD size = SizeofResource(NULL, hRes);
	void* pData = LockResource(hGlob);

	String folder = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\";
	if (!DirectoryExists(folder))
		ForceDirectories(folder);
	// DLL löschen, falls vorhanden (vorher, falls nötig)

	if (FileExists(dllPath)) {
		try {
			DeleteFile(dllPath);
		}
		catch (...) {
			// Ignorieren, evtl. beim Beenden nochmal versuchen
		}
	}

	// DLL schreiben
	std::ofstream fout(dllPath.c_str(), std::ios::binary);
	fout.write((char*)pData, size);
	fout.close();

	hDLL = LoadLibrary(dllPath.c_str());
	return hDLL;
}

void __fastcall TForm5::Button1Click(TObject *Sender)
{
	if (!hDLL)
		ExtractAndLoadMyDLL();

	if (!hDLL) {
		ShowMessage("DLL konnte nicht geladen werden!");
		return;
	}

	typedef int (__cdecl *TAddNumbers)(int, int);
	TAddNumbers AddFunc = (TAddNumbers)GetProcAddress(hDLL, "_Add");
	if (!AddFunc) {
		ShowMessage("Funktion nicht gefunden!");
		return;
	}

	int result = AddFunc(42, 8);
	Label1->Caption = "42 + 8 = " + IntToStr(result);

    // DLL nicht hier freigeben, da wir sie noch zum Ende brauchen
}

void __fastcall TForm5::FormClose(TObject *Sender, TCloseAction &Action)
{
    if (hDLL) {
        FreeLibrary(hDLL);
        hDLL = NULL;
    }

//cleanupDLLTimer->Enabled = true; // Phase 1 starten
	//cleanupTimer->Enabled = true;
	/*bool success = true;

	if (FileExists(dllPath)) {
		try {
			success = DeleteFile(dllPath);
		}
		catch (...) {
			success = false;
		}
	}

	Sleep(500);   //RRRMorgen

	// Wenn DLL gelöscht wurde, versuche Ordner zu löschen
	if (success && DirectoryExists(folderPath)) {
		try {
			success = RemoveDir(folderPath);
		}
		catch (...) {
			success = false;
		}
	}
	*/
}

//========================================
/*
HINSTANCE ExtractAndLoadMyDLL_Juenger()
{
	HRSRC hRes = FindResource(NULL, L"MYDLL", RT_RCDATA);
	if (!hRes) {
		ShowMessage("Eingebettete DLL nicht gefunden");
		return NULL;
	}

	HGLOBAL hGlob = LoadResource(NULL, hRes);
	DWORD size = SizeofResource(NULL, hRes);
	void* pData = LockResource(hGlob);

	String folder = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\";
	if (!DirectoryExists(folder))
		ForceDirectories(folder);

	String dllPath = folder + "MyTestDLL.dll";

	// DLL löschen, falls vorhanden
	if (!DeleteExistingDLL(dllPath))
		return NULL; // Abbruch, wenn Löschen nicht möglich

	// DLL schreiben
	std::ofstream fout(dllPath.c_str(), std::ios::binary);
	fout.write((char*)pData, size);
	fout.close();

	// DLL laden
	return LoadLibrary(dllPath.c_str());
}


HINSTANCE ExtractAndLoadMyDLL_Alt()
{
	HRSRC hRes = FindResource(NULL, L"MYDLL", RT_RCDATA);
	if (!hRes) {
		ShowMessage("Eingebettete DLL nicht gefunden");
		return NULL;
	}

	HGLOBAL hGlob = LoadResource(NULL, hRes);
	DWORD size = SizeofResource(NULL, hRes);
	void* pData = LockResource(hGlob);

	// Extrahieren in Unterordner zum Testen
	String folder = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\";
	if (!DirectoryExists(folder))
		ForceDirectories(folder);

	String dllPath = folder + "MyTestDLL.dll";

	// Schreiben
	std::ofstream fout(dllPath.c_str(), std::ios::binary);
	fout.write((char*)pData, size);
	fout.close();

	return LoadLibrary(dllPath.c_str());
}
//==============

*/



//---------------------------------------------------------------------------
/*__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TForm5::Button1Click(TObject *Sender)
{
    HINSTANCE hDLL = ExtractAndLoadMyDLL();
    if (!hDLL)
        return;

    TAddNumbers AddFunc = (TAddNumbers)GetProcAddress(hDLL, "_Add"); //Numbers");
    if (!AddFunc) {
        ShowMessage("Funktion nicht gefunden!");
        FreeLibrary(hDLL);
        return;
    }

    int result = AddFunc(42, 8);
    Label1->Caption = "42 + 8 = " + IntToStr(result);

	FreeLibrary(hDLL);
}
//---------------------------------------------------------------------------
*/

/*//---------------------------------------------------------------------------

#include <vcl.h>

#include <windows.h>
#include <fstream>
#pragma hdrstop
#include <SysUtils.hpp>  // Für FileExists und DeleteFile

#include "Unit5.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma resource "MyDLL.res"

TForm5 *Form5;

typedef int (__cdecl *TAddNumbers)(int, int);


bool DeleteExistingDLL(const String& dllPath)
{
    if (FileExists(dllPath)) {
        try {
			if (!DeleteFile(dllPath)) {
				ShowMessage("Fehler: DLL konnte nicht gelöscht werden (evtl. noch in Benutzung).");
				return false;
			}
		}
		catch (...) {
			ShowMessage("Ausnahme beim Löschen der DLL.");
			return false;
		}
	}
	return true;
}

HINSTANCE ExtractAndLoadMyDLL_Juenger()
{
	HRSRC hRes = FindResource(NULL, L"MYDLL", RT_RCDATA);
	if (!hRes) {
		ShowMessage("Eingebettete DLL nicht gefunden");
		return NULL;
	}

	HGLOBAL hGlob = LoadResource(NULL, hRes);
	DWORD size = SizeofResource(NULL, hRes);
	void* pData = LockResource(hGlob);

	String folder = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\";
	if (!DirectoryExists(folder))
		ForceDirectories(folder);

	String dllPath = folder + "MyTestDLL.dll";

	// DLL löschen, falls vorhanden
	if (!DeleteExistingDLL(dllPath))
		return NULL; // Abbruch, wenn Löschen nicht möglich

	// DLL schreiben
	std::ofstream fout(dllPath.c_str(), std::ios::binary);
	fout.write((char*)pData, size);
	fout.close();

	// DLL laden
	return LoadLibrary(dllPath.c_str());
}


HINSTANCE ExtractAndLoadMyDLL_Alt()
{
	HRSRC hRes = FindResource(NULL, L"MYDLL", RT_RCDATA);
	if (!hRes) {
		ShowMessage("Eingebettete DLL nicht gefunden");
		return NULL;
	}

	HGLOBAL hGlob = LoadResource(NULL, hRes);
	DWORD size = SizeofResource(NULL, hRes);
	void* pData = LockResource(hGlob);

	// Extrahieren in Unterordner zum Testen
	String folder = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\";
	if (!DirectoryExists(folder))
		ForceDirectories(folder);

	String dllPath = folder + "MyTestDLL.dll";

	// Schreiben
	std::ofstream fout(dllPath.c_str(), std::ios::binary);
	fout.write((char*)pData, size);
	fout.close();

	return LoadLibrary(dllPath.c_str());
}
//==============


//Neu
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner), hDLL(NULL)
{
	folderPath = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\";
	dllPath = folderPath + "MyTestDLL.dll";
//dllPath = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\MyTestDLL.dll";

	// Timer vorbereiten, aber noch nicht aktivieren
	cleanupTimer = new TTimer(this);
	cleanupTimer->Interval = 1000; // jede Sekunde versuchen
	cleanupTimer->Enabled = false;
	cleanupTimer->OnTimer = TryDeleteDLLAndFolder;

}

//Neu
__fastcall TForm5::~TForm5()
{


	//while(cleanupTimer->Enabled)
	//{;}

	if(cleanupTimer)
	{
		delete cleanupTimer;
		cleanupTimer = NULL;
    }
	//cleanupTimer->Enabled = true; // Jetzt regelmäßig löschen versuchen
}

void __fastcall TForm5::TryDeleteDLLAndFolder(TObject *Sender)
{
	bool success = true;

	if (FileExists(dllPath)) {
		try {
			success = DeleteFile(dllPath);
		}
		catch (...) {
			success = false;
		}
	}

	// Wenn DLL gelöscht wurde, versuche Ordner zu löschen
	if (success && DirectoryExists(folderPath)) {
		try {
			success = RemoveDir(folderPath);
		}
		catch (...) {
			success = false;
		}
	}

    // Wenn alles erfolgreich, Timer abschalten
    if (success)
		cleanupTimer->Enabled = false;


}



HINSTANCE TForm5::ExtractAndLoadMyDLL()
{
	HRSRC hRes = FindResource(NULL, L"MYDLL", RT_RCDATA);
	if (!hRes) {
		ShowMessage("Eingebettete DLL nicht gefunden");
		return NULL;
	}

	HGLOBAL hGlob = LoadResource(NULL, hRes);
	DWORD size = SizeofResource(NULL, hRes);
	void* pData = LockResource(hGlob);

	String folder = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\";
	if (!DirectoryExists(folder))
		ForceDirectories(folder);
			// DLL löschen, falls vorhanden (vorher, falls nötig)
	if (FileExists(dllPath)) {
		try {
			DeleteFile(dllPath);
		}
		catch (...) {
			// Ignorieren, evtl. beim Beenden nochmal versuchen
		}
	}

	// DLL schreiben
	std::ofstream fout(dllPath.c_str(), std::ios::binary);
	fout.write((char*)pData, size);
	fout.close();

	hDLL = LoadLibrary(dllPath.c_str());
	return hDLL;
}

void __fastcall TForm5::Button1Click(TObject *Sender)
{
	if (!hDLL)
		ExtractAndLoadMyDLL();

	if (!hDLL) {
		ShowMessage("DLL konnte nicht geladen werden!");
		return;
	}

	typedef int (__cdecl *TAddNumbers)(int, int);
	TAddNumbers AddFunc = (TAddNumbers)GetProcAddress(hDLL, "_Add");
	if (!AddFunc) {
		ShowMessage("Funktion nicht gefunden!");
		return;
	}

	int result = AddFunc(42, 8);
	Label1->Caption = "42 + 8 = " + IntToStr(result);

    // DLL nicht hier freigeben, da wir sie noch zum Ende brauchen
}

void __fastcall TForm5::FormClose(TObject *Sender, TCloseAction &Action)
{
    if (hDLL) {
        FreeLibrary(hDLL);
        hDLL = NULL;
    }

	cleanupTimer->Enabled = true; // Jetzt regelmäßig löschen versuchen
}
//---------------------------------------------------------------------------
*/
void __fastcall TForm5::OnClickTerminate(TObject *Sender)
{
    cleanupTimer->Enabled = true;
}
//---------------------------------------------------------------------------

