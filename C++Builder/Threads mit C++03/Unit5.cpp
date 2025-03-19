//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------

/*
HANDLE ghMutex; // Globaler Mutex-Handle
UINT WM_USER_UPDATE_MEMO = WM_USER + 1; // Benutzerdefinierte Nachricht

// Funktion zur Aktualisierung der GUI (PostMessage wird verwendet)
void PostUpdateMemo(HWND hwndMemo, const std::string& text) {
    // Text wird als LPARAM übergeben (muss in Heap gespeichert werden, um gültig zu bleiben)
    char* buffer = new char[text.size() + 1];
    strcpy(buffer, text.c_str());
    PostMessage(hwndMemo, WM_USER_UPDATE_MEMO, 0, (LPARAM)buffer);
}

// Thread-Funktion
DWORD WINAPI ThreadFunc1(LPVOID param) {
    HWND hwndMemo = (HWND)param;

    // Mutex sperren
    WaitForSingleObject(ghMutex, INFINITE);

    // GUI asynchron aktualisieren
    PostUpdateMemo(hwndMemo, "WinApi: Synchronize: Sofortige Aktualisierung");

    // Mutex freigeben
    ReleaseMutex(ghMutex);
    return 0;
}


__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
}

// Fensterprozedur-Erweiterung, um die Benutzerdefinierte Nachricht zu verarbeiten
void __fastcall TForm5::WndProc(TMessage &Message) {
	if (Message.Msg == WM_USER_UPDATE_MEMO) {
		char* text = reinterpret_cast<char*>(Message.LParam);
		Memo1->Lines->Add(text); // Memo-Feld aktualisieren
		delete[] text; // Speicher freigeben
	}
	TForm::WndProc(Message); // Standard-WndProc aufrufen
}

//---------------------------------------------------------------------------
void __fastcall TForm5::OnClick(TObject *Sender)
{
    // Handle des Memo-Feldes abrufen
	HWND hwndMemo = Form5->Memo1->Handle;

	// Mutex erstellen
	ghMutex = CreateMutex(NULL, FALSE, NULL);
	if (ghMutex == NULL) {
		ShowMessage("Mutex konnte nicht erstellt werden!");
		return;
	}

	// Thread erstellen
	HANDLE thread = CreateThread(NULL, 0, ThreadFunc1, (LPVOID)hwndMemo, 0, NULL);
	if (thread == NULL) {
		ShowMessage("Thread konnte nicht erstellt werden!");
		CloseHandle(ghMutex);
		return;
	}

	// Thread- und Mutex-Handles schließen
	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
	CloseHandle(ghMutex);
}
//---------------------------------------------------------------------------
*/

//NEUER

HANDLE ghMutex; // Globaler Mutex-Handle
UINT WM_USER_UPDATE_MEMO = WM_USER + 1; // Benutzerdefinierte Nachricht

// Funktion zur Aktualisierung der GUI (PostMessage wird verwendet)
void PostUpdateMemo(HWND hwndForm, const std::string& text) {
    // Text wird als LPARAM übergeben (muss in Heap gespeichert werden, um gültig zu bleiben)
    char* buffer = new char[text.size() + 1];
    strcpy(buffer, text.c_str());
    PostMessage(hwndForm, WM_USER_UPDATE_MEMO, 0, (LPARAM)buffer);
}

// Thread-Funktion
DWORD WINAPI ThreadFunc1(LPVOID param) {
    HWND hwndForm = (HWND)param;

    // Mutex sperren
    WaitForSingleObject(ghMutex, INFINITE);

    // GUI asynchron aktualisieren
    PostUpdateMemo(hwndForm, "WinApi: Synchronize: Sofortige Aktualisierung");

    // Mutex freigeben
    ReleaseMutex(ghMutex);
    return 0;
}

__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
    ;
}

// Fensterprozedur-Erweiterung, um die Benutzerdefinierte Nachricht zu verarbeiten
void __fastcall TForm5::WndProc(TMessage &Message) {
    if (Message.Msg == WM_USER_UPDATE_MEMO) {
        char* text = reinterpret_cast<char*>(Message.LParam);
        Memo1->Lines->Add(text); // Memo-Feld aktualisieren
        delete[] text; // Speicher freigeben
    }
    TForm::WndProc(Message); // Standard-WndProc aufrufen
}

// OnClick-Ereignis-Handler
void __fastcall TForm5::OnClick(TObject *Sender) {
    // Handle der Form abrufen
    HWND hwndForm = this->Handle; // Handle des Formulars, nicht des Memos!

    // Mutex erstellen
    ghMutex = CreateMutex(NULL, FALSE, NULL);
    if (ghMutex == NULL) {
        ShowMessage("Mutex konnte nicht erstellt werden!");
        return;
    }

    // Thread erstellen
    HANDLE thread = CreateThread(NULL, 0, ThreadFunc1, (LPVOID)hwndForm, 0, NULL);
    if (thread == NULL) {
        ShowMessage("Thread konnte nicht erstellt werden!");
        CloseHandle(ghMutex);
        return;
    }

    // Thread- und Mutex-Handles schließen
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
    CloseHandle(ghMutex);
}



