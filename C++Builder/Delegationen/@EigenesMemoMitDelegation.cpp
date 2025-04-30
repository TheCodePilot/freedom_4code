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
private:	// Benutzer-Deklarationen

	void PopUp();

public:		// Benutzer-Deklarationen

    TForm *FormPopUp;
	TMemo *MemoPopUp;

	void __fastcall MemoPopUpExit(TObject *Sender);
	void __fastcall FormPopUpQueryClose(TObject *Sender, bool &CanClose);

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
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
	FormPopUp = NULL;
	MemoPopUp = NULL;

	PopUp();
}
//---------------------------------------------------------------------------

__fastcall TForm5::~TForm5()
{
    if (MemoPopUp) {
        delete MemoPopUp;
        MemoPopUp = NULL;
    }

    if (FormPopUp) {
        delete FormPopUp;
        FormPopUp = NULL;
    }
}

void TForm5::PopUp()
{
	
	if(MemoPopUp)
	{
		delete MemoPopUp;
		MemoPopUp = NULL;
	}
	
	if(FormPopUp)
	{
		delete FormPopUp;
		FormPopUp = NULL;
	}

	FormPopUp = new TForm(this);
    //Entscheidend !!!
	FormPopUp->Parent = Form5;
	FormPopUp->Name = "FormPopUp";
	FormPopUp->BorderStyle = bsDialog;
	FormPopUp->Align = alNone;
	FormPopUp->Width = 300;  // Beispielgröße setzen
	FormPopUp->Height = 200;

	MemoPopUp = new TMemo(FormPopUp);
	MemoPopUp->Parent = FormPopUp;
	MemoPopUp->Name = "MemoPopUp";
	MemoPopUp->Align = alClient;

	FormPopUp->OnCloseQuery = &FormPopUpQueryClose;
	// Wichtig: OnMouseLeave statt OnExit!
	MemoPopUp->OnMouseLeave = &MemoPopUpExit;

	FormPopUp->Show();
}

void __fastcall TForm5::MemoPopUpExit(TObject *Sender)
{
	Form5->FormPopUp->Close();
	//ShowMessage("!FormPopUpClose");
}

//Auch bei OnMouseLeave durchlaufen
void __fastcall TForm5::FormPopUpQueryClose(TObject *Sender, bool &CanClose)
{
	int Antwort = MessageDlg("Möchtest du wirklich schließen?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
	if(Antwort == mrYes)
	{
        ;//Irgendwas zu tun Beispiel Memo-Fels auf anderes Memo übertragen
    }
	CanClose = (Antwort == mrYes);
}



