Perfekt! 😎 Dann erkläre ich, wie wir Excel-Funktionen wie Index, Resize, Range usw. komplett in C++03/C++Builder nachbilden, sodass Excel nicht sichtbar sein muss.

Ziel


Keine Abhängigkeit von Excel – also kein COM / OLE nötig.


1:1 VBA-Funktionalität nachbilden:


Cells(row,col) → Zugriff auf Matrix


Range(start,end) → Submatrix


Resize → Bereich verschieben/vergrößern


Index → Elementzugriff in 2D-Array




Performance: alles in Speicher, ideal für große Datenmengen (20.000+ Zeilen).


Typen sicher handhaben: Strings, Double, Variant-ähnliche Logik.



Ansatz
1. Datenstrukturen
#include <vector>
#include <string>
#include <variant>
#include <algorithm>
#include <iostream>

typedef std::variant<double,std::wstring> CellValue;

struct Cell {
    CellValue value;
    bool isNumeric() const { return std::holds_alternative<double>(value); }
    double asDouble() const { return std::get<double>(value); }
    std::wstring asString() const { return std::holds_alternative<std::wstring>(value) ? std::get<std::wstring>(value) : L""; }
};

typedef std::vector<Cell> Row;
typedef std::vector<Row> Sheet;


2. Zugriff wie Cells(row,col)
Cell& Cells(Sheet &sheet, size_t row, size_t col) {
    if(row>=sheet.size()) sheet.resize(row+1);
    if(col>=sheet[row].size()) sheet[row].resize(col+1);
    return sheet[row][col];
}


3. Range(startRow,startCol,endRow,endCol) & Resize
Sheet Range(const Sheet &sheet, size_t startRow, size_t startCol, size_t endRow, size_t endCol) {
    Sheet result;
    for(size_t r=startRow;r<=endRow;r++) {
        Row row;
        for(size_t c=startCol;c<=endCol;c++)
            row.push_back(sheet[r][c]);
        result.push_back(row);
    }
    return result;
}

// Resize: ausgehend von startRow/startCol neue Größe rows x cols
Sheet Resize(const Sheet &sheet, size_t startRow, size_t startCol, size_t rows, size_t cols) {
    return Range(sheet, startRow, startCol, startRow+rows-1, startCol+cols-1);
}


4. Index wie in VBA
Cell Index(const Sheet &sheet, size_t row, size_t col) {
    return sheet[row][col];
}


5. Beispiel: Auftragsvorauswahl
Jetzt können wir das VBA-Makro fast 1:1 in C++ nachbilden:
Sheet wsQuelle, wsRegel, wsZiel;

// Zugriff wie Cells
double dmMin = Cells(wsRegel,r,14).asDouble(); // O in VBA
double dmMax = Cells(wsRegel,r,15).asDouble(); // P
double lMin  = Cells(wsRegel,r,16).asDouble(); // Q
double lMax  = Cells(wsRegel,r,17).asDouble(); // R

// Range/Resize/Index Beispiel
Sheet zielRange = Resize(wsZiel,1,0,1,25);
Cell val = Index(zielRange,0,0);



Alle Schleifen, Bedingungen, Segmentaufteilungen und Summierungen können jetzt vollständig in Speicher erfolgen.


Große Datenmengen (>20.000 Zeilen) werden dadurch viel schneller verarbeitet als mit COM/Excel.



💡 Vorteil: Du bekommst eine eigenständige C++ Bibliothek, die VBA-Daten und Logik 1:1 nachbildet, inklusive:


Cells(row,col)


Range(start,end)


Resize(rows,cols)


Index(row,col)


Dynamisches Hinzufügen von Zeilen/Spalten


Variant-Logik für Double/String



Wenn du willst, kann ich als Nächstes die komplette Auftragsvorauswahl inkl. Segmentlogik f/v, Volumen/Gewicht, Flächenberechnung und Status-Spalte AG direkt auf diese in-memory Sheets bauen, sodass keine Excel-Instanz mehr benötigt wird.
Willst du, dass ich das mache?


//=================

#include <vector>
#include <string>
#include <variant>
#include <algorithm>
#include <iostream>

typedef std::variant<double,std::wstring> CellValue;

struct Cell {
    CellValue value;
    bool isNumeric() const { return std::holds_alternative<double>(value); }
    double asDouble() const { return isNumeric() ? std::get<double>(value) : 0; }
    std::wstring asString() const { return std::holds_alternative<std::wstring>(value) ? std::get<std::wstring>(value) : L""; }
};

typedef std::vector<Cell> Row;
typedef std::vector<Row> Sheet;

// Cells(row,col) Zugriff
Cell& Cells(Sheet &sheet, size_t row, size_t col) {
    if(row >= sheet.size()) sheet.resize(row+1);
    if(col >= sheet[row].size()) sheet[row].resize(col+1);
    return sheet[row][col];
}

// Range(startRow,startCol,endRow,endCol)
Sheet Range(const Sheet &sheet, size_t startRow, size_t startCol, size_t endRow, size_t endCol) {
    Sheet result;
    for(size_t r=startRow; r<=endRow; r++) {
        Row row;
        for(size_t c=startCol; c<=endCol; c++) row.push_back(sheet[r][c]);
        result.push_back(row);
    }
    return result;
}

// Resize ausgehend von startRow/startCol
Sheet Resize(const Sheet &sheet, size_t startRow, size_t startCol, size_t rows, size_t cols) {
    return Range(sheet, startRow, startCol, startRow+rows-1, startCol+cols-1);
}

// Index(row,col) Zugriff wie VBA
Cell Index(const Sheet &sheet, size_t row, size_t col) {
    return sheet[row][col];
}

// Hilfsfunktion für Application.Index wie in VBA
Cell Application_Index(const Sheet &sheet, size_t row, size_t col) {
    return Index(sheet,row,col);
}


void Auftragsvorauswahl(Sheet &wsQuelle, Sheet &wsRegel, Sheet &wsZiel, const std::wstring &kesselName)
{
    wsZiel.clear();

    // Kopfzeilen
    Cells(wsZiel,0,0).value = L"Quellzeile"; Cells(wsZiel,0,1).value = L"DM";
    Cells(wsZiel,0,2).value = L"Länge";     Cells(wsZiel,0,3).value = L"Gewicht";
    Cells(wsZiel,0,4).value = L"Segment";   size_t zeileZiel=1;

    long letzteZeileQ = wsQuelle.size();
    long letzteZeileRegel = wsRegel.size();
    long bestR=0; double bestVolSum=0; long bestStartZeile=0;

    // Beste Regel finden
    for(long r=1;r<letzteZeileRegel;r++) {
        if(wsRegel[r][0].asString()!=kesselName) continue;

        double dmMin = Cells(wsRegel,r,14).asDouble();
        double dmMax = Cells(wsRegel,r,15).asDouble();
        double lMin  = Cells(wsRegel,r,16).asDouble();
        double lMax  = Cells(wsRegel,r,17).asDouble();
        double minVol= Cells(wsRegel,r,9).asDouble();
        double maxVol= Cells(wsRegel,r,10).asDouble();
        double maxGew= Cells(wsRegel,r,11).asDouble();
        std::wstring segmentTyp = wsRegel[r][19].asString();

        for(long i=41;i<letzteZeileQ;i++) {
            double dm = Cells(wsQuelle,i,16).asDouble();
            double l  = Cells(wsQuelle,i,19).asDouble();
            if(dm<dmMin || dm>dmMax || l<lMin || l>lMax) continue;

            double volSum=0, gewSum=0;
            for(long j=41;j<letzteZeileQ;j++) {
                double d=Cells(wsQuelle,j,16).asDouble();
                double ln=Cells(wsQuelle,j,19).asDouble();
                double vol=Cells(wsQuelle,j,29).asDouble();
                double gew=Cells(wsQuelle,j,28).asDouble();

                if(d>=dmMin && d<=dmMax && ln>=lMin && ln<=lMax &&
                   volSum+vol<=maxVol && gewSum+gew<=maxGew) {
                    volSum+=vol; gewSum+=gew;
                } else break;
            }

            if(volSum>=minVol && volSum>bestVolSum) {
                bestVolSum=volSum; bestR=r; bestStartZeile=i;
            }
        }
    }

    if(bestR==0) { std::wcout << L"Keine passende Regel gefunden\n"; return; }

    // Segmentlogik f/v
    long segNum_f=1, segNum_v=1; double laufGew_v=0;
    double S = Cells(wsRegel,bestR,18).asDouble();
    double U = Cells(wsRegel,bestR,20).asDouble();
    double maxGew = Cells(wsRegel,bestR,11).asDouble();
    long maxSeg = Cells(wsRegel,bestR,18).asDouble();
    std::wstring segmentTypGlobal = Cells(wsRegel,bestR,19).asString();

    for(long i=41;i<letzteZeileQ;i++) {
        double dm = Cells(wsQuelle,i,16).asDouble();
        double l  = Cells(wsQuelle,i,19).asDouble();
        double gew= Cells(wsQuelle,i,28).asDouble();
        double flaecheAuftrag=Cells(wsQuelle,i,27).asDouble();

        long segNum=1;
        if(segmentTypGlobal==L"f") {
            segNum=segNum_f;
            if(segNum_f<maxSeg) segNum_f++;
        } else if(segmentTypGlobal==L"v") {
            if(laufGew_v+gew>maxGew) { segNum_v++; laufGew_v=gew; } else laufGew_v+=gew;
            if(segNum_v>maxSeg) segNum_v=maxSeg;
            segNum=segNum_v;
        }

        // Zielblatt füllen
        Cells(wsZiel,zeileZiel,0).value = double(i+1);
        Cells(wsZiel,zeileZiel,1).value = dm;
        Cells(wsZiel,zeileZiel,2).value = l;
        Cells(wsZiel,zeileZiel,3).value = gew;
        Cells(wsZiel,zeileZiel,4).value = segNum;

        // Flächenberechnung
        double flaecheJeSegment = U/S;
        double maxBelegbareFlaeche = flaecheJeSegment*segNum/100;
        Cells(wsZiel,zeileZiel,5).value = flaecheJeSegment;
        Cells(wsZiel,zeileZiel,6).value = maxBelegbareFlaeche;

        zeileZiel++;
    }

    // Status-Spalte AG setzen
    std::map<long,bool> verplantDict;
    for(size_t i=1;i<wsZiel.size();i++) verplantDict[(long)Cells(wsZiel,i,0).asDouble()] = true;

    for(long i=41;i<letzteZeileQ;i++) {
        if(verplantDict.find(i+1)!=verplantDict.end())
            Cells(wsQuelle,i,32).value=L"verplant";
        else
            Cells(wsQuelle,i,32).value=L"nicht verteilt";
    }

    std::wcout << L"Auftragsvorauswahl fertig (Excel nicht nötig!)\n";
}
