//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit5_FileDialog_test.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
	/*
	TOpenDialog *OpenDialog = new TOpenDialog(this);
	OpenDialog->InitialDir = GetCurrentDir();
	if (OpenDialog->Execute())
	{
		ShowMessage("Datei ausgewählt: " + OpenDialog->FileName);
	}
	delete OpenDialog;
	*/

    //Moderner
	OpenTextFileDialog1 = new TOpenTextFileDialog(this);
	OpenTextFileDialog1->InitialDir = GetCurrentDir();
	if (OpenTextFileDialog1->Execute())
	{
		ShowMessage("Datei ausgewählt: " + OpenTextFileDialog1->FileName);
	}
	delete OpenTextFileDialog1;

}
//---------------------------------------------------------------------------
