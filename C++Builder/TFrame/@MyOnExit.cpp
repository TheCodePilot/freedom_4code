1. Der Frame verliert den Fokus (z. B. Benutzer klickt auf ein anderes Steuerelement):
Es gibt kein direktes „OnExit“-Ereignis für TFrame selbst, aber du kannst das OnExit-Ereignis eines enthaltenen Steuerelements (z. B. eines TEdit) verwenden, oder ein eigenes „Fokus-Verlassen“ durch Zusammensetzen steuern.

2. Der Benutzer wechselt das Formular oder schließt das Elternformular:
Wenn du darauf reagieren willst, wenn das Frame nicht mehr sichtbar ist, gibt es zwei Möglichkeiten:

Override der Methode CMVisibleChanged:

cpp
Kopieren
Bearbeiten
void __fastcall TMyFrame::CMVisibleChanged(TMessage &Message)
{
    TFrame::Dispatch(&Message);
    if (!Visible)
    {
        // Frame wurde "verlassen" oder unsichtbar
    }
}
BEGIN_MESSAGE_MAP
    MESSAGE_HANDLER(CM_VISIBLECHANGED, TMessage, CMVisibleChanged)
END_MESSAGE_MAP(TFrame)
3. Der Frame wird aus dem Sichtbereich entfernt oder deaktiviert:
Du könntest folgende Ereignisse oder Methoden überladen:

OnExit: Gilt nur für Steuerelemente, nicht direkt für TFrame.

DoExit(): In einem abgeleiteten Frame könntest du diese Methode überladen, sie wird aber nur aufgerufen, wenn der Frame selbst den Fokus verliert (was selten passiert, da Container meist keinen Fokus erhalten).

OnDeactivate oder OnHide: Diese existieren nicht direkt für TFrame, aber du kannst in einem abgeleiteten Frame auf OnVisibleChanged oder Parent->OnDeactivate reagieren (sofern verfügbar).

Empfehlung:
Wenn du wissen willst, wann der Benutzer den Frame „verlässt“, implementiere in deinem abgeleiteten TFrame eine Methode, die prüft, wann der Fokus auf ein Steuerelement außerhalb des Frames wechselt:

Beispiel (Fokus-Wechsel erkennen):
cpp
Kopieren
Bearbeiten
void __fastcall TMyFrame::AppOnMessage(tagMSG &Msg, bool &Handled)
{
    if (Msg.message == WM_SETFOCUS)
    {
        HWND hNewFocus = (HWND)Msg.wParam;
        if (!IsChild(Handle, hNewFocus))
        {
            // Fokus liegt nun außerhalb des Frames
            OnFrameExit();  // eigene Methode aufrufen
        }
    }
}
Und das registrierst du z. B. in deinem Formular oder Frame:

cpp
Kopieren
Bearbeiten
Application->OnMessage = AppOnMessage;
Zusammenfassung:
Situation	Mögliche Reaktion
Benutzer verlässt Steuerelement	OnExit pro Steuerelement
Frame verliert Fokus	Manuell über Application->OnMessage
Frame wird unsichtbar	CM_VISIBLECHANGED über MESSAGE_HANDLER
Frame wird entladen / zerstört	Destruktor ~TMyFrame()

Wenn du dein konkretes Ziel näher beschreibst (z. B. „ich will validieren, wenn der Nutzer von einem Frame zum nächsten wechselt“), kann ich dir die genau passende Methode vorschlagen.

//====================================Einschub von mir====================================

Sehr gute Frage.

Nein, du musst dich in Application->OnMessage nicht standardmäßig um andere Nachrichten kümmern, wenn du Handled = true; nicht setzt.

Erklärung:
Der Parameter Handled gibt an, ob die Nachricht bereits vollständig verarbeitet wurde. Wenn du Handled = true; setzt, wird die normale Standardverarbeitung dieser Nachricht unterdrückt – das willst du nur dann machen, wenn du die Nachricht komplett selbst behandelst (was in deinem Fall nicht nötig ist).

In deinem Beispiel:
cpp
Kopieren
Bearbeiten
void __fastcall TMyFrame::AppOnMessage(tagMSG &Msg, bool &Handled)
{
    if (Msg.message == WM_SETFOCUS)
    {
        HWND hNewFocus = (HWND)Msg.wParam;
        if (!IsChild(Handle, hNewFocus))
        {
            OnFrameExit();  // eigene Methode
        }
        // Nachricht wurde nur beobachtet, nicht verbraucht:
        // Daher: Handled = false; oder einfach nichts tun
    }
}
Solange du Handled nicht auf true setzt, wird die Nachricht ganz normal weiterverarbeitet, und du musst dich nicht um andere Nachrichten kümmern oder sie weiterleiten.

Wann wäre Handled = true; nötig?
Nur wenn du z. B. die Standardverarbeitung verhindern willst, wie bei Tastendrücken (z. B. VK_RETURN unterdrücken) oder das Weiterreichen an andere Controls.

Fazit:
In deinem Beispiel:

Nein, du musst keine anderen Nachrichten behandeln.

Ja, du kannst dich auf WM_SETFOCUS beschränken.

Lass Handled einfach auf false, und alles andere wird wie gewohnt verarbeitet.







Du:
"Application->OnMessage = AppOnMessage; " Was genau geschieht hier ?


ChatGPT:
Die Zeile:

cpp
Kopieren
Bearbeiten
Application->OnMessage = AppOnMessage;
weist der globalen Ereignisbehandlung von Nachrichten auf Anwendungsebene eine eigene Callback-Funktion zu.

Was genau passiert:
Application ist ein globales Objekt vom Typ TApplication, das für das gesamte VCL-Programm zentral ist – es verarbeitet z. B. Nachrichten, zeigt Exceptions an usw.

OnMessage ist ein Ereignis (Event-Handler) des Typs:

cpp
Kopieren
Bearbeiten
TMessageEvent = void __fastcall (__closure *TMessageEvent)(tagMSG &Msg, bool &Handled);
Das heißt: Es erwartet eine Funktion, die jede Windows-Nachricht abfangen kann, bevor sie an das eigentliche Ziel (z. B. ein Steuerelement) weitergegeben wird.

AppOnMessage ist eine Methode (z. B. in deinem Formular oder Frame), die diesem Signaturtyp entspricht. Sobald eine Windows-Nachricht verarbeitet wird, ruft die VCL deine Methode auf.

Beispielhafte Wirkung:
Wenn du dies in deinem Formular machst:

cpp
Kopieren
Bearbeiten
Application->OnMessage = AppOnMessage;
Dann bekommst du in AppOnMessage jede einzelne Windows-Nachricht, bevor sie intern an die Zielkomponente gesendet wird – z. B.:

Tastatureingaben (z. B. WM_KEYDOWN)

Mausklicks (WM_LBUTTONDOWN)

Fokuswechsel (WM_SETFOCUS, WM_KILLFOCUS)

u.v.m.

Wichtiger Hinweis:
Die Ereignisbehandlung durch OnMessage ist global für die ganze Anwendung. Das bedeutet:

Es gibt immer nur eine OnMessage-Methode gleichzeitig.

Wenn du Application->OnMessage = ... mehrmals setzt, überschreibst du frühere Zuweisungen.

Du solltest vorherigen Handler sichern, wenn du später zurücksetzen willst:

cpp
Kopieren
Bearbeiten
TMessageEvent OldHandler = Application->OnMessage;
Application->OnMessage = MyHandler;
...
Application->OnMessage = OldHandler;  // zurücksetzen
Zusammenfassung:
Teil	Bedeutung
Application	Globale Anwendungskomponente
OnMessage	Ereignis für globale Nachrichtenverarbeitung
AppOnMessage	Eigene Methode, die Nachrichten verarbeitet
Zweck	Nachrichten beobachten oder eingreifen, z. B. Fokus, Tasten etc.

Wenn du z. B. global bestimmte Tastenkombinationen abfangen oder Fokusänderungen zwischen Steuerelementen kontrollieren willst, ist Application->OnMessage genau der richtige Ort dafür.

//====================================Ende Einschub von mir====================================

Genaue Lösung:

Danke, jetzt ist dein Ziel ganz klar:
Sobald der Benutzer ein Steuerelement außerhalb deines TFrame (also z. B. auf dem TForm) fokussiert, willst du MySave() aufrufen.

Das heißt: Du brauchst eine Methode, die erkennt, wenn der Fokus den Frame verlässt.

🧩 Lösung (C++03 / VCL):
Wir verwenden Application->OnMessage, um WM_SETFOCUS-Nachrichten global abzufangen und zu prüfen, ob der neue Fokus außerhalb des Frames liegt.

✅ Schritt-für-Schritt:
1. Eigene Methode zur Prüfung:
cpp
Kopieren
Bearbeiten
void __fastcall TMyFrame::AppOnMessage(tagMSG &Msg, bool &Handled)
{
    if (Msg.message == WM_SETFOCUS)
    {
        HWND hNewFocus = (HWND)Msg.wParam;

        // Prüfen: Ist der neue Fokus außerhalb dieses Frames?
        if (!::IsChild(Handle, hNewFocus))
        {
            // Fokus hat Frame verlassen
            MySave();  // Zwischenspeichern aufrufen
        }
    }
}
2. Ereignis registrieren:
Am besten z. B. im OnCreate des Elternformulars oder beim Initialisieren des Frames:

cpp
Kopieren
Bearbeiten
Application->OnMessage = AppOnMessage;
💡 Optional: Vorherigen Handler sichern und später wiederherstellen (wenn du aufräumen willst):

cpp
Kopieren
Bearbeiten
static TMessageEvent OldAppMessageHandler;

void __fastcall TForm1::FormCreate(TObject *Sender)
{
    OldAppMessageHandler = Application->OnMessage;
    Application->OnMessage = AppOnMessage;
}

void __fastcall TForm1::FormDestroy(TObject *Sender)
{
    Application->OnMessage = OldAppMessageHandler;
}
🔒 Wichtige Hinweise:
Falls du mehrere Frames oder Komponenten hast, die ähnliches Verhalten brauchen, solltest du prüfen, welcher Frame gerade aktiv ist, und eventuell eine zentrale Verwaltung machen.

Wenn MySave() nicht mehrfach aufgerufen werden soll, speichere einen Zustand z. B. mit einem bool isSaved.

🧪 Extra: Alternative über WM_KILLFOCUS
Du könntest auch über den Frame direkt WM_KILLFOCUS behandeln, aber nur, wenn dein Frame selbst fokussiert ist – was in der Praxis bei Containern selten der Fall ist. Die OnMessage-Variante ist zuverlässiger.

✅ Fazit:
Für dein Ziel – beim Verlassen des Frames durch Fokuswechsel auf ein Steuerelement des Formulars automatisch MySave() aufzurufen – ist Application->OnMessage mit WM_SETFOCUS die richtige, saubere Lösung in C++03/VCL.

