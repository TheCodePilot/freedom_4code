class TCustomStaticText : public TStaticText
{
protected:
    void __fastcall WndProc(TMessage &Message);

private:
    COLORREF textColor;
    bool customDrawing;

public:
    __fastcall TCustomStaticText(TComponent* Owner)
        : TStaticText(Owner)
    {
        customDrawing = true;
        textColor = RGB(0, 0, 255); // Standard: Blau
    }

    void __fastcall MyCustomPaint()
    {
        SendMessage(this->Handle, WM_PAINT, 0, 0);
    }

    void __fastcall SetTextColor(COLORREF color)
    {
        textColor = color;
    }

    void __fastcall EnableCustomDrawing(bool enable)
    {
        customDrawing = enable;
    }
};

void __fastcall TCustomStaticText::WndProc(TMessage &Message)
{
    if (Message.Msg == WM_PAINT && customDrawing)
    {
        HDC hDC = ::GetDC(this->Handle);
        if (hDC != NULL)
        {
            RECT rect;
            ::GetClientRect(this->Handle, &rect);

            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(hDC, textColor);

            DrawText(hDC, this->Caption.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            ::ReleaseDC(this->Handle, hDC);
        }
        return; // Verhindert die Standardbehandlung
    }

    TStaticText::WndProc(Message);
}

//--------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    // Erstellen einer Instanz von TCustomStaticText
    TCustomStaticText *customStaticText = new TCustomStaticText(this);

    // Festlegen des Elternteils
    customStaticText->Parent = this;

    // Platzierung und Größenanpassung
    customStaticText->Left = 50;
    customStaticText->Top = 50;
    customStaticText->Width = 200;
    customStaticText->Height = 50;

    // Text-Eigenschaften setzen
    customStaticText->SetTextColor(RGB(255, 0, 0)); // Rot
    customStaticText->Caption = "Hallo, Rudolf!";
    customStaticText->Font->Style = TFontStyles() << fsBold;

    // Custom Drawing aktivieren
    customStaticText->EnableCustomDrawing(true);

    // Optional: Manuelles Neuzeichnen des Controls
    customStaticText->MyCustomPaint();
}


//========================

class CustomStaticText : public TStaticText
{
protected:
    void __fastcall WndProc(TMessage &Message);

private:
    COLORREF textColor;
    bool customDrawing;

public:
	__fastcall CustomStaticText(TComponent* Owner)
        : TStaticText(Owner)
    {
        customDrawing = true;
        textColor = RGB(0, 0, 255); // Standard: Blau
    }

    void __fastcall MyCustomPaint()
    {
        SendMessage(this->Handle, WM_PAINT, 0, 0);
    }

    void __fastcall SetTextColor(COLORREF color)
    {
        textColor = color;
    }

    void __fastcall EnableCustomDrawing(bool enable)
    {
        customDrawing = enable;
    }
};

void __fastcall CustomStaticText::WndProc(TMessage &Message)
{
    if (Message.Msg == WM_PAINT && customDrawing)
    {
        HDC hDC = ::GetDC(this->Handle);
        if (hDC != NULL)
        {
            RECT rect;
            ::GetClientRect(this->Handle, &rect);

            SetBkMode(hDC, TRANSPARENT);
            SetTextColor(textColor);

            DrawText(hDC, this->Caption.c_str(), -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            ::ReleaseDC(this->Handle, hDC);
        }
        return; // Verhindert die Standardbehandlung
    }

    TStaticText::WndProc(Message);
}


class CustomStaticText;

class TForm5 : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
private:	// Benutzer-Deklarationen
	void __fastcall MyFormCreate();
    CustomStaticText *customStaticText;

public:		// Benutzer-Deklarationen
	__fastcall TForm5(TComponent* Owner);
};


//-------------------------------------

void __fastcall TForm5::MyFormCreate()
{
    // Erstellen einer Instanz von TCustomStaticText
    customStaticText = new CustomStaticText(this);

    // Festlegen des Elternteils
    customStaticText->Parent = this;

    // Platzierung und Größenanpassung
    customStaticText->Left = 50;
    customStaticText->Top = 50;
    customStaticText->Width = 200;
    customStaticText->Height = 50;

    // Text-Eigenschaften setzen
    customStaticText->SetTextColor(RGB(255, 0, 0)); // Rot
    customStaticText->Caption = "Hallo, Rudolf!";
    customStaticText->Font->Style = TFontStyles() << fsBold;

    // Custom Drawing aktivieren
    customStaticText->EnableCustomDrawing(true);

    // Optional: Manuelles Neuzeichnen des Controls
    customStaticText->MyCustomPaint();
}





