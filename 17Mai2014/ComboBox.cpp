Um eine ComboBox zur Laufzeit in einem C++ Builder-Projekt zu erweitern, kannst du einfach die Items-Eigenschaft der ComboBox verwenden, um neue Elemente hinzuzufügen. Hier ist ein Beispiel, wie du das machen kannst:

Beispiel: Hinzufügen von Elementen zur ComboBox zur Laufzeit
Header-Datei (MainForm.h)
cpp
Code kopieren
#ifndef MainFormH
#define MainFormH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.StdCtrls.hpp>

class TMainForm : public TForm
{
__published:
    TComboBox *ComboBox1;
    TButton *AddButton;
    TEdit *ItemEdit;

    void __fastcall FormCreate(TObject *Sender);
    void __fastcall AddButtonClick(TObject *Sender);

public:
    __fastcall TMainForm(TComponent* Owner);
};

extern PACKAGE TMainForm *MainForm;

#endif
Implementierungsdatei (MainForm.cpp)
cpp
Code kopieren
#include <vcl.h>
#pragma hdrstop

#include "MainForm.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TMainForm *MainForm;

__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
}

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
    // Initialisierung von ComboBox und anderen Komponenten
    ComboBox1->Items->Add("Initial Item 1");
    ComboBox1->Items->Add("Initial Item 2");

    AddButton->Caption = "Add Item";
    ItemEdit->Text = "";
}

void __fastcall TMainForm::AddButtonClick(TObject *Sender)
{
    String newItem = ItemEdit->Text;
    if (!newItem.IsEmpty())
    {
        ComboBox1->Items->Add(newItem);
        ItemEdit->Text = ""; // Textfeld nach dem Hinzufügen leeren
    }
    else
    {
        ShowMessage("Please enter a valid item.");
    }
}
Erläuterungen:
ComboBox initialisieren:

Im FormCreate-Ereignis fügst du einige Anfangselemente zur ComboBox hinzu.
ButtonClick-Ereignis:

Das AddButtonClick-Ereignis liest den Text aus einem Editierfeld (ItemEdit) und fügt diesen als neues Element zur ComboBox hinzu.
Komponenten auf dem Formular hinzufügen
Stelle sicher, dass du die folgenden Komponenten auf dein Formular ziehst und wie folgt einrichtest:

ComboBox (ComboBox1):

Platziere eine ComboBox auf dem Formular und benenne sie ComboBox1.
Button (AddButton):

Platziere einen Button auf dem Formular und benenne ihn AddButton.
Setze das OnClick-Ereignis auf AddButtonClick.
Edit (ItemEdit):

Platziere ein Edit-Feld auf dem Formular und benenne es ItemEdit.
Dieses einfache Beispiel zeigt, wie du zur Laufzeit Elemente zu einer ComboBox in C++ Builder hinzufügen kannst. Du kannst diese Methode erweitern, um weitere Funktionalitäten hinzuzufügen, wie z.B. das Löschen von Elementen, das Bearbeiten bestehender Elemente oder das Sortieren der Liste.