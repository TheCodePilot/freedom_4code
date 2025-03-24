//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TCustomCanvasControl;

class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TButton *Button1;
	void __fastcall Button1Click(TObject *Sender);
private:	// Benutzer-Deklarationen
	TCustomCanvasControl* tcc;
public:		// Benutzer-Deklarationen
    void __fastcall TForm5::UseCustomControl();
	__fastcall TForm5(TComponent* Owner);
	__fastcall ~TForm5();

};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif


//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;

class TCustomCanvasControl : public TCustomControl
{
protected:

	void __fastcall WndProc(TMessage &Message);
	void __fastcall Proc4PAINT(TMessage &Message);

private:
	COLORREF contextColor;
	bool bFillTheRect;
	void __fastcall FillRectangleWithColor(HDC hDC, const RECT &rect);

public:
	__fastcall TCustomCanvasControl(TComponent* Owner)
		: TCustomControl(Owner)
	{
		bFillTheRect = false;
		contextColor = clRed;
		//this->DoubleBuffered = true; // Aktivieren des Double-Buffering
	}

	void __fastcall MyPaint()
	{
		SendMessage(this->Handle, WM_PAINT, 0, 0);
	}

	void __fastcall SetContextColor(COLORREF color)
	{
		contextColor = color;
	}

	void __fastcall SetFillTheRect(bool customerWish)
	{
		bFillTheRect = customerWish;
	}

};


void __fastcall TCustomCanvasControl::FillRectangleWithColor(HDC hDC, const RECT &rect)
{
	if (!bFillTheRect)
	{
		HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW)); // Standardfarbe des Fensters
		FillRect(hDC, &rect, hBrush);
		DeleteObject(hBrush); // GDI-Objekt freigeben
		return;
	}

	// Erstellt einen Pinsel mit der angegebenen Farbe
	HBRUSH hBrush = CreateSolidBrush(contextColor);
	// Füllt das Rechteck mit dem Pinsel
	FillRect(hDC, &rect, hBrush);
	// Freigeben des Pinsels
	DeleteObject(hBrush);
}


void __fastcall TCustomCanvasControl::Proc4PAINT(TMessage &Message)
{
	if (Message.Msg == WM_PAINT)
	{
		HWND hWnd = this->Handle;
		HDC hDC = ::GetDC(hWnd);

		if (hDC != NULL)
		{
			// Abrufen des Rechtecks
			RECT rect;
			::GetClientRect(hWnd, &rect);

			// Rahmen zeichnen (unabhängig von bFillTheRect)
			if ((rect.right - rect.left > 0) && (rect.bottom - rect.top > 0))
			{
				::Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
			}

			// Rechteck ggf. mit Farbe füllen
			FillRectangleWithColor(hDC, rect);

			::ReleaseDC(hWnd, hDC);
		}
	}

}


void __fastcall TCustomCanvasControl::WndProc(TMessage &Message)
{
	// Debug-Ausgabe für jede Nachricht
	wchar_t buffer[256];
	swprintf(buffer, sizeof(buffer)/sizeof(wchar_t), L"Nachricht empfangen: %d", Message.Msg);
	OutputDebugString(buffer);

	if (Message.Msg == WM_ERASEBKGND)
	{
		// WM_ERASEBKGND verhindern, um Flackern zu vermeiden
		OutputDebugString(L"WM_ERASEBKGND wird ignoriert.");
		Message.Result = 1; // Nachricht als verarbeitet markieren
		return;
	}

	if (Message.Msg == WM_MOVE)
	{
		OutputDebugString(L"Das Control wurde verschoben!");
		// Hier kannst du zusätzliche Logik hinzufügen
	}

	if (Message.Msg == WM_SIZE)
	{
		OutputDebugString(L"Das Control wurde in seiner Größe verändert!");
		// Hier kannst du zusätzliche Logik hinzufügen
	}

	if (Message.Msg == WM_PAINT)
	{
		OutputDebugString(L"WM_PAINT wird verarbeitet.");
		Proc4PAINT(Message);
	}

	// Übergibt die Nachricht zur Standardverarbeitung
	TCustomControl::WndProc(Message);
}


//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
	//Bereich nutzen des Custom_Controlls
	tcc = new TCustomCanvasControl(this);
	UseCustomControl();
	//Bereich nutzen des Custom_Controlls
}
//---------------------------------------------------------------------------


__fastcall TForm5::~TForm5()
{
	if(tcc)
	{
		delete tcc;
		tcc = NULL;
	}
}


void __fastcall TForm5::UseCustomControl()
{
	//Bereich nutzen des Custom_Controlls
	tcc->Parent = this; // Setzt das Formular als Eltern
	tcc->Left = 50;     // X-Position
	tcc->Top = 50;      // Y-Position
	tcc->Width = 75;    // Breite
	tcc->Height = 75;   // Höhe

	tcc->SetContextColor(clSkyBlue);
	tcc->SetFillTheRect(true);
	tcc->MyPaint();
}


//Vom Designer Button und Ereignis erstellt
void __fastcall TForm5::Button1Click(TObject *Sender)
{
	tcc->SetContextColor(GetSysColor(COLOR_WINDOW)); //clGreen);
	tcc->SetFillTheRect(false);
	tcc->MyPaint();
}
//---------------------------------------------------------------------------
