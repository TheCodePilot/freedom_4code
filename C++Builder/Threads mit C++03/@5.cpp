//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "MSAccess"
#pragma resource "*.dfm"
TForm1 *Form1;

//======================================
//Normalerweise in eigenen header und cpp
//======================================

class TUpdateMemoThread : public TThread
{
private:
    TForm1 *Form;
protected:
    void __fastcall Execute()
    {
        Synchronize(&UpdateMemo);
    }
    void __fastcall UpdateMemo()
    {
        Form->Memo1->Lines->Add("Thread läuft...\n");
    }
public:
    __fastcall TUpdateMemoThread(TForm1 *AForm) : TThread(false), Form(AForm) {}
};

DWORD WINAPI rrThreadFunc(LPVOID lpParam)
{
    TForm1 *Form = static_cast<TForm1*>(lpParam);
    TUpdateMemoThread *Thread = new TUpdateMemoThread(Form);
    Thread->WaitFor();
    delete Thread;
    return 0;
}

int rrMain_()
{
    HANDLE hThread = CreateThread(
        NULL,         // Sicherheitsattribute
        0,            // Stackgröße (0 = Standard)
        rrThreadFunc,   // Startadresse des Threads
        Form1,         // Parameter für Thread-Funktion
        0,            // Erstellungsflags (0 = sofort starten)
        NULL          // Thread-ID (optional)
    );

    if (hThread == NULL) {
        ShowMessage("Fehler beim Erstellen des Threads.\n");
        return -1;
    }

    // Warte auf die Thread-Beendigung
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    return 0;
}

void __fastcall TForm1::MitWinApiOnClick(TObject *Sender, TPoint &MousePos, bool &Handled)
{
    rrMain_();
}

void __fastcall TForm1::UpdateMemo()
{
    Memo1->Lines->Add("Thread läuft...\n");
}









/*
void ShowLine()
{
	Form1->UpdateMemo();
}

DWORD WINAPI rrThreadFunc(LPVOID lpParam)
{
	// GUI-Aktualisierung im Haupt-Thread
	//TThread::
	TThread::Synchronize(
		&ShowLine
	);
	return 0;
}

int rrMain_()
{
	HANDLE hThread = CreateThread(
		NULL,         // Sicherheitsattribute
		0,            // Stackgröße (0 = Standard)
		rrThreadFunc,   // Startadresse des Threads
		NULL,         // Parameter für Thread-Funktion
		0,            // Erstellungsflags (0 = sofort starten)
		NULL          // Thread-ID (optional)
	);

	if (hThread == NULL) {
		ShowMessage("Fehler beim Erstellen des Threads.\n");
		return -1;
	}

	// Warte auf die Thread-Beendigung
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	return 0;
}


void __fastcall TForm1::MitWinApiOnClick(TObject *Sender, TPoint &MousePos, bool &Handled)
{
	rrMain_();
}

void __fastcall TForm1::UpdateMemo()
{
	Form1->Memo1->Lines->Add("Thread läuft...\n");
}
*/

/*
DWORD WINAPI rrThreadFunc(LPVOID lpParam)
{
	//ShowMessage("Thread läuft...\n");
	//Form1->Memo1->Lines->Add("Thread läuft...\n");


	// GUI-Aktualisierung im Haupt-Thread
	TThread::Synchronize(NULL,  {
		Form1->Memo1->Lines->Add("Thread läuft...\n");
	});

	return 0;
}

int rrMain_()
{
	HANDLE hThread = CreateThread(
		NULL,         // Sicherheitsattribute
		0,            // Stackgröße (0 = Standard)
		rrThreadFunc,   // Startadresse des Threads
		NULL,         // Parameter für Thread-Funktion
		0,            // Erstellungsflags (0 = sofort starten)
		NULL          // Thread-ID (optional)
	);

	if (hThread == NULL) {
		ShowMessage("Fehler beim Erstellen des Threads.\n");
		return -1;
	}

	// Warte auf die Thread-Beendigung
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	return 0;
}
*/

/*
int main() {
	HANDLE hThread = CreateThread(
		NULL,         // Sicherheitsattribute
		0,            // Stackgröße (0 = Standard)
		ThreadFunc,   // Startadresse des Threads
		NULL,         // Parameter für Thread-Funktion
		0,            // Erstellungsflags (0 = sofort starten)
		NULL          // Thread-ID (optional)
	);

	if (hThread == NULL) {
		std::cerr << "Fehler beim Erstellen des Threads.\n";
		return 1;
	}

	// Warte auf die Thread-Beendigung
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	return 0;
}
*/



//======================================
//Normalerweise in eigenen header und cpp
//======================================
class MyThreadOnlySynChronize : public TThread
{
protected:
	//�berschrieben TThread::Execute()
	void __fastcall Execute();
	//�berschrieben
	void __fastcall ShowThreadMessage();
public:
	__fastcall MyThreadOnlySynChronize(bool CreateSuspended) : TThread(CreateSuspended) {}
};



void __fastcall MyThreadOnlySynChronize::Execute()
{
	// Simuliere Arbeit im Hintergrund
	Sleep(1000);

	// Synchronisiere die GUI-Interaktion mit dem Hauptthread
	Synchronize(&ShowThreadMessage);
}

void __fastcall MyThreadOnlySynChronize::ShowThreadMessage()
{
	ShowMessage("Hello from thread!");
}
//======================================


//======================================
//Normalerweise in eigenen header und cpp
//======================================
class MyThreadQVsSync : public TThread {

protected:

	void __fastcall Execute();

	void __fastcall UpdateImmediate();

	void __fastcall UpdateDelayed();

public:
	__fastcall MyThreadQVsSync(bool CreateSuspended) : TThread(CreateSuspended) {}
};


void __fastcall MyThreadQVsSync::Execute()
{
		// Simulierte Hintergrundarbeit
		for (int i = 0; i < 5; i++) {
			Sleep(1000); // 1 Sekunde warten
			// Aktualisierung des GUI sofort
			Synchronize(&UpdateImmediate);

			// Hinzufügen einer asynchronen Aufgabe zur GUI
			Queue(&UpdateDelayed);
		}
}

void __fastcall MyThreadQVsSync::UpdateImmediate()
{
		// GUI-Aktion, die sofort ausgeführt wird
		Form1->Memo1->Lines->Add("Synchronize: Sofortige Aktualisierung");
}


void __fastcall MyThreadQVsSync::UpdateDelayed()
{
		// GUI-Aktion, die asynchron ausgeführt wird
		Form1->Memo1->Lines->Add("Queue: Asynchrone Aktualisierung");
}
//======================================


//======================================
//Normalerweise in eigenen header und cpp
//======================================
class DatabaseThread : public TThread {

private:
	TMSQuery *dbQuery;

protected:

	void __fastcall Execute();
	void __fastcall ProcessResults();

public:
	__fastcall DatabaseThread(TMSQuery *Query, bool CreateSuspended) : TThread(CreateSuspended)
	{
		dbQuery = Query;
	}

};


void __fastcall DatabaseThread::Execute()
{
	try
	{
		//dbQuery = NULL;
		//dbQuery = new TMSQuery(NULL);
		dbQuery->SQL->Text = "SELECT TOP (1) * FROM Nordwind.dbo.Protokol";
		dbQuery->Open();

		// Verarbeite die Ergebnisse
		Synchronize(&ProcessResults);

	}
	catch (const Exception &e)
	{
		ShowMessage("Fehler Beim Aufbau der Datenbank: Nordwind " + e.Message);
	}

	//delete Query; // Ressourcen freigeben
}


void __fastcall DatabaseThread::ProcessResults()
{
	// GUI-Update oder Verarbeitung
	Form1->Memo1->Lines->Add(dbQuery->FieldByName("PK_NR")->AsString);
}


//======================================
//Normalerweise in eigenen header und cpp
//======================================

class DatabaseThreadMutex : public TThread {
private:
	HANDLE Mutex; // Mutex-Handle
	TMSQuery *dbQuery;
protected:
	void __fastcall Execute()  {
		// Kritischer Bereich mit Mutex schützen
		WaitForSingleObject(Mutex, INFINITE);
		try {
//dbQuery = new TMSQuery(NULL);
			dbQuery->SQL->Text = "SELECT TOP (1) * FROM Nordwind.dbo.Protokol";
			dbQuery->Open();

			// Ergebnisse verarbeiten
			Synchronize(&ProcessResults);
		}
		catch (const Exception &e)
		{
			ShowMessage("Fehler Beim Aufbau der Datenbank: Nordwind " + e.Message);
		}
//delete Query;
		ReleaseMutex(Mutex); // Mutex freigeben
	}

	void __fastcall ProcessResults() {
		// GUI-Aktualisierung
		Form1->Memo1->Lines->Add(dbQuery->FieldByName("PK_NR")->AsString);
	}
public:
	__fastcall DatabaseThreadMutex(TMSQuery *Query, HANDLE aMutex, bool CreateSuspended)
		: TThread(CreateSuspended), Mutex(aMutex)
		{
			dbQuery = Query;
		}
};


/*
class DatabaseThreadMutex : public TThread {
private:
THandle Mutex; // Mutex-Handle
TMSQuery* dbQuery;

protected:
void __fastcall Execute() {
// Kritischer Bereich mit Mutex schützen
if (WaitForSingleObject(Mutex, INFINITE) == WAIT_OBJECT_0) {
try {
// dbQuery = new TMSQuery(NULL);
dbQuery->SQL->Text = "SELECT TOP (1) FROM Nordwind.dbo.Protokol";
dbQuery->Open();
// Ergebnisse verarbeiten
Synchronize(&ProcessResults);
} catch (const Exception &e) {
ShowMessage("Fehler Beim Aufbau der Datenbank: Nordwind " + e.Message);
}
ReleaseMutex(Mutex); // Mutex freigeben
}
}

void __fastcall ProcessResults() {
// GUI-Aktualisierung
Form1->Memo1->Lines->Add(dbQuery->FieldByName("PK_NR")->AsString);
}

public:
__fastcall DatabaseThreadMutex(TMSQuery *Query, THandle aMutex, bool CreateSuspended)
: TThread(CreateSuspended), dbQuery(Query), Mutex(aMutex) {}
};
*/











//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	Ini();
}


__fastcall TForm1::~TForm1()
{
	if(MSCon)
		delete MSCon;
	MSCon = NULL;

	HandleQuerys(-1);
}


void TForm1::Ini()
{
	Q4SQL = NULL;
	MSCon = NULL;
	IniDB(0);
	HandleQuerys(1);
	//Nun jede Art von Query möglich
}


//----------------------------------------------------------------------------
void TForm1::CreateExplicitMSCon()
//----------------------------------------------------------------------------
{
	if(MSCon)
		return;
	MSCon = new TMSConnection(NULL);
}


//----------------------------------------------------------------------------
void TForm1::IniDB(int optionWhichDBServer)
//----------------------------------------------------------------------------
{
   /*	if( (optionWhichDBServer == 0) && bLocalHost)
		optionWhichDBServer = 1;
   */
	if(optionWhichDBServer == 0)
	{
		try
		{
			CreateExplicitMSCon();

			MSCon->Server = "detoic008A";
			MSCon->Username = "MDE";
			MSCon->Password = "MDEKrump";
			MSCon->Database = "Nordwind"; //"MDE";

			MSCon->LoginPrompt = !False;
			MSCon->Connect();
		}
		catch (const Exception &e)
		{
			ShowMessage("Fehler Beim Aufbau der Datenbank: " + e.Message);
		}

		return;
	}

	if(optionWhichDBServer == 1)
	{
		try
		{
			CreateExplicitMSCon();

			MSCon->Server = "localhost\\SQLEXPRESS";
			MSCon->Authentication = auWindows;
			MSCon->Database = "MDE";

			MSCon->LoginPrompt = !False;
			MSCon->Connect();
		}
		catch (const Exception &e)
		{
			ShowMessage("Fehler Beim Aufbau der Datenbank: " + e.Message);
		}

		return;
	}

}


//==============================================================================================================================================
bool TForm1::HandleQuerys(int option)
//==============================================================================================================================================
{

	if(option == 1)
	{

		if(!MSCon || !MSCon->Connected)
		{
			ShowMessage("Keine Verbindung zu MDE-SQL-Server. Aktion abgebrochen");
			return false;
		}

		if(!Q4SQL)
			Q4SQL = new TMSQuery(NULL);

		if(Q4SQL)
			Q4SQL->Connection = MSCon;
		else
			return false;

		return true;
	}
	else if(option == -1)
	{
		if (Q4SQL)
		{
			delete Q4SQL;
			Q4SQL = NULL;
		}
		return true;
	}
	return false;
}


void TForm1::TestUmgebung()
{
	// Starten des benutzerdefinierten Threads
	MyThreadOnlySynChronize *thread = new MyThreadOnlySynChronize(false); // Automatischer Start, false = nicht suspendiert bzw. ausgesetzt
	thread->FreeOnTerminate = true;  // Automatischer Speicherfreigabe //Optional aber sch�ner so
	ShowMessage("Hello from main thread");

}



void TForm1::TestUmgebung_ThreadManuellStarten()
{
	// Thread im suspendierten Modus erstellen also noch warten
	MyThreadOnlySynChronize* thread = new MyThreadOnlySynChronize(true);

	// Vor dem Start des Threads initialisieren oder konfigurieren
	ShowMessage("Thread wurde erstellt, aber ist noch nicht gestartet.");

	// Thread manuell starten
	thread->Start();
	ShowMessage("Thread wurde manuell gestartet.");
}


//---------------------------------------------------------------------------
void __fastcall TForm1::QVSyOnClick(TObject *Sender)
{
	MyThreadQVsSync *thread = new MyThreadQVsSync(false);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::ManuellStartenOnClick(TObject *Sender)
{
	TestUmgebung_ThreadManuellStarten();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::QueryTestOnClick(TObject *Sender)
{
	DatabaseThread *dbThread = new DatabaseThread(Q4SQL, false);
	dbThread->Priority = tpHigher; // Priorität setzen
}

/*
//---------------------------------------------------------------------------
void __fastcall TForm1::MitWinApiOnClick(TObject *Sender, TPoint &MousePos, bool &Handled)
{
	rrMain_(); //So funktioniert es nicht !!!
}
//---------------------------------------------------------------------------
*/

void __fastcall TForm1::QueryTestMutexOnClick(TObject *Sender)
{
    // Mutex erstellen
	HANDLE mutexHandle = CreateMutex(NULL, FALSE, NULL);

	// Starte den Datenbank-Thread
	DatabaseThreadMutex *dbThread = new DatabaseThreadMutex(Q4SQL, mutexHandle, false);

//Application->Run();

	// Schließe den Mutex am Ende
	CloseHandle(mutexHandle);
}
//---------------------------------------------------------------------------



/*
Beispiel: TThread mit THandle Mutex
cpp
#include <vcl.h>
#pragma hdrstop
#include <windows.h> // Für Mutex
#include <tchar.h>

class DatabaseThread : public TThread {
private:
	THandle Mutex; // Mutex-Handle
	TMSQuery *Query;
protected:
	void __fastcall Execute() override {
		// Kritischer Bereich mit Mutex schützen
		WaitForSingleObject(Mutex, INFINITE);
		try {
			Query = new TMSQuery(NULL);
			Query->SQL->Text = "SELECT TOP (1) * FROM YourTable";
			Query->Open();

			// Ergebnisse verarbeiten
			Synchronize(&ProcessResults);
		} catch (...) {
			// Fehlerbehandlung
		}
		delete Query;
		ReleaseMutex(Mutex); // Mutex freigeben
	}

    void __fastcall ProcessResults() {
		// GUI-Aktualisierung
        Form1->Memo1->Lines->Add(Query->FieldByName("YourField")->AsString);
    }
public:
    __fastcall DatabaseThread(THandle aMutex, bool CreateSuspended)
        : TThread(CreateSuspended), Mutex(aMutex) {}
};

// Hauptprogramm
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
    try {
		Application->Initialize();
		Application->CreateForm(__classid(TForm1), &Form1);

		// Mutex erstellen
		THandle mutexHandle = CreateMutex(NULL, FALSE, NULL);

		// Starte den Datenbank-Thread
		DatabaseThread *dbThread = new DatabaseThread(mutexHandle, false);

		Application->Run();

		// Schließe den Mutex am Ende
		CloseHandle(mutexHandle);

    } catch (Exception &exception) {
        Application->ShowException(&exception);
	}
    return 0;
}
Erklärung:
Mutex-Definition:

*/


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


/*
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>

// Funktion für den Thread
void databaseThreadFunction() {
	try {
		// Simulierte Datenbankabfrage
		std::cout << "Datenbankabfrage läuft..." << std::endl;
		boost::this_thread::sleep_for(boost::chrono::seconds(2)); // Simulierte Verzögerung
		std::string result = "SELECT TOP (1) * FROM YourTable -> Ergebnis";

		// Ergebnisse verarbeiten (z. B. an die GUI weitergeben)
		std::cout << "Abfrageergebnis: " << result << std::endl;

	} catch (const std::exception& e) {
		std::cerr << "Fehler: " << e.what() << std::endl;
	}
}

int main() {
	try {
		// Starte einen Thread mit Boost
		boost::thread dbThread(databaseThreadFunction);

		// Priorität kann mit der nativen API eingestellt werden, wenn nötig
		// Beispiel: SetThreadPriority(dbThread.native_handle(), THREAD_PRIORITY_HIGHEST);

		// Warte auf die Thread-Beendigung
		dbThread.join();
		std::cout << "Thread abgeschlossen." << std::endl;

	} catch (const std::exception& e) {
		std::cerr << "Fehler im Hauptthread: " << e.what() << std::endl;
	}
	return 0;
}
*/

/*
Umsetzung mit Boost und GUI-Synchronisation
Angenommen, du nutzt Boost für den Hintergrundthread und möchtest, dass die Ergebnisse sicher in der GUI angezeigt werden, könntest du folgendes tun:

cpp
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <iostream>
#include <queue>
#include <mutex>
#include <windows.h> // Für GUI-Integration mit PostMessage()

// Globale Variablen für die Thread-Kommunikation
std::queue<std::string> resultQueue;
std::mutex queueMutex;
HWND mainWindowHandle; // Handle des Hauptfensters für GUI-Aktualisierungen

// Definiere eine benutzerdefinierte Nachricht für die GUI
#define WM_UPDATE_GUI (WM_USER + 1)

// Thread-Funktion
void databaseThreadFunction() {
	try {
		// Simulierte Datenbankabfrage
		boost::this_thread::sleep_for(boost::chrono::seconds(2)); // Simulierte Verzögerung
		std::string result = "SELECT TOP (1) * FROM YourTable -> Ergebnis";

		// Ergebnis in die Warteschlange einfügen
		{
			std::lock_guard<std::mutex> lock(queueMutex);
			resultQueue.push(result);
		}

		// Informiere die GUI über neue Daten
		PostMessage(mainWindowHandle, WM_UPDATE_GUI, 0, 0);

	} catch (const std::exception& e) {
		std::cerr << "Fehler: " << e.what() << std::endl;
	}
}

// Window-Prozedur (GUI)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_UPDATE_GUI:
		// Verarbeitung der neuen Daten
		{
			std::lock_guard<std::mutex> lock(queueMutex);
			while (!resultQueue.empty()) {
				std::string result = resultQueue.front();
				resultQueue.pop();
				std::cout << "GUI Update: " << result << std::endl;
				// Hier würdest du das Ergebnis in die GUI einfügen (z. B. ein Memo-Feld aktualisieren).
			}
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}

// Hauptprogramm mit Boost
int main() {
	// Fenster erstellen (simplifiziert für das Beispiel)
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = "BoostExampleClass";

	if (!RegisterClass(&wc)) {
		std::cerr << "Fensterklasse konnte nicht registriert werden.\n";
		return 1;
	}

	mainWindowHandle = CreateWindowEx(0, "BoostExampleClass", "Boost Thread Beispiel",
									  0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
									  CW_USEDEFAULT, NULL, NULL, wc.hInstance, NULL);

	if (!mainWindowHandle) {
		std::cerr << "Fenster konnte nicht erstellt werden.\n";
		return 1;
	}

	// Starte den Boost-Thread
	boost::thread dbThread(databaseThreadFunction);

	// Nachrichten-Schleife
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Warten auf Thread-Ende
	dbThread.join();
	return 0;
}
Erklärung:
Datenbankabfrage im Boost-Thread:
*/

/*
Beispiel mit boost::mutex
cpp
#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include <queue>

// Globale Variablen für die Thread-Kommunikation
std::queue<std::string> resultQueue;
boost::mutex queueMutex;

// Thread-Funktion
void databaseThreadFunction() {
	try {
		// Simulierte Datenbankabfrage
		boost::this_thread::sleep_for(boost::chrono::seconds(2)); // Simulierte Verzögerung
		std::string result = "SELECT TOP (1) * FROM YourTable -> Ergebnis";

		// Ergebnis in die Warteschlange einfügen (geschützt durch Mutex)
		{
			boost::mutex::scoped_lock lock(queueMutex);
			resultQueue.push(result);
		}
		std::cout << "Thread: Ergebnis hinzugefügt." << std::endl;

	} catch (const std::exception& e) {
		std::cerr << "Fehler: " << e.what() << std::endl;
	}
}

// Hauptprogramm
int main() {
	try {
		// Starte den Boost-Thread
		boost::thread dbThread(databaseThreadFunction);

		// Warte auf die Thread-Beendigung
		dbThread.join();

		// Verarbeite die Ergebnisse
		while (!resultQueue.empty()) {
			boost::mutex::scoped_lock lock(queueMutex);
			std::string result = resultQueue.front();
			resultQueue.pop();
			std::cout << "Hauptthread: " << result << std::endl;
		}
	} catch (const std::exception& e) {
		std::cerr << "Fehler im Hauptthread: " << e.what() << std::endl;
	}
	return 0;
}
Erklärung:
Boost-Mutex (boost::mutex):

*/


//Unit1.h

//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "DBAccess.hpp"
#include "MemDS.hpp"
#include "MSAccess.hpp"
#include <Data.DB.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TButton *Button1;
	TMemo *Memo1;
	TButton *Button2;
	TMSConnection *MSConnection1;
	TMSQuery *MSQuery1;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	void __fastcall QVSyOnClick(TObject *Sender);
	void __fastcall ManuellStartenOnClick(TObject *Sender);
	void __fastcall QueryTestOnClick(TObject *Sender);
	void __fastcall MitWinApiOnClick(TObject *Sender, TPoint &MousePos, bool &Handled);
	void __fastcall QueryTestMutexOnClick(TObject *Sender);

private:	// Benutzer-Deklarationen

	//DB Anbindung Beispiel
	TMSConnection* MSCon;

	TMSQuery* Q4SQL;

	void IniDB(int optionWhichDBServer);
	void CreateExplicitMSCon();
	void Ini();
	bool HandleQuerys(int option);

public:		// Benutzer-Deklarationen

    __fastcall ~TForm1();
	__fastcall TForm1(TComponent* Owner);
	void TestUmgebung();
	void TestUmgebung_ThreadManuellStarten();

    void __fastcall UpdateMemo();



};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif


/*
void __fastcall TForm1::MitWinApiOnClick(TObject *Sender, TPoint &MousePos, bool &Handled)
{
	rrMain_();
}

void __fastcall TForm1::UpdateMemo()
{
	Form1->Memo1->Lines->Add("Thread läuft...\n");
}

DWORD WINAPI rrThreadFunc(LPVOID lpParam)
{
	// GUI-Aktualisierung im Haupt-Thread
	TThread::Synchronize(NULL,  {
		Form1->UpdateMemo();
	});
	return 0;
}

int rrMain_()
{
	HANDLE hThread = CreateThread(
		NULL,         // Sicherheitsattribute
		0,            // Stackgröße (0 = Standard)
		rrThreadFunc,   // Startadresse des Threads
		NULL,         // Parameter für Thread-Funktion
		0,            // Erstellungsflags (0 = sofort starten)
		NULL          // Thread-ID (optional)
	);

	if (hThread == NULL) {
		ShowMessage("Fehler beim Erstellen des Threads.\n");
		return -1;
	}

	// Warte auf die Thread-Beendigung
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	return 0;
}
*/








