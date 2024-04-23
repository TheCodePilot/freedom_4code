//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RRMainForm01.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Frame01"
#pragma resource "*.dfm"
TMainForm01 *MainForm01;

static void Create_XMLFrame(){
		TFrame4XML* Frame4XML = NULL;
		Application->CreateForm(__classid(TFrame4XML),&Frame4XML);
	}
//---------------------------------------------------------------------------
__fastcall TMainForm01::TMainForm01(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm01::FrameMainOnShow(TObject *Sender)
{
	Create_XMLFrame();
}
//---------------------------------------------------------------------------

