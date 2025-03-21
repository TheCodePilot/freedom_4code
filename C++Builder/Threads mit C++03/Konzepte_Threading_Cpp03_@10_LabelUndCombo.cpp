//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class MyCustomLabel;

class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TButton *Button1;
	void __fastcall OnClick(TObject *Sender);
private:	// Benutzer-Deklarationen

public:		// Benutzer-Deklarationen
	MyCustomLabel *CustomLabel;
	__fastcall TForm5(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif




//Cpp


#include <vcl.h>
#pragma hdrstop
#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;

class MyCustomLabel : public TLabel // Benutzerdefinierte Klasse
{
protected:

	TColor TextColor; // Variable für die Textfarbe
	void __fastcall Paint(void)
	{
		Canvas->Brush->Style = bsClear; // Hintergrund transparent
		Canvas->Font->Color = TextColor;    // Standardfarbe ist Rot
		Canvas->TextOut(0, 0, Caption); // Text zeichnen
	}

public:
	__fastcall MyCustomLabel(TComponent* Owner) : TLabel(Owner) {}
	void __fastcall SetFontColor(TColor NewColor)
	{
		TextColor = NewColor; // Textfarbe speichern
		Invalidate(); // Neu zeichnen
	}
};

__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
	 // Erstellen des benutzerdefinierten Labels
	CustomLabel = new MyCustomLabel(this);
	CustomLabel->Parent = this;
	CustomLabel->SetBounds(50, 50, 200, 30);
	CustomLabel->Caption = L"Benutzerdefinierter Text";
	// Speicherung des Labels in einer Member-Variable
//this->CustomLabel = CustomLabel; redundant oder ?
}

//Mit dem Designer Button und Ereignis generiert
void __fastcall TForm5::OnClick(TObject *Sender)
{
	if (CustomLabel)
	{
		CustomLabel->SetFontColor(clSkyBlue); // Ändert die Schriftfarbe auf Blau
	}
}