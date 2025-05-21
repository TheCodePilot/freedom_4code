🎯 Ziel bleibt:
Eine absichtlich falsche Spalte anfragen

Den Fehler „falsche Spaltenbezeichnung“ provozieren

Optional: vorher gültige Spaltennamen per Metadaten auslesen

✅ Beispiel 1: Fehler provozieren mit TMSQuery
cpp
Kopieren
Bearbeiten
void TForm1::TestFalscheSpalte()
{
    TMSQuery* qry = new TMSQuery(NULL);
    try
    {
        qry->Database = DeineMSConnection;  // MSConnection muss korrekt verbunden sein
        qry->SQL->Text = "SELECT NichtExistierendeSpalte FROM MyDataBank.dbo.MyTable";
        qry->Open();  // ➜ Hier wird ein Fehler auftreten
    }
    catch (const Exception& e)
    {
        ShowMessage("Fehler (erwartet): " + e.Message);
    }
    delete qry;
}
✅ Beispiel 2: Metadaten der Tabelle holen (gültige Spaltennamen anzeigen)
TMS bietet eigene Komponenten (z. B. TMSMetadata, TMSQuery) oder Zugriff über SQL selbst. Hier ein Ansatz ohne Extra-Komponenten:

cpp
Kopieren
Bearbeiten
void TForm1::ListeSpaltenMitTMSQuery()
{
    TMSQuery* qry = new TMSQuery(NULL);
    try
    {
        qry->Database = DeineMSConnection;
        qry->SQL->Text = "SELECT TOP 0 * FROM MyDataBank.dbo.MyTable";
        qry->Open();

        for (int i = 0; i < qry->FieldCount; ++i)
        {
            ShowMessage("Spalte: " + qry->Fields->Fields[i]->FieldName);
        }
    }
    catch (const Exception& e)
    {
        ShowMessage("Fehler beim Spaltenlesen: " + e.Message);
    }
    delete qry;
}
🧪 Bonus: Vorher prüfen, ob eine Spalte existiert
cpp
Kopieren
Bearbeiten
bool SpalteExistiert(TMSQuery* qry, const AnsiString& spaltenname)
{
    for (int i = 0; i < qry->FieldCount; ++i)
    {
        if (qry->Fields->Fields[i]->FieldName == spaltenname)
            return true;
    }
    return false;
}
💡 Hinweis zu TMSQuery
Die Komponente TMSQuery funktioniert analog zu TADOQuery, hat aber teils andere Verhalten beim Fehlerhandling. Wichtig ist:

qry->Open() schlägt sofort fehl bei falscher Spalte.

TMSQuery setzt Params-Platzhalter via :Paramname.

Database muss korrekt gesetzt sein (nicht Connection, wie bei ADO).

//==============================================

Gerne! Jetzt möchtest du gezielt den Fehler „Spaltenanzahl stimmt nicht überein“ (auch bekannt als „Column count doesn't match value count“ oder ähnlich) provozieren – typischerweise tritt das auf beim INSERT oder UPDATE, wenn du z. B.:

mehr oder weniger Werte angibst als Spalten

implizite Spaltenanzahl annimmst, aber explizit zu viele oder zu wenige Werte gibst

✅ Beispiel mit TMSQuery: Spaltenanzahl passt nicht
🎯 Fehler gezielt auslösen durch falsches INSERT
cpp
Kopieren
Bearbeiten
void TForm1::TestFalscheSpaltenanzahl()
{
    TMSQuery* qry = new TMSQuery(NULL);
    try
    {
        qry->Database = DeineMSConnection;

        // Beispiel: Tabelle hat 3 Spalten, aber wir geben 4 Werte
        qry->SQL->Text =
            "INSERT INTO MyDataBank.dbo.MyTable "
            "VALUES ('Wert1', 'Wert2', 'Wert3', 'Zuviel')";

        qry->ExecSQL();  // ➜ Hier sollte der Fehler auftreten
    }
    catch (const Exception& e)
    {
        ShowMessage("Fehler (erwartet): " + e.Message);
    }
    delete qry;
}
✅ So erzielst du sicher den Fehler, wenn du die Spaltenanzahl absichtlich falsch angibst.

🧪 Alternativen zum Fehler provozieren:
1. Weniger Werte als Spalten:
sql
Kopieren
Bearbeiten
-- Tabelle hat z. B. 4 Pflichtspalten, wir geben nur 2 Werte
INSERT INTO MyDataBank.dbo.MyTable VALUES ('A', 'B')
2. Keine Spaltenliste, aber Werteanzahl falsch:
sql
Kopieren
Bearbeiten
-- Keine Spalten explizit angegeben → SQL nimmt alle Pflichtfelder an
-- Aber du gibst nur 3 von 4 notwendigen Werten
INSERT INTO MyDataBank.dbo.MyTable VALUES ('A', 'B', 'C')
🛡️ Sicherer Stil (kein Fehler):
cpp
Kopieren
Bearbeiten
qry->SQL->Text =
    "INSERT INTO MyDataBank.dbo.MyTable (Spalte1, Spalte2, Spalte3) "
    "VALUES (:W1, :W2, :W3)";
qry->ParamByName("W1")->AsString = "abc";
// ...
qry->ExecSQL();
Damit bist du geschützt vor Änderungen in der Tabellenstruktur.

🎁 Tipp: Metadatenbasierte Spaltenzählung
Wenn du vorher prüfen willst, wie viele Spalten eine Tabelle hat:

cpp
Kopieren
Bearbeiten
int SpaltenAnzahl()
{
    TMSQuery* qry = new TMSQuery(NULL);
    int count = 0;
    try
    {
        qry->Database = DeineMSConnection;
        qry->SQL->Text = "SELECT TOP 0 * FROM MyDataBank.dbo.MyTable";
        qry->Open();
        count = qry->FieldCount;
    }
    __finally
    {
        delete qry;
    }
    return count;
}


//=========================

Sehr gut – du möchtest jetzt nicht annehmen, wie viele Spalten die Tabelle hat, sondern den Fehler „Spaltenanzahl stimmt nicht überein“ gezielt und dynamisch auslösen, indem du die tatsächliche Anzahl der Spalten per Metadaten ermittelst und dann absichtlich zu viele Werte lieferst.

✅ Vorgehen
Spaltenanzahl per SELECT TOP 0 * ermitteln

SQL-Statement mit +1 zusätzlichem Wert zusammenbauen

ExecSQL ausführen → Fehler wird provoziert

🔧 Kompletter Code (mit Metadaten-Auswertung)
cpp
Kopieren
Bearbeiten
void TForm1::TestFalscheSpaltenanzahl()
{
    TMSQuery* meta = new TMSQuery(NULL);
    TMSQuery* qry = new TMSQuery(NULL);

    try
    {
        meta->Database = DeineMSConnection;
        meta->SQL->Text = "SELECT TOP 0 * FROM MyDataBank.dbo.MyTable";
        meta->Open();

        int spaltenAnzahl = meta->FieldCount;
        ShowMessage("Tatsächliche Spaltenanzahl: " + IntToStr(spaltenAnzahl));

        // Jetzt baue das VALUES-Statement mit absichtlich ZU VIELEN Werten
        AnsiString values = "VALUES (";
        for (int i = 0; i < spaltenAnzahl + 1; ++i)  // +1: Fehlerabsicht
        {
            if (i > 0) values += ", ";
            values += "'Test" + IntToStr(i+1) + "'";
        }
        values += ")";

        qry->Database = DeineMSConnection;
        qry->SQL->Text = "INSERT INTO MyDataBank.dbo.MyTable " + values;

        qry->ExecSQL();  // ➜ Hier sollte der Fehler auftreten
    }
    catch (const Exception& e)
    {
        ShowMessage("Fehler (erwartet): " + e.Message);
    }

    delete meta;
    delete qry;
}
🧪 Ergebnis
Wenn z. B. die Tabelle 5 Spalten hat, erzeugt der Code:

sql
Kopieren
Bearbeiten
INSERT INTO MyDataBank.dbo.MyTable VALUES ('Test1','Test2','Test3','Test4','Test5','Test6')
→ Ergebnis: „Spaltenanzahl stimmt nicht überein“

🛡️ Hinweis
Dieser Fehler tritt nur auf, wenn keine Spaltenliste angegeben ist, also:

sql
Kopieren
Bearbeiten
INSERT INTO ... VALUES (...)
Wenn du explizit die Spalten angibst, erwartet SQL Server genau diese Anzahl – dann bekommst du bei falscher Wertanzahl denselben Fehler, aber expliziter.

//======================

Sehr gern – jetzt möchtest du einen Fehler vom Typ „falscher Spaltentyp“ (z. B. "Datentyp-Konflikt in Ausdruck" oder "conversion failed") auslösen – und zwar durch gezielte Verwendung der FieldDefs, um den erwarteten Typ der Spalten zur Laufzeit zu ermitteln – und absichtlich einen Wert falschen Typs einzusetzen.

✅ Ziel: Fehler durch falschen Datentyp (z. B. String in Integer-Spalte)
🔧 Beispiel: Laufzeitprüfung mit FieldDefs → absichtlich falscher Typ
cpp
Kopieren
Bearbeiten
void TForm1::TestFalscherSpaltentyp()
{
    TMSQuery* meta = new TMSQuery(NULL);
    TMSQuery* qry = new TMSQuery(NULL);

    try
    {
        meta->Database = DeineMSConnection;
        meta->SQL->Text = "SELECT * FROM MyDataBank.dbo.MyTable WHERE 1 = 0";
        meta->Open();  // oder alternativ: meta->FieldDefs->Update();

        meta->FieldDefs->Update();  // sorgt dafür, dass FieldDefs korrekt geladen werden

        if (meta->FieldDefs->Count == 0)
            throw Exception("Keine Spalten gefunden!");

        // Nimm z. B. die erste Spalte – und provoziere einen Typfehler
        TFieldDef* feld = meta->FieldDefs->Items[0];

        ShowMessage("Spalte: " + feld->Name + ", Typ: " + IntToStr((int)feld->DataType));

        // Jetzt absichtlich falschen Typ verwenden
        // Angenommen, Spalte 0 ist vom Typ ftInteger → wir setzen einen String
        qry->Database = DeineMSConnection;
        qry->SQL->Text = "INSERT INTO MyDataBank.dbo.MyTable (" + feld->Name + ") VALUES (:VAL)";
        qry->ParamByName("VAL")->AsString = "ABC123";  // absichtlich falscher Datentyp

        qry->ExecSQL();  // ➜ Sollte bei z. B. Integer-Spalte fehlschlagen
    }
    catch (const Exception& e)
    {
        ShowMessage("Fehler (erwartet): " + e.Message);
    }

    delete meta;
    delete qry;
}
🧠 Was passiert?
Wenn z. B. feld->DataType == ftInteger, erwartet SQL Server einen ganzzahligen Wert, aber du gibst "ABC123" als String → SQL Server löst einen Fehler aus wie:

Conversion failed when converting the varchar value 'ABC123' to data type int.

🧪 Variante: Datentyp gezielt nachschauen
Du kannst auch gezielt nach einem Spaltentyp suchen:

cpp
Kopieren
Bearbeiten
for (int i = 0; i < meta->FieldDefs->Count; ++i)
{
    TFieldDef* def = meta->FieldDefs->Items[i];
    if (def->DataType == ftInteger)  // Nur Integer-Spalten testen
    {
        ShowMessage("Test mit Spalte: " + def->Name);
        // Dann gezielt Insert mit String usw.
    }
}
🔧 FieldDefs->Update() vs. Open()
meta->Open() lädt auch die FieldDefs, aber erzeugt echte TField-Instanzen

FieldDefs->Update() ist ressourcenschonender, wenn du nur die Struktur brauchst

🔐 Hinweis
Wenn die Tabelle Pflichtfelder hat (NOT NULL), kann es sinnvoll sein, alle Spalten anzugeben – oder eine Testtabelle zu verwenden, wo du beliebig experimentieren kannst.

