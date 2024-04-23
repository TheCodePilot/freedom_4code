//---------------------------------------------------------------------------

#ifndef RRMainForm01H
#define RRMainForm01H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "Frame01.h"
//---------------------------------------------------------------------------
class TMainForm01 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TFrame4XML *Frame4XML1;
	void __fastcall FrameMainOnShow(TObject *Sender);
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	__fastcall TMainForm01(TComponent* Owner);
	void __fastcall IchMainForm(){;}

};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm01 *MainForm01;
//---------------------------------------------------------------------------
#endif
