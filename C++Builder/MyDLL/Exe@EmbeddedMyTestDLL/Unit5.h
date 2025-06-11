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
	TButton *Button1;
	TLabel *Label1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall OnClickTerminate(TObject *Sender);
private:	// Benutzer-Deklarationen
    //RRRNeu
	HINSTANCE hDLL;
	String dllPath;
	String folderPath;
	HINSTANCE ExtractAndLoadMyDLL();

	TTimer *cleanupTimer;

	void __fastcall TryDeleteDLLAndFolder(TObject *Sender);

public:		// Benutzer-Deklarationen

	__fastcall TForm5(TComponent* Owner);
    __fastcall ~TForm5();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------


#endif/*
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
	TButton *Button1;
	TLabel *Label1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Benutzer-Deklarationen
	//RRRNeu
	HINSTANCE hDLL;
	String dllPath;
	HINSTANCE ExtractAndLoadMyDLL();

	String folderPath;
	TTimer *cleanupTimer;
	void __fastcall TryDeleteDLLAndFolder(TObject *Sender);

public:		// Benutzer-Deklarationen

	__fastcall TForm5(TComponent* Owner);
	__fastcall ~TForm5();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm5 *Form5;
//---------------------------------------------------------------------------
#endif
*/
