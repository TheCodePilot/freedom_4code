//---------------------------------------------------------------------------

#ifndef Unit5_FileDialog_testH
#define Unit5_FileDialog_testH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TOpenDialog *OpenDialog1;
	TOpenTextFileDialog *OpenTextFileDialog1;
	TButton *Button1;
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	__fastcall TForm5(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
