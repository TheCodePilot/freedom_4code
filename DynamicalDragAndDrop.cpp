// Unit1.h
//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:    // Von der IDE verwaltete Komponenten
    TListBox *ListBox1;
    TEdit *Edit1;
    void __fastcall ListBox1StartDrag(TObject *Sender, TDragObject *&DragObject);
    void __fastcall Edit1StartDrag(TObject *Sender, TDragObject *&DragObject);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State,
          bool &Accept);
    void __fastcall FormDragDrop(TObject *Sender, TObject *Source, int X, int Y);
private:    // Benutzer-Deklarationen
    void __fastcall AddItemToListBox(TControl *SourceControl);
public:        // Benutzer-Deklarationen
    __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif


// Unit1.cpp
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox1StartDrag(TObject *Sender, TDragObject *&DragObject)
{
    ListBox1->BeginDrag(false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit1StartDrag(TObject *Sender, TDragObject *&DragObject)
{
    Edit1->BeginDrag(false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    ListBox1->DragMode = dmAutomatic;
    Edit1->DragMode = dmAutomatic;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    ListBox1->Items->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State,
      bool &Accept)
{
    Accept = Source == ListBox1 || Source == Edit1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormDragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
    if (Source == ListBox1)
        AddItemToListBox(ListBox1);
    else if (Source == Edit1)
        AddItemToListBox(Edit1);
}

void __fastcall TForm1::AddItemToListBox(TControl *SourceControl)
{
    UnicodeString ItemText = "";

    if (SourceControl == ListBox1)
        ItemText = ListBox1->Items->Strings[ListBox1->ItemIndex];
    else if (SourceControl == Edit1)
        ItemText = Edit1->Text;

    ListBox1->Items->Add(ItemText);
}
//---------------------------------------------------------------------------

//###########################################################################
/*In diesem Beispiel gibt es ein Hauptformular (Form1), das eine ListBox (ListBox1)
und ein Edit-Feld (Edit1) enthält. Beide Komponenten wurden so konfiguriert, 
dass sie automatisch Drag-and-Drop unterstützen (DragMode = dmAutomatic). 
Außerdem gibt es Ereignisse für das Starten des Ziehvorgangs für ListBox1 und Edit1. 
Wenn ein Drag-and-Drop-Vorgang von einer dieser Komponenten gestartet wird, fügt das 
Programm den Text des ausgewählten Elements der ListBox oder den Text des Edit-Felds 
zur ListBox hinzu.

Es gibt auch Ereignisse für das Formular selbst, um das Ziehen und Ablegen zu verwalten
(FormDragOver und FormDragDrop). Das Formular akzeptiert nur Ziehvorgänge von der 
ListBox oder dem Edit-Feld.*/
//###########################################################################

// Unit1.cpp
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListBox1StartDrag(TObject *Sender, TDragObject *&DragObject)
{
    if (!Button1->Enabled) return; // Wenn die Schaltfläche deaktiviert ist, wird das Drag-and-Drop nicht gestartet
    ListBox1->BeginDrag(false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Edit1StartDrag(TObject *Sender, TDragObject *&DragObject)
{
    if (!Button1->Enabled) return; // Wenn die Schaltfläche deaktiviert ist, wird das Drag-and-Drop nicht gestartet
    Edit1->BeginDrag(false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    // Die Schaltfläche wurde geklickt, um das Drag-and-Drop zu sperren oder zu entsperren
    ListBox1->AllowDrag = !ListBox1->AllowDrag;
    Edit1->AllowDrag = !Edit1->AllowDrag;

    if (ListBox1->AllowDrag)
        Button1->Caption = "Drag & Drop sperren";
    else
        Button1->Caption = "Drag & Drop entsperren";
}
//---------------------------------------------------------------------------
