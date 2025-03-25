class TCustomStaticText : public TStaticText
{
protected:
    void __fastcall WndProc(TMessage &Message);

private:
    COLORREF textColor;
    bool customDrawing;

public:
    __fastcall TCustomStaticText(TComponent* Owner)
        : TStaticText(Owner)
    {
        customDrawing = true;
        textColor = RGB(0, 0, 255); // Standard: Blau
    }

    void __fastcall MyCustomPaint()
    {
        SendMessage(this->Handle, WM_PAINT, 0, 0);
    }

    void __fastcall SetTextColor(COLORREF color)
    {
        textColor = color;
    }

    void __fastcall EnableCustomDrawing(bool enable)
    {
        customDrawing = enable;
    }
};

void __fastcall TCustomStaticText::WndProc(TMessage &Message)
{
    if (Message.Msg == WM_PAINT && customDrawing)
    {
        HDC hDC = ::GetDC(this->Handle);
        if (hDC != NULL)
        {
            RECT rect;
            ::GetClientRect(this->Handle, &rect);

            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(hDC, textColor);

            DrawText(hDC, this->Caption.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            ::ReleaseDC(this->Handle, hDC);
        }
        return; // Verhindert die Standardbehandlung
    }

    TStaticText::WndProc(Message);
}

//--------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    // Erstellen einer Instanz von TCustomStaticText
    TCustomStaticText *customStaticText = new TCustomStaticText(this);

    // Festlegen des Elternteils
    customStaticText->Parent = this;

    // Platzierung und Größenanpassung
    customStaticText->Left = 50;
    customStaticText->Top = 50;
    customStaticText->Width = 200;
    customStaticText->Height = 50;

    // Text-Eigenschaften setzen
    customStaticText->SetTextColor(RGB(255, 0, 0)); // Rot
    customStaticText->Caption = "Hallo, Rudolf!";
    customStaticText->Font->Style = TFontStyles() << fsBold;

    // Custom Drawing aktivieren
    customStaticText->EnableCustomDrawing(true);

    // Optional: Manuelles Neuzeichnen des Controls
    customStaticText->MyCustomPaint();
}


//========================

class CustomStaticText : public TStaticText
{
protected:
    void __fastcall WndProc(TMessage &Message);

private:
    COLORREF textColor;
    bool customDrawing;

public:
	__fastcall CustomStaticText(TComponent* Owner)
        : TStaticText(Owner)
    {
        customDrawing = true;
        textColor = RGB(0, 0, 255); // Standard: Blau
    }

    void __fastcall MyCustomPaint()
    {
        SendMessage(this->Handle, WM_PAINT, 0, 0);
    }

    void __fastcall SetTextColor(COLORREF color)
    {
        textColor = color;
    }

    void __fastcall EnableCustomDrawing(bool enable)
    {
        customDrawing = enable;
    }
};

void __fastcall CustomStaticText::WndProc(TMessage &Message)
{
    if (Message.Msg == WM_PAINT && customDrawing)
    {
        HDC hDC = ::GetDC(this->Handle);
        if (hDC != NULL)
        {
            RECT rect;
            ::GetClientRect(this->Handle, &rect);

            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(textColor);

            DrawText(hDC, this->Caption.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            ::ReleaseDC(this->Handle, hDC);
        }
        return; // Verhindert die Standardbehandlung
    }

    TStaticText::WndProc(Message);
}


class CustomStaticText;

class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
private:	// Benutzer-Deklarationen
	void __fastcall MyFormCreate();
    CustomStaticText *customStaticText;

public:		// Benutzer-Deklarationen
	__fastcall TForm5(TComponent* Owner);
};


//-------------------------------------

void __fastcall TForm5::MyFormCreate()
{
    // Erstellen einer Instanz von TCustomStaticText
    customStaticText = new CustomStaticText(this);

    // Festlegen des Elternteils
    customStaticText->Parent = this;

    // Platzierung und Größenanpassung
    customStaticText->Left = 50;
    customStaticText->Top = 50;
    customStaticText->Width = 200;
    customStaticText->Height = 50;

    // Text-Eigenschaften setzen
    customStaticText->SetTextColor(RGB(255, 0, 0)); // Rot
    customStaticText->Caption = "Hallo, Rudolf!";
    customStaticText->Font->Style = TFontStyles() << fsBold;

    // Custom Drawing aktivieren
    customStaticText->EnableCustomDrawing(true);

    // Optional: Manuelles Neuzeichnen des Controls
    customStaticText->MyCustomPaint();
}

//----------------------------
//Um flackern zu verhindern ist es besser

void __fastcall TCustomStaticText::WndProc(TMessage &Message)
{
    if (Message.Msg == WM_ERASEBKGND)
    {
        // Blockiert die Hintergrundlöschung, um den Hintergrund zu erhalten
        Message.Result = 1;
        return;
    }

    if (Message.Msg == WM_PAINT && customDrawing)
    {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(this->Handle, &ps);

        if (hDC != NULL)
        {
            RECT rect;
            ::GetClientRect(this->Handle, &rect);

            // Setzt den Textmodus auf transparent, sodass der Hintergrund nicht überschrieben wird
            SetBkMode(hDC, TRANSPARENT);

            // Text-Farbe setzen
            SetTextColor(hDC, textColor);

            // Nur den Text zeichnen, ohne den Hintergrund zu beeinflussen
            DrawText(hDC, this->Caption.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Kontext freigeben
        }

        EndPaint(this->Handle, &ps);
        Message.Result = 0; // Markiert die Nachricht als verarbeitet
        return;
    }

    TStaticText::WndProc(Message);
}

/=====================================Morgen================================

//---------------------------------------------------------------------------

#ifndef LastwechselThreadH
#define LastwechselThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class TLastwecheslberchungThread : public TThread
{
private:
protected:
	// Diese Funktion representiert den Thread, alles was gemacht werden soll muss hier rein.
	void __fastcall Execute();
public:

		TMSQuery 	*QTab;
		TMSQuery 	*QLastW;


	// Konstuktor mit den übergabewerten Suspended-> ausgesetzt oder immediatly -> sofort
	__fastcall TLastwecheslberchungThread(bool CreateSuspended);
	void __fastcall UpdateCaption();
};
//---------------------------------------------------------------------------
#endif


//-----------------------------------------------------------------------------
#include "_CommonHeaders.h" // muss oben stehen bleiben (Precompiled Header)
#include "_Defines.h"
#include "_ExternalVars.h"
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop


#include "LastwechselThread.h"
#include "Unit103B_Druckkessel.h"

#pragma package(smart_init)

#include <System.SyncObjs.hpp> // Für TMutex
//---------------------------------------------------------------------------

//   Wichtig: Methoden und Eigenschaften von Objekten in der VCL können nur
//   in Methoden verwendet werden, die mit 'Synchronize' aufgerufen wurden, z.B.:
//
//      Synchronize(&UpdateCaption);
//
//   wobei UpdateCaption so aussehen könnte:
//

//OK
void __fastcall TLastwecheslberchungThread::UpdateCaption()
{
	Form103B->UpdateCaption();
}
//---------------------------------------------------------------------------
/*
//OK

__fastcall TLastwecheslberchungThread::TLastwecheslberchungThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	;
}
//---------------------------------------------------------------------------
//OK

void __fastcall TLastwecheslberchungThread::Execute()
{
	Sleep(10);
	Synchronize(&UpdateCaption);
}
*/








/*
 void __fastcall TLastwecheslberchungThread::UpdateCaption()
{
		 // Tabelle wird neu eingelesen
	   //	 Form103B->UpdateSelect(0);

Form103B->lblLastWPrzThr->Caption   = "Lastwechselaktualisierung vom: " + Now().FormatString("dd.mm.yyyy hh:nn");
Form103B->lblLastWPrzThr->Font->Color = clGreen;
//Form103B->lblLastWPrzThrCustomized->Caption   = "Lastwechselaktualisierung vom: " + Now().FormatString("dd.mm.yyyy hh:nn");
//Form103B->lblLastWPrzThrCustomized->SetFontColor(clGreen);



 }  */
//---------------------------------------------------------------------------

__fastcall TLastwecheslberchungThread::TLastwecheslberchungThread(bool CreateSuspended)
	: TThread(CreateSuspended)
{
	QTab=NULL;
	QLastW = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TLastwecheslberchungThread::Execute()
{


	//FreeOnTerminate = true;
	//Sleep(100);
	TMutex *Mymutex = new TMutex(true); //sofort für andere diesen Mutex sperren
	try
	{
	   Mymutex->Acquire();



		AnsiString sql, where="", WNummer;
	int Result = 0;

	  QTab=new TMSQuery(NULL);
	  QLastW=new TMSQuery(NULL);
	   //Initialized=(QTab!=NULL);

	// Einlesen der Tabelle um an die WNummer für die Zählung zu gelangen
	sql="SELECT TOP(10000) * FROM dbo.Hilfsmittel WHERE Protokoll_XTYP = 'Form103' ORDER BY ID DESC ";
	Form1->SendSQLLine(sql, QTab);


   // Tabelle wird durchgegangen (äußere Schleife)
   QTab->First();
   while(!QTab->Eof)
   {

	 // WNummer wird geholt und das Ergenis zurückgesetzt
	 WNummer= QTab->FieldByName("WNummer")->AsString.Trim();
	 Result = 0;


	 // Zählung der Einträge in [MDE].[dbo].[PROTOKOL] Felder HM1-4
	 for(int i=0; i<4; i++)
	 {

		 //sql="SELECT COUNT(HM1) FROM [MDE].[dbo].[PROTOKOL] Where HM1 = '" +WNummer+  "'";
		 sql="SELECT COUNT(HM"+IntToStr(i+1)+") FROM [MDE].[dbo].[PROTOKOL] Where HM"+IntToStr(i+1)+" = '" +WNummer+  "'";
		 Form1->SendSQLLine(sql, QLastW);
		 Result = Result + QLastW->Fields->Fields[0]->AsInteger;

	 }

	 // Einträge in dbo.Hilfsmittel werden aktualisiert
	 Form1->UpdateDboItem("Hilfsmittel","IstLastwechsel='"+IntToStr(Result)+"'","WNummer = '"+WNummer+"'");


	 QTab->Next();
   }




			   delete  QTab;
		QTab=NULL;

				   delete   QLastW;
		 QLastW=NULL;






	// Es ist nicht sicher, Windows GUI Applikationen aus einem Thread heraus zu updaten
	// daher muss das Update der GUI über die Methode Synchronize() erfolgen.
	// Snchronize() signalisiert dem MainThread dass etwas in der GUI geupdaten werden muss
	// dies geschieht aus der Funktion heraus und nicht aus dem Thread

	// Als Übergabeparamter muss spezifiziert werden welche Methode zum Updaten verwendet
	// werden soll.

	Synchronize(&UpdateCaption);
	 }
     __finally
    {
		Mymutex->Release();
		delete Mymutex;
	}

}

//---------------------------------------------------------------------------				
