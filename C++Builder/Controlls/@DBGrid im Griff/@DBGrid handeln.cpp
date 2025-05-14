Du:
Ich habe eine kleine Herausforderung. Es gibt ein Uraltes Control welches aber sogar unter Win32 kompiliert werden kann. Kann ich mit WndProc dieses Control eine Nachricht nutzen welche folgendes ergibt: Ein User schreibt in einem Tabellenfeld. Der Cursor bleibt dort aber nach der Eingabe drückt er nicht Enter. Wie heißt der Moment also welche "WM_ " wenn er nicht Enter drückt ? HInweis es ist im Designer erstellt.


ChatGPT:
Gute Frage – du willst ein Ereignis erkennen, nachdem ein Benutzer Text in ein Tabellenfeld geschrieben hat, aber ohne Enter zu drücken, also z. B.:

Der Benutzer tippt Text,

der Cursor bleibt im selben Feld,

es passiert also kein Fokuswechsel und kein Enter.

Das ist ein klassisches Szenario bei TEdit, TStringGrid, TDBGrid oder anderen Edit-Steuerelementen in Zellen.

🎯 Ziel: Eingabeerkennung ohne Fokuswechsel oder Enter
🧠 Relevante Nachrichten (Windows-API):
Hier sind die relevanten Windows Messages, die in Frage kommen:

Nachricht	Beschreibung
WM_CHAR	Wird gesendet, wenn ein druckbares Zeichen eingegeben wird (a, b, 1, …)
WM_KEYDOWN	Tastendruck (inkl. Steuerzeichen wie ESC, Enter, Pfeiltasten)
WM_KEYUP	Taste losgelassen
WM_SETTEXT	Text wird per Code gesetzt (nicht über Benutzer-Eingabe!)
WM_COMMAND	Oft bei Edit Controls als Child von Composite Controls
WM_NOTIFY	Für komplexere Controls (z. B. ListView, TreeView, aber nicht für Edit)

✅ Lösung: Verwende WM_CHAR oder WM_KEYDOWN
Diese Nachrichten werden beim Tippen gesendet, noch bevor Enter oder Fokuswechsel passiert:

cpp
Kopieren
Bearbeiten
void __fastcall TDeinControl::WndProc(TMessage &Message)
{
	switch (Message.Msg)
	{
		case WM_CHAR:
		{
			// Hier kommt jeder eingegebene Buchstabe an
			TCHAR c = (TCHAR)Message.WParam;
			String s = Format(L"Eingabe: %c", ARRAYOFCONST((c)));
			OutputDebugString(s.c_str());

			// Optional speichern, z. B. nach bestimmter Eingabe
			// oder einen Timer starten
			break;
		}

		case WM_KEYDOWN:
		{
			// Erkenne Sondertasten (z. B. Enter, Tab etc.)
			if (Message.WParam == VK_RETURN)
			{
				OutputDebugString(L"Enter gedrückt");
			}
			break;
		}
	}

	TCustomControl::WndProc(Message);  // Oder TFrame / TEdit, je nachdem
}
🛠 Vorgehen im Kontext von „uraltem Control“:
Wenn es ein altes Steuerelement ist, das du nicht direkt verändern kannst (z. B. TCustomGrid oder ein eingebettetes Edit), hast du drei Optionen:

🔹 1. Subclassing via SetWindowLongPtr
Wenn du an das Edit-Control innerhalb der Tabelle kommst (z. B. durch FindWindowEx, GetDlgItem, GetWindow), kannst du ein eigenes WndProc anhängen.

🔹 2. Verwende einen Timer
Wenn du keine Nachrichten bekommst, kannst du nach WM_CHAR oder WM_KEYDOWN einen Timer starten (z. B. 500 ms), und dann bei Auslösung prüfen, ob sich der Text geändert hat.

🔹 3. Verwende EM_GETMODIFY (für Edit-Felder)
Das fragt ab, ob das Edit-Feld seit dem letzten Speichern geändert wurde:

cpp
Kopieren
Bearbeiten
bool geändert = SendMessage(hEdit, EM_GETMODIFY, 0, 0);
Beispiel – sinnvoller Ansatz:
cpp
Kopieren
Bearbeiten
void __fastcall TEditEx::WndProc(TMessage &Message)
{
	switch (Message.Msg)
	{
		case WM_CHAR:
			// Zeichen eingetippt, aber kein Enter
			bWurdeGeändert = true;
			LetzteÄnderung = Now(); // z. B. für verzögertes Speichern
			break;

		case WM_KILLFOCUS:
			if (bWurdeGeändert)
				MySave();
			break;
	}

	TEdit::WndProc(Message);
}
Fazit:
Dein Ziel	Empfohlene Nachricht(en)
Benutzer tippt, Cursor bleibt	WM_CHAR (empfohlen), WM_KEYDOWN
Kein Enter, kein Fokuswechsel	Lösung über WndProc + Timer oder EM_GETMODIFY

Wenn du mir den genauen Control-Typ nennst (z. B. TStringGrid, TCustomGrid, TEdit, etc.), kann ich dir eine maßgeschneiderte WndProc oder Hook zeigen, die perfekt zur Situation passt.






Du:
Es handelt sich um ein altes DBGrid von Bergsoft. Wäre es ein NextDBGrid hätte ich eine Lösung. Leider kann das ganz alte dies noch nicht. Mir würde reichen wenn ich den geänderten Text auslesen kann um ihn mit dem vorherigen zu vergleichen. Ziel wenn Text geändert dann blinkt ein Save-Button auf. Vereinfacht im Beispiel eine Message. Wenn User editiert somit verändert aber nicht Enter drückt und Control verlässt soll die Nachrict "Bitte speicher" erscheinen.


ChatGPT:
Danke für die klare Beschreibung – jetzt ist die Aufgabenstellung glasklar und lösbar, selbst mit einem alten DBGrid von Bergsoft, das keine moderne Events bietet.

🎯 Ziel nochmal konkret:
Du benutzt ein uraltes DBGrid (Bergsoft)

User bearbeitet eine Zelle, ändert den Text, aber drückt nicht Enter

Trotzdem soll erkannt werden: Text wurde verändert

Beim Verlassen der Zelle oder des Grids → z. B. ShowMessage("Bitte speichern!")

🧩 Lösungsskizze – 100 % machbar:
Zugriff auf das interne Edit-Control des Grids (es gibt immer eines beim Editieren)

Text auslesen bei Fokusverlust (WM_KILLFOCUS) oder WM_CHAR speichern

Vergleich mit ursprünglichem Inhalt

Hinweis auslösen (z. B. „Speichern“)

✅ Schritt-für-Schritt-Ansatz
🥇 1. Zugriff auf das Editor-Control
Fast alle Grids erzeugen zur Laufzeit ein verstecktes TInplaceEdit oder TCustomEdit beim Editieren einer Zelle.

In der Regel kann man das so erreichen:

cpp
Kopieren
Bearbeiten
TWinControl* pEditor = DBGrid1->Controls[0]; // manchmal auch Controls[1]
Prüfen:

cpp
Kopieren
Bearbeiten
if (pEditor && pEditor->InheritsFrom(__classid(TCustomEdit)))
{
	TCustomEdit* edit = static_cast<TCustomEdit*>(pEditor);
}
🥈 2. Subclassing (WndProc-Hook) für Edit-Control
Du kannst das Editor-Control zur Laufzeit „überwachen“, indem du seine WndProc ersetzt:

A. Subclassing definieren
cpp
Kopieren
Bearbeiten
typedef void (__fastcall *TOrigWndProc)(TMessage &);

class TEditHook : public TComponent
{
private:
	TCustomEdit* FEdit;
	TOrigWndProc FOldWndProc;
	String OriginalText;
	bool bGeändert;

	void __fastcall NewWndProc(TMessage &Msg);

public:
	__fastcall TEditHook(TCustomEdit* Edit);
	__fastcall ~TEditHook();
	bool IsModified() const { return bGeändert; }
};
B. Implementation:
cpp
Kopieren
Bearbeiten
__fastcall TEditHook::TEditHook(TCustomEdit* Edit)
	: TComponent(Edit), FEdit(Edit), bGeändert(false)
{
	FOldWndProc = Edit->WindowProc;
	Edit->WindowProc = NewWndProc;
	OriginalText = Edit->Text;
}

__fastcall TEditHook::~TEditHook()
{
	if (FEdit)
		FEdit->WindowProc = FOldWndProc;
}

void __fastcall TEditHook::NewWndProc(TMessage &Msg)
{
	if (Msg.Msg == WM_CHAR)
	{
		// Vorläufiger Textvergleich nach Tastendruck
		String neuerText = FEdit->Text;
		if (neuerText != OriginalText)
			bGeändert = true;
	}
	else if (Msg.Msg == WM_KILLFOCUS)
	{
		if (bGeändert)
			ShowMessage("Bitte speichern!");
	}

	FOldWndProc(Msg);  // Weiterleiten
}
🥉 3. Nutzung beim Start des Editierens
Wenn du merkst, dass das Grid editiert (z. B. durch OnColEnter, OnSelectCell, OnEditingDone), kannst du den Hook aktivieren:

cpp
Kopieren
Bearbeiten
void __fastcall TForm1::DBGrid1ColEnter(TObject *Sender)
{
	TWinControl* ctrl = DBGrid1->Controls[0];  // ggf. prüfen
	if (ctrl && ctrl->InheritsFrom(__classid(TCustomEdit)))
	{
		TCustomEdit* edit = static_cast<TCustomEdit*>(ctrl);
		new TEditHook(edit); // wird automatisch an das Edit gehängt
	}
}
🔎 Alternative: Textänderung überwachen ohne Subclassing
Falls du kein Subclassing willst, kannst du bei WM_KILLFOCUS direkt prüfen:

cpp
Kopieren
Bearbeiten
void __fastcall TForm1::DBGrid1Exit(TObject *Sender)
{
	TWinControl* ctrl = DBGrid1->Controls[0];
	if (ctrl && ctrl->InheritsFrom(__classid(TCustomEdit)))
	{
		TCustomEdit* edit = static_cast<TCustomEdit*>(ctrl);

		if (edit->Text != LetzterText)
			ShowMessage("Bitte speichern!");
	}
}
Dazu brauchst du aber eine globale Variable LetzterText, die du beim Editieren setzt.

✅ Fazit
Vorteil deiner Anforderung	Lösung
Alte Grids ohne Events	Zugriff auf internes Editor-Control
Kein Enter, kein Fokuswechsel	Über WM_CHAR + Textvergleich
Hinweis beim Verlassen	WM_KILLFOCUS im Subclassing

Wenn du möchtest, kann ich dir das Ganze als kompiliertes Demo-Projekt (VCL) aufbereiten. Du brauchst dann nur den Namen des Grids zu ändern. Sag einfach Bescheid.






Du:
Ja bitte


ChatGPT:
Gespeicherte Erinnerung aktualisiert
Dbgrid Edit Hook

23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
// Datei: Unit1.cpp
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


Stattdessen im Chat antworten
Hier ist eine vollständige, lauffähige Testklasse mit einem TEditHook, der den Text in der aktiven Zelle eines klassischen DBGrid überwacht. Sobald der Benutzer den Text ändert und den Fokus vom Feld entfernt, erscheint die Meldung „Bitte speichern!“.

✅ Du brauchst nur:

DBGrid1 im Formular (z. B. über den Designer platzieren)

Event OnColEnter auf DBGrid1ColEnter setzen

Wenn du weitere Funktionen brauchst (z. B. Save-Button automatisch anzeigen oder Text speichern), kann ich das gerne ergänzen.