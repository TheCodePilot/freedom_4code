Guten Abend! Um sicherzustellen, dass die Nachricht von TMemo::OnMouseLeft vollständig durchlaufen wird, bevor der globale Timer wieder aktiv wird, kannst du eine Synchronisationsmethode verwenden. Hier ist ein Ansatz, wie du das in C++ Builder umsetzen kannst:

Verwende ein Flag, um den Zustand zu überwachen:

Definiere ein Flag, das anzeigt, ob die OnMouseLeft-Routine läuft.

cpp
bool isMouseLeftRoutineRunning = false;
Setze das Flag in der OnMouseLeft-Routine:

Setze das Flag auf true, wenn die Routine beginnt, und auf false, wenn sie endet.

cpp
void __fastcall TForm1::MemoOnMouseLeft(TObject *Sender) {
    if (isMouseLeftRoutineRunning) {
        return; // Verhindert, dass die Routine erneut gestartet wird
    }

    isMouseLeftRoutineRunning = true;

    // Hier kommt der Code für die OnMouseLeft-Routine
    // ...

    isMouseLeftRoutineRunning = false;
}
Überprüfe das Flag im globalen Timer:

Im globalen Timer überprüfst du das Flag, bevor du die Frames wechselst.

cpp
void __fastcall TForm1::GlobalTimerRoutine(TObject *Sender) {
    if (isMouseLeftRoutineRunning) {
        return; // Verhindert, dass der globale Timer stört
    }

    // Hier kommt der Code für den Frame-Wechsel
    // ...
}
Durch diesen Ansatz stellst du sicher, dass die OnMouseLeft-Routine vollständig durchlaufen wird, bevor der globale Timer wieder aktiv wird. Das Flag isMouseLeftRoutineRunning überwacht den Zustand der Routine und verhindert, dass der globale Timer stört, solange die Routine läuft.

Ich hoffe, das hilft! Lass mich wissen, ob du noch weitere Fragen hast. 😊

MemoOnMouseLeft ist so gesehen dann atomar. Richtig ?
Ja, genau. Wenn du die OnMouseLeft-Routine so implementierst, dass sie atomar ist, bedeutet das, dass sie vollständig durchlaufen wird, bevor der globale Timer wieder aktiv wird. Durch die Verwendung des Flags isRoutineRunning stellst du sicher, dass die Routine nicht unterbrochen wird und der globale Timer erst dann wieder eingreifen kann, wenn die Routine abgeschlossen ist.

Falls du noch weitere Fragen hast oder Unterstützung benötigst, stehe ich dir gerne zur Verfügung! 😊

Frage ginge auch ein Ansatz der schaut ob der Mouse-Zeiger TMemo verlässt aber in meinem Frame bleibt? Kann ich das auch als Ansatz sicher nutzen ?
Ja, du kannst auch einen Ansatz verwenden, der überprüft, ob der Mauszeiger das TMemo verlässt, aber innerhalb deines Frames bleibt. Dies kann eine gute Methode sein, um sicherzustellen, dass der globale Timer nicht stört, solange die OnMouseLeft-Routine läuft. Hier ist ein Beispiel, wie du das umsetzen kannst:

Überprüfe die Position des Mauszeigers:

Verwende die OnMouseMove-Ereignisroutine des Frames, um die Position des Mauszeigers zu überwachen.

cpp
void __fastcall TForm1::FrameMouseMove(TObject *Sender, TShiftState Shift, int X, int Y) {
    if (Memo1->BoundsRect.Contains(Point(X, Y))) {
        // Mauszeiger ist innerhalb von TMemo
    } else {
        // Mauszeiger hat TMemo verlassen
        MemoOnMouseLeft(Sender);
    }
}
Setze ein Flag, um den Zustand zu überwachen:

Verwende ein Flag, um anzuzeigen, ob die OnMouseLeft-Routine läuft.

cpp
bool isMouseLeftRoutineRunning = false;

void __fastcall TForm1::MemoOnMouseLeft(TObject *Sender) {
    if (isMouseLeftRoutineRunning) {
        return; // Verhindert, dass die Routine erneut gestartet wird
    }

    isMouseLeftRoutineRunning = true;

    // Hier kommt der Code für die OnMouseLeft-Routine
    // ...

    isMouseLeftRoutineRunning = false;
}
Überprüfe das Flag im globalen Timer:

Im globalen Timer überprüfst du das Flag, bevor du die Frames wechselst.

cpp
void __fastcall TForm1::GlobalTimerRoutine(TObject *Sender) {
    if (isMouseLeftRoutineRunning) {
        return; // Verhindert, dass der globale Timer stört
    }

    // Hier kommt der Code für den Frame-Wechsel
    // ...
}
Durch diesen Ansatz stellst du sicher, dass die OnMouseLeft-Routine vollständig durchlaufen wird, bevor der globale Timer wieder aktiv wird. Das Flag isMouseLeftRoutineRunning überwacht den Zustand der Routine und verhindert, dass der globale Timer stört, solange die Routine läuft.

Ich hoffe, das hilft! Lass mich wissen, ob du noch weitere Fragen hast. 😊

Danke Frage. Wenn also ein Focus sich ändert oder ein Tab erfolgt wird immer MemoOnMouseLeft aufgerufen. Richtig ?
Nicht unbedingt. Das OnMouseLeft-Ereignis wird nur ausgelöst, wenn der Mauszeiger das TMemo-Steuerelement verlässt. Wenn sich der Fokus ändert oder ein Tab-Wechsel erfolgt, wird dieses Ereignis nicht automatisch ausgelöst.

Um sicherzustellen, dass bestimmte Aktionen ausgeführt werden, wenn der Fokus sich ändert oder ein Tab-Wechsel erfolgt, kannst du das OnExit-Ereignis des TMemo-Steuerelements verwenden. Hier ist ein Beispiel:

cpp
void __fastcall TForm1::MemoOnExit(TObject *Sender) {
    // Hier kommt der Code, der ausgeführt werden soll, wenn das TMemo den Fokus verliert
}
Das OnExit-Ereignis wird ausgelöst, wenn das TMemo den Fokus verliert, unabhängig davon, ob dies durch einen Tab-Wechsel oder eine andere Aktion geschieht. Du kannst diese Methode verwenden, um sicherzustellen, dass die gewünschten Aktionen ausgeführt werden, wenn der Fokus sich ändert.

Ich hoffe, das hilft! Lass mich wissen, ob du noch weitere Fragen hast. 😊

Wie sieht es mit OnActivate eines anderen Frames aus ?
Das OnActivate-Ereignis eines Frames wird ausgelöst, wenn der Frame aktiviert wird, also den Fokus erhält. Du kannst dieses Ereignis nutzen, um sicherzustellen, dass bestimmte Aktionen ausgeführt werden, wenn ein anderer Frame aktiviert wird. Hier ist ein Beispiel, wie du das umsetzen kannst:

Definiere das OnActivate-Ereignis für den Frame:

cpp
void __fastcall TForm1::FrameOnActivate(TObject *Sender) {
    // Hier kommt der Code, der ausgeführt werden soll, wenn der Frame aktiviert wird
}
Weise das OnActivate-Ereignis dem Frame zu:

cpp
Frame1->OnActivate = FrameOnActivate;
Überprüfe das Flag im globalen Timer:

Im globalen Timer überprüfst du das Flag, bevor du die Frames wechselst.

cpp
void __fastcall TForm1::GlobalTimerRoutine(TObject *Sender) {
    if (isMouseLeftRoutineRunning) {
        return; // Verhindert, dass der globale Timer stört
    }

    // Hier kommt der Code für den Frame-Wechsel
    // ...
}
Durch diesen Ansatz stellst du sicher, dass die OnMouseLeft-Routine vollständig durchlaufen wird, bevor der globale Timer wieder aktiv wird. Das Flag isMouseLeftRoutineRunning überwacht den Zustand der Routine und verhindert, dass der globale Timer stört, solange die Routine läuft.

Ich hoffe, das hilft! Lass mich wissen, ob du noch weitere Fragen hast. 😊

OK ich denke das aber der erste Ansatz von Dir sehr kompakt ist und eine guten atomaren Charakter aufweist. Dieser Ansatz braucht sehr wenig Code. Stimmt Du mir zu ?
Ja, ich stimme dir zu! Der erste Ansatz ist sehr kompakt und hat einen guten atomaren Charakter. Durch die Verwendung des Flags isRoutineRunning wird sichergestellt, dass die OnMouseLeft-Routine vollständig durchlaufen wird, bevor der globale Timer wieder aktiv wird. Dies minimiert den Code und sorgt für eine klare und effiziente Lösung.

Wenn du noch weitere Fragen hast oder Unterstützung benötigst, stehe ich dir gerne zur Verfügung! 😊

Vieln Dank ich werde das ausprobieren. In einem Beispiel- Code mit T Timer und ein TMemo in einem TForm.
Gern geschehen! Hier ist ein Beispielcode, der zeigt, wie du einen TTimer und ein TMemo in einem TForm verwenden kannst, um sicherzustellen, dass die OnMouseLeft-Routine vollständig durchlaufen wird, bevor der globale Timer wieder aktiv wird:

cpp
#include <vcl.h>
#include <vector>
#pragma hdrstop

class TForm1 : public TForm {
__published:
    TMemo *Memo1;
    TTimer *GlobalTimer;
    void __fastcall MemoOnMouseLeft(TObject *Sender);
    void __fastcall GlobalTimerRoutine(TObject *Sender);
private:
    bool isMouseLeftRoutineRunning;
    bool CheckAndSetRoutineRunning();
    void ResetRoutineRunning();
public:
    __fastcall TForm1(TComponent* Owner);
};

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner), isMouseLeftRoutineRunning(false) {
    Memo1 = new TMemo(this);
    Memo1->Parent = this;
    Memo1->OnMouseLeave = MemoOnMouseLeft;

    GlobalTimer = new TTimer(this);
    GlobalTimer->Interval = 1000; // Beispiel: 1 Sekunde
    GlobalTimer->OnTimer = GlobalTimerRoutine;
    GlobalTimer->Enabled = true;
}

bool TForm1::CheckAndSetRoutineRunning() {
    if (isMouseLeftRoutineRunning) {
        return false; // Verhindert, dass die Routine erneut gestartet wird
    }
    isMouseLeftRoutineRunning = true;
    return true;
}

void TForm1::ResetRoutineRunning() {
    isMouseLeftRoutineRunning = false;
}

void __fastcall TForm1::MemoOnMouseLeft(TObject *Sender) {
    if (!CheckAndSetRoutineRunning()) {
        return;
    }

    // Hier kommt der Code für die OnMouseLeft-Routine
    // ...

    ResetRoutineRunning();
}

void __fastcall TForm1::GlobalTimerRoutine(TObject *Sender) {
    if (isMouseLeftRoutineRunning) {
        return; // Verhindert, dass der globale Timer stört
    }

    // Hier kommt der Code für den Frame-Wechsel
    // ...
}
