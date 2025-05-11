#include <vector>
#include <string>
#include <vcl.h>
#include <MSAccess.hpp>  // Für SDAC

// Beispiel-Funktion zum Abrufen der Spaltennamen
std::vector<std::string> GetColumnNames(TMSQuery* Query)
{
    std::vector<std::string> columnNames;

    // Abfrage vorbereiten: z.B. SELECT * FROM <Tabelle>
    Query->SQL->Text = "SELECT * FROM DeineTabelle WHERE 1 = 0"; // keine Daten, nur Struktur
    Query->Open();

    // Spaltenanzahl abrufen
    int fieldCount = Query->FieldCount;

    for (int i = 0; i < fieldCount; ++i)
    {
        AnsiString colName = Query->Fields->Fields[i]->FieldName;
        columnNames.push_back(std::string(colName.c_str()));
    }

    Query->Close();

    return columnNames;
}

WHERE 1 = 0 in der SQL-Anweisung sorgt dafür, dass keine Daten abgerufen werden – nur die Struktur (d. h. Spalteninformationen).

Query->Fields->Fields[i]->FieldName gibt dir den Spaltennamen als AnsiString, den du dann in std::string umwandelst.





#include <vector>
#include <string>
#include <utility>   // für std::pair
#include <vcl.h>
#include <MSAccess.hpp>  // SDAC

typedef std::pair<std::string, std::string> ColumnInfo;

std::vector<ColumnInfo> GetColumnInfo(TMSQuery* Query)
{
    std::vector<ColumnInfo> columns;

    Query->SQL->Text = "SELECT * FROM DeineTabelle WHERE 1 = 0";  // keine Daten laden
    Query->Open();

    int fieldCount = Query->FieldCount;

    for (int i = 0; i < fieldCount; ++i)
    {
        TField* field = Query->Fields->Fields[i];

        AnsiString name = field->FieldName;
        TFieldType type = field->DataType;

        // TFieldType als lesbaren Text (optional schöner darstellen)
        AnsiString typeName = FieldTypeToStr(type);

        columns.push_back(
            std::make_pair(std::string(name.c_str()), std::string(typeName.c_str()))
        );
    }

    Query->Close();

    return columns;
}

// Hilfsfunktion zur Umwandlung von TFieldType in Klartext
AnsiString FieldTypeToStr(TFieldType fieldType)
{
    switch (fieldType)
    {
        case ftString:       return "ftString";
        case ftInteger:      return "ftInteger";
        case ftFloat:        return "ftFloat";
        case ftDate:         return "ftDate";
        case ftBoolean:      return "ftBoolean";
        case ftTime:         return "ftTime";
        case ftDateTime:     return "ftDateTime";
        case ftWideString:   return "ftWideString";
        case ftMemo:         return "ftMemo";
        case ftBCD:          return "ftBCD";
        case ftLargeint:     return "ftLargeint";
        // Weitere je nach Bedarf ergänzen
        default:             return "ftUnknown";
    }
}

Du bekommst einen std::vector<std::pair<std::string, std::string>> wie:
{ ("KundenID", "ftInteger"), ("Vorname", "ftString"), ("Geburtstag", "ftDate") }







Sehr gut – dann erweitern wir den Code um folgende zusätzliche Spalteninformationen:

Spaltenname

Datentyp (als Klartext)

Nullable (ob NULL erlaubt ist)

Maximale Länge (z. B. bei ftString)

Strukturvorschlag mit std::pair bleibt erhalten
Wir erweitern auf std::pair<std::string, std::string> mit einem lesbaren Format in std::string, z. B.:

plaintext
Kopieren
Bearbeiten
("Vorname", "ftString, nullable, size=50")
Beispielcode (C++03)
cpp
Kopieren
Bearbeiten
#include <vector>
#include <string>
#include <utility>
#include <sstream>
#include <vcl.h>
#include <MSAccess.hpp>  // SDAC

typedef std::pair<std::string, std::string> ColumnInfo;

std::vector<ColumnInfo> GetDetailedColumnInfo(TMSQuery* Query)
{
    std::vector<ColumnInfo> columns;

    Query->SQL->Text = "SELECT * FROM DeineTabelle WHERE 1 = 0";
    Query->Open();

    int fieldCount = Query->FieldCount;

    for (int i = 0; i < fieldCount; ++i)
    {
        TField* field = Query->Fields->Fields[i];

        AnsiString name = field->FieldName;
        TFieldType type = field->DataType;
        bool isNullable = !(field->Required);  // Required == NOT NULL
        int size = field->Size;

        // Textbeschreibung erzeugen
        std::ostringstream desc;
        desc << FieldTypeToStr(type).c_str();

        if (isNullable)
            desc << ", nullable";
        else
            desc << ", not null";

        if (size > 0)
            desc << ", size=" << size;

        columns.push_back(std::make_pair(
            std::string(name.c_str()),
            desc.str()
        ));
    }

    Query->Close();

    return columns;
}

// Gleiche Hilfsfunktion wie vorher
AnsiString FieldTypeToStr(TFieldType fieldType)
{
    switch (fieldType)
    {
        case ftString:       return "ftString";
        case ftInteger:      return "ftInteger";
        case ftFloat:        return "ftFloat";
        case ftDate:         return "ftDate";
        case ftBoolean:      return "ftBoolean";
        case ftTime:         return "ftTime";
        case ftDateTime:     return "ftDateTime";
        case ftWideString:   return "ftWideString";
        case ftMemo:         return "ftMemo";
        case ftBCD:          return "ftBCD";
        case ftLargeint:     return "ftLargeint";
        default:             return "ftUnknown";
    }
}
Beispielausgabe
plaintext
Kopieren
Bearbeiten
("Vorname", "ftString, nullable, size=50")
("KundenID", "ftInteger, not null")
("Geburtstag", "ftDate, nullable")
Wenn du willst, kann ich dir auch einen Formatumschalter einbauen, z. B. für JSON oder CSV-Ausgabe. Wäre so eine Ausgabeform nützlich für dich?

//===========================Interessant==============================

Hier sind zwei Varianten zur Formatierung der Spalteninfos als:

CSV (z. B. für Excel oder einfache Logs)

JSON (z. B. für Konfigdateien oder APIs)

Ich zeige dir einfach eine Funktion, die aus dem std::vector<ColumnInfo> die gewünschte Darstellung als std::string erzeugt.

1. CSV-Ausgabe
cpp
Kopieren
Bearbeiten
std::string FormatAsCSV(const std::vector<ColumnInfo>& columns)
{
    std::ostringstream out;
    out << "ColumnName,TypeInfo\n";

    for (size_t i = 0; i < columns.size(); ++i)
    {
        out << "\"" << columns[i].first << "\",\"" << columns[i].second << "\"\n";
    }

    return out.str();
}
Beispielausgabe:

csv
Kopieren
Bearbeiten
ColumnName,TypeInfo
"Vorname","ftString, nullable, size=50"
"KundenID","ftInteger, not null"
"Geburtstag","ftDate, nullable"
2. JSON-Ausgabe (vereinfacht)
cpp
Kopieren
Bearbeiten
std::string FormatAsJSON(const std::vector<ColumnInfo>& columns)
{
    std::ostringstream out;
    out << "[\n";

    for (size_t i = 0; i < columns.size(); ++i)
    {
        out << "  {\n"
            << "    \"name\": \"" << columns[i].first << "\",\n"
            << "    \"type\": \"" << columns[i].second << "\"\n"
            << "  }";

        if (i != columns.size() - 1)
            out << ",";

        out << "\n";
    }

    out << "]\n";
    return out.str();
}
Beispielausgabe:

json
Kopieren
Bearbeiten
[
  {
    "name": "Vorname",
    "type": "ftString, nullable, size=50"
  },
  {
    "name": "KundenID",
    "type": "ftInteger, not null"
  },
  {
    "name": "Geburtstag",
    "type": "ftDate, nullable"
  }
]
Verwendung:
Du kannst z. B. in deiner Anwendung einfach so aufrufen:

cpp
Kopieren
Bearbeiten
std::string csv = FormatAsCSV(columns);
std::string json = FormatAsJSON(columns);

// In Datei schreiben, Log, Konsole etc.
Möchtest du zusätzlich eine Funktion zum Export in eine Datei (Textdatei, UTF-8, etc.)?



Datei speichern (ANSI oder UTF-8)
Variante 1: ANSI-Datei
cpp
Kopieren
Bearbeiten
#include <fstream>

bool SaveToFileAnsi(const std::string& content, const std::string& filePath)
{
    std::ofstream file(filePath.c_str());
    if (!file.is_open())
        return false;

    file << content;
    file.close();
    return true;
}
Variante 2: UTF-8-Datei mit BOM (für Excel, Notepad++ etc.)
cpp
Kopieren
Bearbeiten
bool SaveToFileUtf8(const std::string& content, const std::string& filePath)
{
    std::ofstream file(filePath.c_str(), std::ios::binary);
    if (!file.is_open())
        return false;

    // UTF-8 BOM: EF BB BF
    unsigned char bom[] = {0xEF, 0xBB, 0xBF};
    file.write(reinterpret_cast<char*>(bom), 3);
    file << content;
    file.close();
    return true;
}
Beispielnutzung
cpp
Kopieren
Bearbeiten
std::vector<ColumnInfo> columns = GetDetailedColumnInfo(Query);

std::string json = FormatAsJSON(columns);
SaveToFileUtf8(json, "C:\\temp\\columns.json");

std::string csv = FormatAsCSV(columns);
SaveToFileUtf8(csv, "C:\\temp\\columns.csv");
Hinweise:

Pfade mit \\ angeben oder L"..." für WideString nutzen.

Für internationale Zeichen empfiehlt sich UTF-8 mit BOM, sonst zeigt z. B. Excel manchmal Kauderwelsch.