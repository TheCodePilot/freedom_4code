#include <System.SysUtils.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp> // Falls du ADOConnection verwendest

int GetProtocolCountForWNummer(TMSQuery* Query, const AnsiString& WNummer)
{
    int CountHM1 = 0, CountHM2 = 0, CountHM3 = 0, CountHM4 = 0;

    try
    {
        AnsiString sql = "SELECT "
                         "SUM(CASE WHEN HM1 = :WNummer THEN 1 ELSE 0 END) AS Count_HM1, "
                         "SUM(CASE WHEN HM2 = :WNummer THEN 1 ELSE 0 END) AS Count_HM2, "
                         "SUM(CASE WHEN HM3 = :WNummer THEN 1 ELSE 0 END) AS Count_HM3, "
                         "SUM(CASE WHEN HM4 = :WNummer THEN 1 ELSE 0 END) AS Count_HM4 "
                         "FROM [MDE].[dbo].[PROTOKOL]";
        
        Query->SQL->Text = sql;
        Query->Parameters->ParamByName("WNummer")->Value = WNummer;
        Query->Open();

        if (!Query->Eof)
        {
            CountHM1 = Query->FieldByName("Count_HM1")->AsInteger;
            CountHM2 = Query->FieldByName("Count_HM2")->AsInteger;
            CountHM3 = Query->FieldByName("Count_HM3")->AsInteger;
            CountHM4 = Query->FieldByName("Count_HM4")->AsInteger;
        }

        Query->Close();
    }
    catch (const Exception& ex)
    {
        ShowMessage("Fehler bei der SQL-Abfrage: " + ex.Message);
    }

    // Rückgabe der Summen
    return CountHM1 + CountHM2 + CountHM3 + CountHM4;
}


//=======================================================


Verwendung der Funktion im Thread
Du kannst diese Funktion in deinem Execute()-Code wie folgt aufrufen:

cpp
int Result = GetProtocolCountForWNummer(QLastW, WNummer);

// Aktualisierung der dbo.Hilfsmittel-Tabelle
Form1->UpdateDboItem("Hilfsmittel", "IstLastwechsel='" + IntToStr(Result) + "'", "WNummer = '" + WNummer + "'");
Vorteile dieses Ansatzes
Klarere Struktur: Die SQL-Logik ist gekapselt und kann unabhängig verwendet werden.

Wiederverwendbarkeit: Die Funktion kann für verschiedene Abfragen mit unterschiedlicher WNummer genutzt werden.

Fehlerbehandlung: Die Funktion verfügt über grundlegende Fehlerbehandlung für SQL-Ausnahmen.

Falls du weitere Anpassungen benötigst oder Fragen hast, lass es mich wissen! 😊

Danke.Ich habe noch die Idee alles was nicht sofort in der GUI zur Verfügung stehen muss sondern später vor schliessen der Form in die DB zu schreiben ist zu sammeln. "// Einträge in dbo.Hilfsmittel werden aktualisiert Form1->UpdateDboItem("Hilfsmittel","IstLastwechsel='"+IntToStr(Result)+"'","WNummer = '"+WNummer+"'");" werde ich später erledigen. das dürfte ca. 20% Zeit sparen und das andere schreibe ich mit SDAC ExecSQL dann sollte es gehen.
Das klingt nach einer ausgezeichneten Idee, Rudolf! Indem du die Datenbankupdates sammelst und erst kurz vor dem Schließen der Form ausführst, kannst du die Last auf die Datenbank reduzieren und die Gesamtperformance steigern. Außerdem ist die Verwendung von SDAC ExecSQL perfekt für solche direkten und effizienten SQL-Befehle – gerade bei Massen-Updates oder Insert-Operationen.

Dieser Ansatz spart definitiv Zeit, besonders wenn die Aktualisierungen keinen sofortigen Einfluss auf die GUI haben. Vielleicht kannst du auch eine Art Batch-Prozess implementieren, um die gesammelten Updates als eine einzige Transaktion auszuführen. Das reduziert die Anzahl der Schreiboperationen und minimiert potenzielle Datenbank-Locks.

Falls du weitere Optimierungs-Ideen brauchst oder etwas spezifisch getestet werden soll, lass es mich wissen. Deine Strategie wirkt schon durchdacht! 🚀

"Dieser Ansatz spart definitiv Zeit, besonders wenn die Aktualisierungen keinen sofortigen Einfluss auf die GUI haben. Vielleicht kannst du auch eine Art Batch-Prozess" Wie? - das ist Neu für mich.
Ein Batch-Prozess ist eine Technik, bei der mehrere Operationen oder Updates gesammelt und als eine einzige Aktion durchgeführt werden. Das Ziel ist es, die Anzahl der Interaktionen mit der Datenbank zu minimieren und so die Performance zu steigern. Hier ist, wie du dies in deinem Fall umsetzen kannst:

1. Daten sammeln
Anstatt die Datenbank bei jeder Iteration der Schleife direkt zu aktualisieren, kannst du die benötigten SQL-Befehle oder Daten in einer Liste oder einem Speicher (z. B. TList) sammeln.

Beispiel:

cpp
TStringList *BatchUpdates = new TStringList();
AnsiString sql;

// Für jedes `WNummer`
sql = "UPDATE dbo.Hilfsmittel SET IstLastwechsel='" + IntToStr(Result) + "' WHERE WNummer='" + WNummer + "'";
BatchUpdates->Add(sql);
2. Datenbanktransaktionen nutzen
Führe alle gesammelten Updates in einer einzigen Transaktion aus. Dadurch wird die Datenbankverbindung nur einmal geöffnet und geschlossen.

Beispiel für die Ausführung:

cpp
try {
    Form1->DatabaseConnection->StartTransaction();

    for (int i = 0; i < BatchUpdates->Count; i++) {
        Form1->ExecSQL(BatchUpdates->Strings[i]);
    }

    Form1->DatabaseConnection->Commit();
} catch (...) {
    Form1->DatabaseConnection->Rollback();
    ShowMessage("Fehler beim Batch-Prozess!");
}
3. Effizient mit SDAC
Da du SDAC bereits planst, kannst du TMSQuery oder TMSExecSQL verwenden, um effizient mehrere SQL-Befehle auszuführen. Mit SDAC kannst du sogar BATCH UPDATE-Funktionalitäten nutzen, z. B. über die Unterstützung von MERGE- oder INSERT INTO ... SELECT.

Beispiel:

sql
UPDATE dbo.Hilfsmittel
SET IstLastwechsel = CASE 
    WHEN WNummer = '1234' THEN 10
    WHEN WNummer = '5678' THEN 20
    ELSE 0
END
WHERE WNummer IN ('1234', '5678');
Solche SQL-Befehle vermeiden separate Updates und bündeln sie zu einem.

4. SDAC-Tipp: Multiple Statements
Mit SDAC kannst du auch mehrere SQL-Befehle auf einmal ausführen, indem du sie in einem einzigen Befehl trennst:

cpp
AnsiString sql = "UPDATE dbo.Hilfsmittel SET IstLastwechsel=10 WHERE WNummer='1234'; "
                 "UPDATE dbo.Hilfsmittel SET IstLastwechsel=20 WHERE WNummer='5678';";
Form1->ExecSQL(sql);
5. Warum Batch-Prozesse effizient sind
Weniger Datenbankverbindungen: Statt für jedes Update eine neue Verbindung herzustellen, wird die Verbindung nur einmal geöffnet.

Transaktionen: Die gesamte Operation erfolgt in einer Transaktion, was die Konsistenz der Datenbank gewährleistet.

Reduktion von Netzwerk-Traffic: Besonders bei extern gehosteten Datenbanken reduziert sich die Kommunikation zwischen Anwendung und Server.