 Was du brauchst
Alle Controls (Children) des Frames durchlaufen.

Ihre Namen (z. B. Edit1, ComboBox3, etc.) als Spaltennamen verwenden.

Prüfen, ob eine Tabelle mit dem gewünschten Namen schon existiert.

Wenn nicht vorhanden, dann per SQL CREATE TABLE mit diesen Spaltennamen.

Die Tabelle bleibt in der DB erhalten (persistent).

🔧 Annahmen
Du nutzt eine TMSConnection und TMSQuery von SDAC.

Die Spalten sind z. B. VARCHAR(255) oder einheitlich.

C++03 in C++Builder 10.4.2 (Sydney) – also kein modernes auto, keine Ranges, aber alles solide nutzbar.

🧩 Code: Erstelle Tabelle aus Child-Komponenten eines Frames
cpp
Kopieren
Bearbeiten
#include <vcl.h>
#include <vector>
#include <string>
#include <sstream>
#include <MSAccess.hpp>  // SDAC

bool TableExists(TMSConnection* Conn, const AnsiString& tableName)
{
    TMSQuery* q = new TMSQuery(NULL);
    q->Connection = Conn;
    q->SQL->Text = "SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = :name";
    q->ParamByName("name")->AsString = tableName;
    q->Open();
    bool exists = !q->IsEmpty();
    q->Close();
    delete q;
    return exists;
}

bool CreateTableFromFrameChildren(TMSConnection* Conn, const AnsiString& tableName, TFrame* Frame)
{
    if (TableExists(Conn, tableName))
        return false; // Tabelle existiert bereits

    std::vector<std::string> columnNames;

    for (int i = 0; i < Frame->ControlCount; ++i)
    {
        TControl* ctrl = Frame->Controls[i];
        if (ctrl->Name.IsEmpty())
            continue;

        // Nur eindeutige Namen verwenden
        columnNames.push_back(std::string(ctrl->Name.c_str()));
    }

    if (columnNames.empty())
        return false;

    // SQL-Kommando bauen
    std::ostringstream sql;
    sql << "CREATE TABLE " << tableName.c_str() << " (\n";

    for (size_t i = 0; i < columnNames.size(); ++i)
    {
        sql << "  [" << columnNames[i] << "] VARCHAR(255)";
        if (i != columnNames.size() - 1)
            sql << ",\n";
    }

    sql << "\n);";

    // SQL ausführen
    TMSQuery* exec = new TMSQuery(NULL);
    exec->Connection = Conn;
    exec->SQL->Text = AnsiString(sql.str().c_str());
    try
    {
        exec->ExecSQL();
    }
    catch (const Exception& e)
    {
        ShowMessage("Fehler beim Erstellen der Tabelle: " + e.Message);
        delete exec;
        return false;
    }

    delete exec;
    return true;
}
✅ Verwendung
cpp
Kopieren
Bearbeiten
// Beispiel
CreateTableFromFrameChildren(MSConnection1, "MeineTabelle", MyFrame);
📌 Hinweise
Nutzt INFORMATION_SCHEMA.TABLES, um die Existenz zu prüfen (funktioniert bei MS SQL Server).

Du kannst bei Bedarf auch den Typ je nach Control (z. B. TEdit, TCheckBox) differenzieren.

VARCHAR(255) ist als Standard gewählt, lässt sich aber einfach pro Control erweitern.

Möchtest du auch automatisch den Datentyp je nach Komponententyp (z. B. int für TCheckBox, date für TDateTimePicker, etc.) setzen?







Du:
Ja das wäre gut


ChatGPT:
Perfekt – dann bauen wir die Funktion so um, dass der Spaltentyp automatisch anhand des Komponententyps gesetzt wird.

🧠 Logik: Mapping von Komponenten auf SQL-Typen
VCL-Komponente	SQL-Spaltentyp	Hinweis
TEdit	VARCHAR(255)	Textfeld
TMemo	TEXT	Längerer Text
TComboBox	VARCHAR(255)	Vorauswahl + Text
TCheckBox	BIT	Boolean (0/1)
TDateTimePicker	DATETIME	Datum/Zeit
Andere (Default)	VARCHAR(255)	Fallback

🧩 Erweiterter Code mit Typ-Erkennung
cpp
Kopieren
Bearbeiten
#include <vcl.h>
#include <vector>
#include <string>
#include <sstream>
#include <MSAccess.hpp>

bool TableExists(TMSConnection* Conn, const AnsiString& tableName)
{
    TMSQuery* q = new TMSQuery(NULL);
    q->Connection = Conn;
    q->SQL->Text = "SELECT * FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME = :name";
    q->ParamByName("name")->AsString = tableName;
    q->Open();
    bool exists = !q->IsEmpty();
    q->Close();
    delete q;
    return exists;
}

// Bestimme SQL-Typ anhand des VCL-Komponenten-Typs
std::string GetSQLTypeForControl(TControl* ctrl)
{
    if (dynamic_cast<TEdit*>(ctrl))
        return "VARCHAR(255)";
    else if (dynamic_cast<TMemo*>(ctrl))
        return "TEXT";
    else if (dynamic_cast<TComboBox*>(ctrl))
        return "VARCHAR(255)";
    else if (dynamic_cast<TCheckBox*>(ctrl))
        return "BIT";
    else if (dynamic_cast<TDateTimePicker*>(ctrl))
        return "DATETIME";
    else
        return "VARCHAR(255)"; // Fallback
}

bool CreateTableFromFrameChildren(TMSConnection* Conn, const AnsiString& tableName, TFrame* Frame)
{
    if (TableExists(Conn, tableName))
        return false; // Tabelle existiert bereits

    std::vector<std::pair<std::string, std::string> > columns;

    for (int i = 0; i < Frame->ControlCount; ++i)
    {
        TControl* ctrl = Frame->Controls[i];
        if (ctrl->Name.IsEmpty())
            continue;

        std::string name = std::string(ctrl->Name.c_str());
        std::string sqlType = GetSQLTypeForControl(ctrl);

        columns.push_back(std::make_pair(name, sqlType));
    }

    if (columns.empty())
        return false;

    // SQL-Kommando bauen
    std::ostringstream sql;
    sql << "CREATE TABLE [" << tableName.c_str() << "] (\n";

    for (size_t i = 0; i < columns.size(); ++i)
    {
        sql << "  [" << columns[i].first << "] " << columns[i].second;
        if (i != columns.size() - 1)
            sql << ",\n";
    }

    sql << "\n);";

    // SQL ausführen
    TMSQuery* exec = new TMSQuery(NULL);
    exec->Connection = Conn;
    exec->SQL->Text = AnsiString(sql.str().c_str());
    try
    {
        exec->ExecSQL();
    }
    catch (const Exception& e)
    {
        ShowMessage("Fehler beim Erstellen der Tabelle: " + e.Message);
        delete exec;
        return false;
    }

    delete exec;
    return true;
}
✅ Beispielverwendung:
cpp
Kopieren
Bearbeiten
CreateTableFromFrameChildren(MSConnection1, "EingabeTabelle", Frame1);
📌 Erweiterungsmöglichkeiten
Wenn du z. B. TSpinEdit, TRadioButton, TPanel oder TMaskEdit auch berücksichtigen willst, kann ich das ergänzen.

Ebenso möglich: Zusatzinfos wie „NOT NULL“, Standardwerte, Auto-Increment.

Möchtest du das auf bestimmte Control-Typen einschränken oder auch verschachtelte Controls (z. B. in TPanel oder TGroupBox) mit aufnehmen?







Du:
Bitte es sollte eine ID als int in Form einer Identitätsspezifikation als erste Spalte geben. Des weiteren es sei zunächst als Test also Name der Datenbank Nordwind und das Schema dbo. Bitte dies berücksichtigen.


ChatGPT:
Sehr gut – dann fügen wir folgendes hinzu:

✅ Zusätzliche Anforderungen
Die Tabelle bekommt als erste Spalte eine ID mit:

sql
Kopieren
Bearbeiten
ID INT IDENTITY(1,1) PRIMARY KEY
(automatisch hochzählend)

Die Datenbank ist Nordwind, Schema ist dbo.

→ Wir setzen also den vollqualifizierten Tabellennamen auf:

sql
Kopieren
Bearbeiten
[Nordwind].[dbo].[Tabellenname]
🧩 Finaler Code (mit ID-Spalte und Schema)
cpp
Kopieren
Bearbeiten
#include <vcl.h>
#include <vector>
#include <string>
#include <sstream>
#include <MSAccess.hpp>

bool TableExists(TMSConnection* Conn, const AnsiString& tableName)
{
    TMSQuery* q = new TMSQuery(NULL);
    q->Connection = Conn;

    // Prüfe mit SCHEMA und DB
    q->SQL->Text =
        "SELECT * FROM [Nordwind].INFORMATION_SCHEMA.TABLES "
        "WHERE TABLE_NAME = :name AND TABLE_SCHEMA = 'dbo'";
    q->ParamByName("name")->AsString = tableName;
    q->Open();
    bool exists = !q->IsEmpty();
    q->Close();
    delete q;
    return exists;
}

std::string GetSQLTypeForControl(TControl* ctrl)
{
    if (dynamic_cast<TEdit*>(ctrl))
        return "VARCHAR(255)";
    else if (dynamic_cast<TMemo*>(ctrl))
        return "TEXT";
    else if (dynamic_cast<TComboBox*>(ctrl))
        return "VARCHAR(255)";
    else if (dynamic_cast<TCheckBox*>(ctrl))
        return "BIT";
    else if (dynamic_cast<TDateTimePicker*>(ctrl))
        return "DATETIME";
    else
        return "VARCHAR(255)";
}

bool CreateTableFromFrameChildren(TMSConnection* Conn, const AnsiString& tableName, TFrame* Frame)
{
    if (TableExists(Conn, tableName))
        return false;

    std::vector<std::pair<std::string, std::string> > columns;

    for (int i = 0; i < Frame->ControlCount; ++i)
    {
        TControl* ctrl = Frame->Controls[i];
        if (ctrl->Name.IsEmpty())
            continue;

        std::string name = std::string(ctrl->Name.c_str());
        std::string sqlType = GetSQLTypeForControl(ctrl);

        columns.push_back(std::make_pair(name, sqlType));
    }

    if (columns.empty())
        return false;

    // SQL-Kommando bauen
    std::ostringstream sql;
    sql << "CREATE TABLE [Nordwind].[dbo].[" << tableName.c_str() << "] (\n";

    // Erste Spalte: ID als Identity
    sql << "  [ID] INT IDENTITY(1,1) PRIMARY KEY";

    if (!columns.empty())
        sql << ",\n";

    for (size_t i = 0; i < columns.size(); ++i)
    {
        sql << "  [" << columns[i].first << "] " << columns[i].second;
        if (i != columns.size() - 1)
            sql << ",\n";
    }

    sql << "\n);";

    // SQL ausführen
    TMSQuery* exec = new TMSQuery(NULL);
    exec->Connection = Conn;
    exec->SQL->Text = AnsiString(sql.str().c_str());
    try
    {
        exec->ExecSQL();
    }
    catch (const Exception& e)
    {
        ShowMessage("Fehler beim Erstellen der Tabelle: " + e.Message);
        delete exec;
        return false;
    }

    delete exec;
    return true;
}
✅ Beispielaufruf
cpp
Kopieren
Bearbeiten
bool ok = CreateTableFromFrameChildren(MSConnection1, "EingabeTest", Frame1);

if (ok)
    ShowMessage("Tabelle wurde erstellt.");
else
    ShowMessage("Tabelle existiert bereits oder Fehler.");
Ergebnis: erzeugtes SQL (Beispiel)
sql
Kopieren
Bearbeiten
CREATE TABLE [Nordwind].[dbo].[EingabeTest] (
  [ID] INT IDENTITY(1,1) PRIMARY KEY,
  [Edit1] VARCHAR(255),
  [CheckBox1] BIT,
  [Memo1] TEXT,
  [DatePicker1] DATETIME
);