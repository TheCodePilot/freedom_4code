Um den Rahmen um ein Control in einem Windows-Programm dauerhaft zu zeichnen und sicherzustellen, dass er auch nach einem WM_PAINT-Ereignis bestehen bleibt, solltest du das Zeichnen des Rahmens im OnPaint-Handler des Controls durchführen. Dies gewährleistet, dass der Rahmen bei jedem Neuzeichnen des Controls korrekt wieder angezeigt wird.

Hier ist eine angepasste Version deiner Funktion, die auf den WM_PAINT-Handler reagiert:

Überschreibe den WM_PAINT-Handler des Controls.
Zeichne den Rahmen im WM_PAINT-Handler.
cpp
Code kopieren
// Header-Datei (z.B. MainForm.h)
class TMainForm : public TForm
{
__published:
    void __fastcall ControlWndProc(TMessage &Message);

public:
    __fastcall TMainForm(TComponent* Owner);
    void __fastcall DrawFrame(TWinControl *Control, COLORREF color);
};

// Implementierungs-Datei (z.B. MainForm.cpp)
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    // Beispiel: Setze das WndProc von einem Button
    Button1->WindowProc = ControlWndProc;
}

void __fastcall TMainForm::ControlWndProc(TMessage &Message)
{
    TWinControl *Control = dynamic_cast<TWinControl*>(Sender);
    if (Control)
    {
        if (Message.Msg == WM_PAINT)
        {
            DrawFrame(Control, RGB(255, 0, 0)); // Farbe nach Wahl
        }
    }
    Control->Dispatch(&Message); // Standardverarbeitung
}

void __fastcall TMainForm::DrawFrame(TWinControl *Control, COLORREF color)
{
    HDC hdc = GetDC(Control->Handle);
    if (hdc)
    {
        RECT rect;
        GetClientRect(Control->Handle, &rect);
        HBRUSH brush = CreateSolidBrush(color);
        HPEN pen = CreatePen(PS_SOLID, 3, color); // 3 ist die Breite des Rahmens
        HGDIOBJ oldPen = SelectObject(hdc, pen);
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
        ReleaseDC(Control->Handle, hdc);
    }
}
In dieser Lösung:

Die ControlWndProc-Methode ersetzt die Standard-Nachrichtenverarbeitung des Controls und fügt das Zeichnen des Rahmens während der WM_PAINT-Nachricht hinzu.
Die DrawFrame-Methode zeichnet den Rahmen um das Control.
Im Konstruktor des TMainForm wird die WindowProc des Buttons (oder eines anderen Controls) auf die benutzerdefinierte ControlWndProc gesetzt.
Dadurch wird sichergestellt, dass der Rahmen um das Control gezeichnet wird, wann immer das Control neu gezeichnet wird. Das Control wird weiterhin seine Standardnachrichtenverarbeitung durchführen, da Control->Dispatch(&Message) aufgerufen wird.

"WindowProc" ist dies ein handle auf die Nachrichten des Controlls allgemein und hier mit Button1->WindowProc vom Button1 ?
ChatGPT
Ja, WindowProc ist ein Member des Controls (z.B. eines Buttons), das die Nachrichtenverarbeitung für dieses Control übernimmt. Durch das Setzen des WindowProc auf eine eigene Methode (in diesem Fall ControlWndProc), kannst du die Nachrichtenverarbeitung des Controls anpassen und zusätzliche Aktionen durchführen, wie das Zeichnen eines Rahmens.

Hier sind die Details dazu:

WindowProc: Diese Eigenschaft enthält die Adresse einer Methode, die alle Nachrichten des Controls verarbeitet. Standardmäßig verweist sie auf die Standard-Nachrichtenverarbeitung des Controls. Du kannst sie überschreiben, um zusätzliche Nachrichtenverarbeitung zu implementieren.

ControlWndProc: Dies ist deine benutzerdefinierte Nachrichtenverarbeitungsmethode. Sie wird aufgerufen, wenn das Control eine Nachricht empfängt. Du kannst dort zusätzliche Logik implementieren, bevor du die Nachricht an die Standard-Nachrichtenverarbeitung weiterleitest.

Hier ist die vollständige Implementierung mit Kommentaren, um dies zu verdeutlichen:

cpp
Code kopieren
// Header-Datei (z.B. MainForm.h)
class TMainForm : public TForm
{
__published:    // IDE-managed Components
    TButton *Button1; // Beispielbutton

public:        // User declarations
    __fastcall TMainForm(TComponent* Owner);
    void __fastcall DrawFrame(TWinControl *Control, COLORREF color);
    void __fastcall ControlWndProc(TMessage &Message, TWinControl* Control);
};

// Implementierungs-Datei (z.B. MainForm.cpp)
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    // Beispiel: Setze das WndProc von Button1 auf eine eigene Methode
    Button1->WindowProc = [this](TMessage &Message) { ControlWndProc(Message, Button1); };
}

void __fastcall TMainForm::ControlWndProc(TMessage &Message, TWinControl* Control)
{
    if (Message.Msg == WM_PAINT)
    {
        // Wenn eine WM_PAINT Nachricht empfangen wird, zeichne den Rahmen
        DrawFrame(Control, RGB(255, 0, 0)); // Farbe nach Wahl
    }

    // Standardnachrichtenverarbeitung des Controls aufrufen
    Control->Dispatch(&Message);
}

void __fastcall TMainForm::DrawFrame(TWinControl *Control, COLORREF color)
{
    HDC hdc = GetDC(Control->Handle);
    if (hdc)
    {
        RECT rect;
        GetClientRect(Control->Handle, &rect); // Verwende GetClientRect statt GetWindowRect
        HBRUSH brush = CreateSolidBrush(color);
        HPEN pen = CreatePen(PS_SOLID, 3, color); // 3 ist die Breite des Rahmens
        HGDIOBJ oldPen = SelectObject(hdc, pen);
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
        ReleaseDC(Control->Handle, hdc);
    }
}
In dieser Implementierung:

Setzen des WindowProc: Im Konstruktor des TMainForm wird das WindowProc-Member von Button1 auf eine Lambda-Funktion gesetzt, die ControlWndProc aufruft. Dies geschieht, um sicherzustellen, dass ControlWndProc mit dem spezifischen Control (Button1) aufgerufen wird.

ControlWndProc-Methode: Diese Methode empfängt alle Nachrichten für das Control. Wenn eine WM_PAINT-Nachricht empfangen wird, wird DrawFrame aufgerufen, um den Rahmen zu zeichnen. Danach wird die Nachricht zur Standardnachrichtenverarbeitung des Controls weitergeleitet, indem Control->Dispatch(&Message) aufgerufen wird.

Durch diese Vorgehensweise stellst du sicher, dass der Rahmen bei jeder Neuzeichnung des Controls vorhanden ist, da das Zeichnen des Rahmens im WM_PAINT-Handler erfolgt.

Kann ich das auch so machen bei mehreren Controlls? Schau: "Button1->WindowProc = ControlWndProcButton1; "Button2->WindowProc = ControlWndProcButton2;
ChatGPT
Ja, du kannst das für mehrere Controls machen, indem du für jedes Control eine eigene Nachrichtenverarbeitungsmethode erstellst. Alternativ kannst du auch eine einzige Methode verwenden, die anhand des Senders erkennt, welches Control die Nachricht gesendet hat. Hier sind beide Ansätze:

Ansatz 1: Separate Methoden für jedes Control
cpp
Code kopieren
// Header-Datei (z.B. MainForm.h)
class TMainForm : public TForm
{
__published:    // IDE-managed Components
    TButton *Button1;
    TButton *Button2;

public:        // User declarations
    __fastcall TMainForm(TComponent* Owner);
    void __fastcall DrawFrame(TWinControl *Control, COLORREF color);
    void __fastcall ControlWndProcButton1(TMessage &Message);
    void __fastcall ControlWndProcButton2(TMessage &Message);
};

// Implementierungs-Datei (z.B. MainForm.cpp)
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    // Setze das WindowProc für jedes Control
    Button1->WindowProc = [this](TMessage &Message) { ControlWndProcButton1(Message); };
    Button2->WindowProc = [this](TMessage &Message) { ControlWndProcButton2(Message); };
}

void __fastcall TMainForm::ControlWndProcButton1(TMessage &Message)
{
    if (Message.Msg == WM_PAINT)
    {
        DrawFrame(Button1, RGB(255, 0, 0)); // Farbe nach Wahl
    }
    Button1->Dispatch(&Message);
}

void __fastcall TMainForm::ControlWndProcButton2(TMessage &Message)
{
    if (Message.Msg == WM_PAINT)
    {
        DrawFrame(Button2, RGB(0, 255, 0)); // Farbe nach Wahl
    }
    Button2->Dispatch(&Message);
}

void __fastcall TMainForm::DrawFrame(TWinControl *Control, COLORREF color)
{
    HDC hdc = GetDC(Control->Handle);
    if (hdc)
    {
        RECT rect;
        GetClientRect(Control->Handle, &rect);
        HBRUSH brush = CreateSolidBrush(color);
        HPEN pen = CreatePen(PS_SOLID, 3, color); // 3 ist die Breite des Rahmens
        HGDIOBJ oldPen = SelectObject(hdc, pen);
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
        ReleaseDC(Control->Handle, hdc);
    }
}
Ansatz 2: Eine Methode für alle Controls
cpp
Code kopieren
// Header-Datei (z.B. MainForm.h)
class TMainForm : public TForm
{
__published:    // IDE-managed Components
    TButton *Button1;
    TButton *Button2;

public:        // User declarations
    __fastcall TMainForm(TComponent* Owner);
    void __fastcall DrawFrame(TWinControl *Control, COLORREF color);
    void __fastcall ControlWndProc(TMessage &Message, TWinControl* Control, COLORREF color);
};

// Implementierungs-Datei (z.B. MainForm.cpp)
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    // Setze das WindowProc für jedes Control
    Button1->WindowProc = [this](TMessage &Message) { ControlWndProc(Message, Button1, RGB(255, 0, 0)); };
    Button2->WindowProc = [this](TMessage &Message) { ControlWndProc(Message, Button2, RGB(0, 255, 0)); };
}

void __fastcall TMainForm::ControlWndProc(TMessage &Message, TWinControl* Control, COLORREF color)
{
    if (Message.Msg == WM_PAINT)
    {
        DrawFrame(Control, color);
    }
    Control->Dispatch(&Message);
}

void __fastcall TMainForm::DrawFrame(TWinControl *Control, COLORREF color)
{
    HDC hdc = GetDC(Control->Handle);
    if (hdc)
    {
        RECT rect;
        GetClientRect(Control->Handle, &rect);
        HBRUSH brush = CreateSolidBrush(color);
        HPEN pen = CreatePen(PS_SOLID, 3, color); // 3 ist die Breite des Rahmens
        HGDIOBJ oldPen = SelectObject(hdc, pen);
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
        ReleaseDC(Control->Handle, hdc);
    }
}
Erklärung:
Ansatz 1 (Separate Methoden):

Jede Schaltfläche (Button1 und Button2) hat ihre eigene ControlWndProc-Methode.
Jede Methode ruft DrawFrame mit der entsprechenden Farbe auf.
Ansatz 2 (Eine Methode für alle Controls):

Eine gemeinsame ControlWndProc-Methode wird für alle Schaltflächen verwendet.
Die Methode erhält zusätzlich das Control und die Farbe als Parameter, um den entsprechenden Rahmen zu zeichnen.
Beide Ansätze sind gültig. Der zweite Ansatz ist kompakter und vermeidet redundanten Code, indem er eine einzige Methode für alle Controls verwendet.

" Button1->WindowProc = [this](TMessage &Message) { ControlWndProc(Message, Button1, RGB(255, 0, 0)); };
    Button2->WindowProc = [this](TMessage &Message) { ControlWndProc(Message, Button2, RGB(0, 255, 0)); };" Dies wird leider vermutlich der BCC32 nicht verstehen. Ich kann bei diesem Projekt bedauerlicherweise keinen modernen Compiler nutzen.
ChatGPT
Verstehe. Wenn du keinen modernen Compiler verwenden kannst, der Lambda-Ausdrücke unterstützt, musst du eine andere Methode wählen, um die WindowProc-Eigenschaft der Steuerelemente zu ändern. Hier ist eine Lösung, die mit älteren Compilern kompatibel ist:

Erstelle eine benutzerdefinierte WindowProc-Methode für jedes Control.
Speichere die ursprüngliche WindowProc-Methode, um später darauf zurückzugreifen.
Verarbeite die Nachrichten und rufe die ursprüngliche WindowProc-Methode nach der eigenen Verarbeitung auf.
Beispielcode:
cpp
Code kopieren
// Header-Datei (z.B. MainForm.h)
class TMainForm : public TForm
{
__published:    // IDE-managed Components
    TButton *Button1;
    TButton *Button2;

public:        // User declarations
    __fastcall TMainForm(TComponent* Owner);
    void __fastcall DrawFrame(TWinControl *Control, COLORREF color);
    void __fastcall ControlWndProcButton1(TMessage &Message);
    void __fastcall ControlWndProcButton2(TMessage &Message);

private:
    TWndMethod OriginalButton1WndProc;
    TWndMethod OriginalButton2WndProc;
};

// Implementierungs-Datei (z.B. MainForm.cpp)
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    // Speichere die ursprüngliche WindowProc-Methode
    OriginalButton1WndProc = Button1->WindowProc;
    OriginalButton2WndProc = Button2->WindowProc;

    // Setze die benutzerdefinierte WindowProc-Methode
    Button1->WindowProc = ControlWndProcButton1;
    Button2->WindowProc = ControlWndProcButton2;
}

void __fastcall TMainForm::ControlWndProcButton1(TMessage &Message)
{
    if (Message.Msg == WM_PAINT)
    {
        DrawFrame(Button1, RGB(255, 0, 0)); // Farbe nach Wahl
    }
    // Rufe die ursprüngliche WindowProc-Methode auf
    OriginalButton1WndProc(Message);
}

void __fastcall TMainForm::ControlWndProcButton2(TMessage &Message)
{
    if (Message.Msg == WM_PAINT)
    {
        DrawFrame(Button2, RGB(0, 255, 0)); // Farbe nach Wahl
    }
    // Rufe die ursprüngliche WindowProc-Methode auf
    OriginalButton2WndProc(Message);
}

void __fastcall TMainForm::DrawFrame(TWinControl *Control, COLORREF color)
{
    HDC hdc = GetDC(Control->Handle);
    if (hdc)
    {
        RECT rect;
        GetClientRect(Control->Handle, &rect);
        HBRUSH brush = CreateSolidBrush(color);
        HPEN pen = CreatePen(PS_SOLID, 3, color); // 3 ist die Breite des Rahmens
        HGDIOBJ oldPen = SelectObject(hdc, pen);
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
        ReleaseDC(Control->Handle, hdc);
    }
}
Erklärung:
Speichern der ursprünglichen WindowProc-Methode:

Die ursprünglichen WindowProc-Methoden von Button1 und Button2 werden in OriginalButton1WndProc und OriginalButton2WndProc gespeichert.
Setzen der benutzerdefinierten WindowProc-Methode:

Die WindowProc-Eigenschaft von Button1 und Button2 wird auf ControlWndProcButton1 bzw. ControlWndProcButton2 gesetzt.
Nachrichtenverarbeitung in den benutzerdefinierten Methoden:

In ControlWndProcButton1 und ControlWndProcButton2 wird geprüft, ob es sich um eine WM_PAINT-Nachricht handelt. Falls ja, wird DrawFrame aufgerufen, um den Rahmen zu zeichnen.
Nach der eigenen Verarbeitung wird die ursprüngliche WindowProc-Methode aufgerufen, um sicherzustellen, dass das Control weiterhin korrekt funktioniert.
Auf diese Weise kannst du sicherstellen, dass der Rahmen korrekt gezeichnet wird, ohne dass moderne Compilerfunktionen erforderlich sind.

"Button1->WindowProc = [this](TMessage &Message) { ControlWndProc(Message, Button1, RGB(255, 0, 0)); };
    Button2->WindowProc = [this](TMessage &Message) { ControlWndProc(Message, Button2, RGB(0, 255, 0)); };" Ein Funktor würde aber akzeptiert werden. Kannst Du mir eine Klasse entsprechen erstellen ?
ChatGPT
Ja, ein Funktor ist eine geeignete Lösung. Du kannst eine Klasse erstellen, die als Funktor fungiert und die Nachrichtenverarbeitung für jedes Control übernimmt. Hier ist, wie du dies umsetzen kannst:

Definiere eine Funktorklasse, die die Nachrichtenverarbeitung übernimmt.
Setze die WindowProc-Eigenschaft jedes Controls auf eine Instanz dieser Funktorklasse.
Funktorklasse
cpp
Code kopieren
// Funktor-Klasse zur Nachrichtenverarbeitung
class TControlWndProc
{
public:
    TControlWndProc(TWinControl* Control, COLORREF Color, TWndMethod OriginalProc)
        : Control(Control), Color(Color), OriginalProc(OriginalProc) {}

    void __fastcall operator()(TMessage &Message)
    {
        if (Message.Msg == WM_PAINT)
        {
            DrawFrame(Control, Color);
        }
        // Rufe die ursprüngliche WindowProc-Methode auf
        OriginalProc(Message);
    }

private:
    TWinControl* Control;
    COLORREF Color;
    TWndMethod OriginalProc;

    void __fastcall DrawFrame(TWinControl *Control, COLORREF color)
    {
        HDC hdc = GetDC(Control->Handle);
        if (hdc)
        {
            RECT rect;
            GetClientRect(Control->Handle, &rect);
            HBRUSH brush = CreateSolidBrush(color);
            HPEN pen = CreatePen(PS_SOLID, 3, color); // 3 ist die Breite des Rahmens
            HGDIOBJ oldPen = SelectObject(hdc, pen);
            HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
            SelectObject(hdc, oldBrush);
            SelectObject(hdc, oldPen);
            DeleteObject(brush);
            DeleteObject(pen);
            ReleaseDC(Control->Handle, hdc);
        }
    }
};
Verwendung der Funktorklasse
cpp
Code kopieren
// Header-Datei (z.B. MainForm.h)
class TMainForm : public TForm
{
__published:    // IDE-managed Components
    TButton *Button1;
    TButton *Button2;

public:        // User declarations
    __fastcall TMainForm(TComponent* Owner);

private:
    TControlWndProc* Button1WndProc;
    TControlWndProc* Button2WndProc;
};

// Implementierungs-Datei (z.B. MainForm.cpp)
__fastcall TMainForm::TMainForm(TComponent* Owner)
    : TForm(Owner)
{
    // Speichere die ursprüngliche WindowProc-Methode
    TWndMethod OriginalButton1WndProc = Button1->WindowProc;
    TWndMethod OriginalButton2WndProc = Button2->WindowProc;

    // Erstelle und setze die benutzerdefinierte WindowProc-Methode für Button1
    Button1WndProc = new TControlWndProc(Button1, RGB(255, 0, 0), OriginalButton1WndProc);
    Button1->WindowProc = Button1WndProc;

    // Erstelle und setze die benutzerdefinierte WindowProc-Methode für Button2
    Button2WndProc = new TControlWndProc(Button2, RGB(0, 255, 0), OriginalButton2WndProc);
    Button2->WindowProc = Button2WndProc;
}

// Vergiss nicht, die Funktoren freizugeben, wenn sie nicht mehr gebraucht werden
__fastcall TMainForm::~TMainForm()
{
    delete Button1WndProc;
    delete Button2WndProc;
}
Erklärung:
Funktorklasse TControlWndProc:

Diese Klasse speichert das Control, die Farbe und die ursprüngliche WindowProc-Methode.
Der Funktor (operator ()) übernimmt die Nachrichtenverarbeitung. Wenn eine WM_PAINT-Nachricht empfangen wird, zeichnet er den Rahmen und ruft anschließend die ursprüngliche WindowProc-Methode auf.
Verwendung im Hauptformular (TMainForm):

Im Konstruktor des Hauptformulars (TMainForm) wird die ursprüngliche WindowProc-Methode für jedes Control gespeichert.
Eine Instanz von TControlWndProc wird erstellt und auf die WindowProc-Eigenschaft des jeweiligen Controls gesetzt.
Destruktor des Hauptformulars:

Die Funktorinstanzen werden im Destruktor des Hauptformulars freigegeben, um Speicherlecks zu vermeiden.
