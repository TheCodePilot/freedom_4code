//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "Unit6.h"
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TFrame6 *Frame61;
	void __fastcall FormOnCreate(TObject *Sender);
private:	// Benutzer-Deklarationen
	TFrame6* MyFrame6;
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
#pragma hdrstop

#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Unit6"
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
	MyFrame6 = new TFrame6(this);

	if (Frame61)
		; //Application->OnMessage = Frame61->AppOnMessage;  Da ich WndProc nutze nicht mehr nötig
	else
		ShowMessage("NIX");
}

__fastcall TForm5::~TForm5()
{
	if (MyFrame6)
	{
		delete MyFrame6;
        MyFrame6 = NULL;
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm5::FormOnCreate(TObject *Sender)
{
	//Application->OnMessage = Frame6->AppOnMessage;
}
//---------------------------------------------------------------------------




#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFrame6 : public TFrame
{
__published:	// Von der IDE verwaltete Komponenten
private:	// Benutzer-Deklarationen
    bool bMessage;
public:		// Benutzer-Deklarationen
	void __fastcall AppOnMessage_WM_SETFOCUS(tagMSG &Msg, bool &Handled);
	void __fastcall AppOnMessage(tagMSG &Msg, bool &Handled);
    void __fastcall WndProc(TMessage &Message);
	void StartMouseLeaveTracking();
	void MySave();
	__fastcall TFrame6(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrame6 *Frame6;
//---------------------------------------------------------------------------
#endif



//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit6.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFrame6 *Frame6;
/*
#include <windowsx.h>  // für GET_X_LPARAM etc.
#include <CommCtrl.h>
#include <WinUser.h>
*/
//---------------------------------------------------------------------------
__fastcall TFrame6::TFrame6(TComponent* Owner)
	: TFrame(Owner)
{
	bMessage = false;
}
//---------------------------------------------------------------------------

/* 	//1
void __fastcall TFrame6::AppOnMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_MOUSEACTIVATE) //WM_SETFOCUS)
	{
		HWND hNewFocus = (HWND)Msg.wParam;

		// Prüfen: Ist der neue Fokus außerhalb dieses Frames?
		if (!::IsChild(Handle, hNewFocus))
		{
			// Fokus hat Frame verlassen
			if(!bMessage)
			{
				bMessage = true;
				ShowMessage("WM_MOUSEACTIVATE");
			}
//MySave();  // Zwischenspeichern aufrufen
		}
	}

	//if (Msg.message == CM_EXIT) //WM_SETFOCUS)



}

	//2
void __fastcall TFrame6::AppOnMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_MOUSEACTIVATE || Msg.message == WM_LBUTTONDOWN)
	{
		// Aktuellen Fokus holen
		HWND hFocus = ::GetFocus();

		if (hFocus && !::IsChild(Handle, hFocus))
		{
			if (!bMessage)
			{
				bMessage = true;
				ShowMessage("Fokus ist NICHT im Frame");
				// MySave();
			}
		}
		else
		{
			// Fokus ist wieder im Frame
			bMessage = false;
		}
	}
}

*/



void TFrame6::StartMouseLeaveTracking()
{
	TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT) };
	tme.dwFlags = TME_LEAVE;
	tme.hwndTrack = Handle;
	tme.dwHoverTime = HOVER_DEFAULT;
	TrackMouseEvent(&tme);
}

void __fastcall TFrame6::WndProc(TMessage &Message)
{
	switch (Message.Msg)
	{
		case WM_MOUSEMOVE:
			StartMouseLeaveTracking();
			break;

		case WM_MOUSELEAVE:
			ShowMessage("Maus hat Frame verlassen");
			break;
	}

	TFrame::WndProc(Message);  // Basisklasse nicht vergessen!
}

void __fastcall TFrame6::AppOnMessage(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_LBUTTONDOWN || Msg.message == WM_MOUSEACTIVATE)
	{
		// 1. Mausklickposition im Bildschirm
		POINT pt = Msg.pt;

		// 2. Rechteck des Frames im Bildschirm
		TRect frameRect;
		GetWindowRect(Handle, &frameRect);

		// 3. Prüfen ob der Punkt im Frame liegt
		if (!PtInRect(&frameRect, pt))
		{
			if (!bMessage)
			{
				bMessage = true;
				ShowMessage("Klick außerhalb des Frames > Zwischenspeichern");
				// MySave();
			}
		}
		else
		{
			bMessage = false; // Reset wenn wieder im Frame
		}
	}
}

//Nicht OK
//pt { 1062, 582 }
//frameRect { 963, 566, 1283, 806 }

//Wenn angeblich OK
//pt { 1169, 535 }
//frameRect{ 963, 566, 1283, 806 }


void __fastcall TFrame6::AppOnMessage_WM_SETFOCUS(tagMSG &Msg, bool &Handled)
{
	if (Msg.message == WM_SETFOCUS)
	{
		HWND hNewFocus = (HWND)Msg.wParam;

		// Prüfen: Ist der neue Fokus außerhalb dieses Frames?
		if (!::IsChild(Handle, hNewFocus))
		{
			if (!bMessage)
			{
				bMessage = true;
				ShowMessage("WM_SETFOCUS zu Exit");
				// MySave();  // Optional aufrufen
			}
		}
		else
		{
			bMessage = false; // Reset, wenn Fokus wieder im Frame
		}
	}
}