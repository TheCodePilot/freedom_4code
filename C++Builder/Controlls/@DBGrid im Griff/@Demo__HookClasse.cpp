// Datei: Unit1.cpp
#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

// --- Subclass-Komponente ---
class TEditHook : public TComponent
{
private:
    TCustomEdit* FEdit;
    TWndMethod FOldWndProc;
    String OriginalText;
    bool bGeaendert;

    void __fastcall NewWndProc(TMessage &Msg)
    {
        if (Msg.Msg == WM_CHAR)
        {
            if (FEdit->Text != OriginalText)
                bGeaendert = true;
        }
        else if (Msg.Msg == WM_KILLFOCUS)
        {
            if (bGeaendert)
                ShowMessage("Bitte speichern!");
        }

        FOldWndProc(Msg); // weiterleiten
    }

public:
    __fastcall TEditHook(TCustomEdit* Edit) : TComponent(Edit), FEdit(Edit), bGeaendert(false)
    {
        FOldWndProc = Edit->WindowProc;
        Edit->WindowProc = NewWndProc;
        OriginalText = Edit->Text;
    }

    __fastcall ~TEditHook()
    {
        if (FEdit)
            FEdit->WindowProc = FOldWndProc;
    }
};

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}

void __fastcall TForm1::DBGrid1ColEnter(TObject *Sender)
{
    if (DBGrid1->ControlsCount > 0)
    {
        TWinControl* ctrl = DBGrid1->Controls[0];
        if (ctrl && ctrl->InheritsFrom(__classid(TCustomEdit)))
        {
            TCustomEdit* edit = static_cast<TCustomEdit*>(ctrl);
            new TEditHook(edit); // Hook setzt sich automatisch
        }
    }
}
