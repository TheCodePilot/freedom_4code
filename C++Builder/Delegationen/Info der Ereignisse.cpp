MyGetEditText
	MyOnClick
		MyMouseUp
			!MyOnMouseLeave
				MySetEditText //schreiben //pro Zeichen ; Zeichen 1
					MyKeyUp //Schreiben fertig //pro Zeichen ; Zeichen 1
				MySetEditText //schreiben ; Zeichen 2
					MyKeyUp //Schreiben fertig ; Zeichen 2
					
MyOnMouseLeave //OK weil echtes 	



//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TStringGrid *StringGrid1;
	TButton *Button1;
	void __fastcall MyOnMouseLeave(TObject *Sender);
	void __fastcall MyOnClick(TObject *Sender);
	void __fastcall MyGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall MySetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall MySelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall MyFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall MyKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MyOnMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall FormOnEnter(TObject *Sender);
	void __fastcall MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall OnClick(TObject *Sender);





private:	// Benutzer-Deklarationen

    TForm* DynamicForm;
	TStringGrid* StringGrid0;

	bool bOnlyOneMessageBox;
	bool bOnlyOneOnClick;
	bool bOnlyOneMouseUp;
    bool bGetEditText;

	String strEreignisse;
	String strEreignisKette;

	void __fastcall FormQueryClose(TObject *Sender, bool &CanClose);
	void __fastcall StringGridExit(TObject *Sender);

	//==============================================================
	void __fastcall GridSelectCell(TObject *Sender, int Col, int Row, bool &CanSelect);
	void __fastcall StringGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);

	void CreateDynamicForm();




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
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{
	//Form5->OnClose = &FormOnClose;
	Form5->OnCloseQuery = &FormQueryClose;
	StringGrid1->OnMouseLeave = &MyOnMouseLeave; // &StringGridExit;

	/*StringGrid1->ColWidths[0] = 100; // Linke Spalte
	StringGrid1->ColWidths[1] = 50; // Mittlere Spalte (4x der rechten Spalte)
	StringGrid1->ColWidths[2] = 200;  // Rechte Spalte

	StringGrid1->Align = alClient;
	*/

	bOnlyOneMessageBox = false;
	bOnlyOneOnClick = false;
	bOnlyOneMouseUp = false;
	bGetEditText = false;

	strEreignisse = " | ";
	strEreignisKette = "";
}

__fastcall TForm5::~TForm5()
{
	if(StringGrid0)
		delete StringGrid0;
	if(DynamicForm)
		delete DynamicForm;
}

//4-5--------------------------------------------------------------------------
void __fastcall TForm5::MyOnMouseLeave(TObject *Sender)
{
    //Debug-Ketten-Test
	//if( /*(strEreignisse.Pos("@return") == 0) && */(strEreignisse.Pos("GetEditText")) && (strEreignisse.Pos("OnClick")) /*&& (strEreignisse.Pos("MouseUp"))*/ )
	if( bOnlyOneOnClick && bGetEditText ) //dies sonst Probleme mit TStringGrid
	{
		strEreignisKette = strEreignisse;
		strEreignisse = "MyOnMouseLeave@return |";
		bOnlyOneOnClick = false;
		bGetEditText = false;
		return;
	}

	strEreignisse = strEreignisse + "MyOnMouseLeave | ";
	if(!bOnlyOneMessageBox)
	{
		bOnlyOneMessageBox = true;
		//ShowMessage("MyOnMouseLeave");
	}
	//TCloseAction* Action = new TCloseAction; //NULL
	//Form5->FormOnClose(NULL, &Action);
    Form5->Close();

}
//2---------------------------------------------------------------------------
void __fastcall TForm5::MyOnClick(TObject *Sender)
{
	bOnlyOneOnClick = true;
	strEreignisse = strEreignisse + "MyOnClick | ";
}
//1---------------------------------------------------------------------------
void __fastcall TForm5::MyGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value)
{
    bGetEditText = true;
	strEreignisse = strEreignisse + "MyGetEditText | ";
}
//---------------------------------------------------------------------------
void __fastcall TForm5::MySetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value)
{
	strEreignisse = strEreignisse + "MySetEditText | ";
}
//---------------------------------------------------------------------------
void __fastcall TForm5::MySelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
	//strEreignisse = strEreignisse + "MySelectCell | ";
	CanSelect = (ACol != 0) && (ARow != 0);
}
//---------------------------------------------------------------------------
void __fastcall TForm5::MyFixedCellClick(TObject *Sender, int ACol, int ARow)
{
	strEreignisse = strEreignisse + "MyFixedCellClick | ";
}
//---------------------------------------------------------------------------
void __fastcall TForm5::MyKeyUp(TObject *Sender, WORD &Key, TShiftState Shift)
{
	strEreignisse = strEreignisse + "MyKeyUp | ";
}
//3---------------------------------------------------------------------------
void __fastcall TForm5::MyOnMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	strEreignisse = strEreignisse + "MyMouseUp | ";
}
//6--------------------------------------------------------------------------
void __fastcall TForm5::FormOnEnter(TObject *Sender)
{
	strEreignisse = strEreignisse + "FormOnEnter | ";
}
//---------------------------------------------------------------------------
void __fastcall TForm5::MyMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	strEreignisse = strEreignisse + "MyMouseDown | ";
}
//---------------------------------------------------------------------------


/*

MyGetEditText
	MyOnClick
		MyMouseUp
			!MyOnMouseLeave
				MySetEditText //schreiben //pro Zeichen ; Zeichen 1
					MyKeyUp //Schreiben fertig //pro Zeichen ; Zeichen 1
				MySetEditText //schreiben ; Zeichen 2
					MyKeyUp //Schreiben fertig ; Zeichen 2

MyOnMouseLeave //OK weil echtes

*/



void __fastcall TForm5::FormQueryClose(TObject *Sender, bool &CanClose)
{
	//int Antwort = MessageDlg("Möchtest du wirklich schließen?", mtConfirmation, TMsgDlgButtons() << mbYes << mbNo, 0);
	//if(Antwort == mrYes) { ;//Irgendwas zu tun Beispiel Memo-Fels auf anderes Memo übertragen }
    ShowMessage(strEreignisse);
	CanClose = true; //(Antwort == mrYes);
}

void __fastcall TForm5::StringGridExit(TObject *Sender)
{
	Form5->Close();
	//ShowMessage("!FormPopUpClose");
}







void __fastcall TForm5::GridSelectCell(TObject *Sender, int Col, int Row, bool &CanSelect)
{
	// Erste Spalte nicht editierbar machen
	CanSelect = (Col != 1);
}

void TForm5::CreateDynamicForm()
{
	// Neues Form erstellen
	DynamicForm = new TForm(this);
	DynamicForm->Width = 2*350; //400;
	DynamicForm->Height = 2*150; //300;
	DynamicForm->BorderStyle = bsNone; //bsDialog;
	DynamicForm->Caption = "Dynamisches Grid";

	// Grid erstellen und als Child setzen
	StringGrid0 = new TStringGrid(DynamicForm);
	StringGrid0->Parent = DynamicForm;
	StringGrid0->Align = alClient;

    StringGrid0->Options << goEditing;

	// Anzahl der Spalten & Zeilen setzen
	StringGrid0->ColCount = 3;
	StringGrid0->RowCount = 7; //6; //10; // Variable Anzahl möglich

	// Spaltenbreiten setzen (rechte Spalte 50px, mittlere Spalte 200px)
	StringGrid0->ColWidths[0] = 100; // Linke Spalte
	StringGrid0->ColWidths[1] = 50; // Mittlere Spalte (4x rechte Spalte)
	StringGrid0->ColWidths[2] = 200;  // Rechte Spalte

	// Erste Spalte als readonly setzen – mit Methode
	StringGrid0->OnSelectCell = &GridSelectCell;
	//StringGrid0->OnDrawCell = &OnDrawCellStatic; //&TForm5::StringGridDrawCell; //&StringGridDrawCell;


	int i = GetSystemMetrics(SM_CYBORDER);
	i =  GetSystemMetrics(SM_CXDLGFRAME);
	//Höhe nach korrigieren
	int gridHeight = (StringGrid0->DefaultRowHeight * StringGrid0->RowCount) // Zeilenhöhe
			   + StringGrid0->DefaultRowHeight // Kopfzeile
			   -14; // Kleiner zusätzlicher Puffer

	int formHeight = gridHeight
			   // + GetSystemMetrics(SM_CYCAPTION) // Raus: Fenster-Titelleiste wegen bsNone;
			   + 2 * GetSystemMetrics(SM_CYDLGFRAME) // Fenster-Rahmen
			   -6; //Rein wegen bsNone

	DynamicForm->Height = formHeight;


	int gridWidth = StringGrid0->ColWidths[0]
			  + StringGrid0->ColWidths[1]
			  + StringGrid0->ColWidths[2]
			  + 4; // Kleiner zusätzlicher Puffer

	int formWidth = gridWidth
			  + 2 * GetSystemMetrics(SM_CXDLGFRAME); // Fenster-Rahmen

	DynamicForm->Width = formWidth - 4; // + 2; // - 14; // Feinanpassung wie bei der Höhe

	DynamicForm->Show();
}


void __fastcall TForm5::OnClick(TObject *Sender)
{
	CreateDynamicForm();
}
//---------------------------------------------------------------------------


//funktioniert nicht
void __fastcall TForm5::StringGridDrawCell(TObject *Sender, int Col, int Row, TRect &Rect, TGridDrawState State)
{
    TStringGrid *Grid = (TStringGrid*)Sender;
    if (Row == 0) // Nur die Header-Zeile (erste Zeile)
    {
		Grid->Canvas->Brush->Color = clSilver; // Hintergrundfarbe setzen
        Grid->Canvas->FillRect(Rect); // Rechteck füllen

        // Beispiel: Ein kleines Icon zeichnen (ersetzbar durch eine echte Bilddatei)
        Graphics::TBitmap *img = new Graphics::TBitmap();
		img->LoadFromFile("D:\\test.bmp"); //("icon.bmp"); // Pfad zum Bild setzen

        int imgWidth = 16; // Beispielgröße des Bildes
        int imgHeight = 16;
        int imgX = Rect.Left + 4; // Position im Header
        int imgY = Rect.Top + (Rect.Bottom - Rect.Top - imgHeight) / 2;

        Grid->Canvas->Draw(imgX, imgY, img); // Bild zeichnen

        delete img; // Speicher freigeben

        // Header-Text zeichnen
        Grid->Canvas->Font->Color = clBlack;
        Grid->Canvas->TextOut(Rect.Left + imgWidth + 8, Rect.Top + 4, Grid->Cells[Col][Row]);
    }
}





				