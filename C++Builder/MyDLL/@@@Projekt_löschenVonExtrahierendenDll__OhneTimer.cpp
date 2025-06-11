//https://chatgpt.com/c/6843fa2d-3e98-800d-85a0-c2b4be5f2ac6

//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TButton *Button1;
	TLabel *Label1;
	void __fastcall Button1Click(TObject *Sender);
private:	// Benutzer-Deklarationen
    //RRRNeu
	HINSTANCE hDLL;
	String dllPath;
	HINSTANCE ExtractAndLoadMyDLL();

public:		// Benutzer-Deklarationen

	__fastcall TForm5(TComponent* Owner);
    __fastcall ~TForm5();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif


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
	dllPath = ExtractFilePath(Application->ExeName) + "MyExtractOfDLL\\MyTestDLL.dll";
}

//Neu
__fastcall TForm5::~TForm5()
{
    if (hDLL) {
        FreeLibrary(hDLL);
        hDLL = NULL;
    }

    if (FileExists(dllPath)) {
        try {
            DeleteFile(dllPath);
        }
        catch (...) {
            // Falls löschen nicht klappt, kann man hier noch weiter probieren oder ignorieren
        }
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