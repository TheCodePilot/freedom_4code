Alles klar! 😎 Wir können auch diesen VBA-Code in C++03 / C++Builder-kompatiblen Code übersetzen.

Ich zeige dir den Ansatz Schritt für Schritt, angepasst an C++03, ohne VBA-Objekte, also CSV-/Matrix-basierte Verarbeitung:

1️⃣ Datenmodell

Wir brauchen wieder Tabellen-Strukturen ähnlich wie vorher:

typedef std::vector<std::string> Row;
typedef std::vector<Row> Sheet;

// Hilfsfunktion zum Zugriff auf Zellen (0-basiert)
inline std::string &Cells(Sheet &sheet, size_t row, size_t col) {
    while (sheet.size() <= row) sheet.resize(row+1);
    while (sheet[row].size() <= col) sheet[row].resize(col+1);
    return sheet[row][col];
}

2️⃣ Farb- & Kürzel-Dictionaries

Da C++03 noch keine std::map Initializer erlaubt:

#include <map>
#include <string>

std::map<int,std::string> dictAbkuerzungen;
std::map<std::string,int> dictFarben;

void InitDictionaries() {
    dictAbkuerzungen[10] = "VT_Best.";  // Spalte K10 -> Index 10
    dictAbkuerzungen[11] = "DLZ VT";
    dictAbkuerzungen[12] = "EFK";
    dictAbkuerzungen[13] = "AK";
    dictAbkuerzungen[14] = "IMP";
    dictAbkuerzungen[15] = "DLZ-K";
    dictAbkuerzungen[16] = "A/E-Bau";
    dictAbkuerzungen[17] = "DLZ-NT";
    dictAbkuerzungen[18] = "AS";

    dictFarben["VT_Best."] = 0xB4C6E7;
    dictFarben["DLZ VT"] = 0xBFFFBF;
    dictFarben["EFK"] = 0xFFCC99;
    dictFarben["AK"] = 0xFFFF99;
    dictFarben["IMP"] = 0xFF99CC;
    dictFarben["DLZ-K"] = 0xC8FFFF;
    dictFarben["A/E-Bau"] = 0xFF9999;
    dictFarben["DLZ-NT"] = 0xC8E6FF;
    dictFarben["AS"] = 0xCCCCFF;
}

3️⃣ Kesselplanung Funktion
void PlaneKesselInZeitskala(Sheet &ws, bool wochenendeErlaubt) {

    size_t letzteZeile = ws.size();

    size_t spalteStartSkala = 23; // X = 23 (0-basiert)

    std::map<std::string,size_t> dictLetztesAusEinbau;

    for (size_t zeile = 10; zeile < letzteZeile; ++zeile) { // VBA: 11..letzteZeile
        std::string kesselName = Cells(ws,zeile,0); // Spalte A

        if (kesselName.empty()) continue;

        // Startdatum und Schicht ermitteln
        std::string startDatum, startSchicht;
        ErmittleStartFuerKessel(ws,kesselName,startDatum,startSchicht);

        if (startDatum.empty()) {
            Cells(ws,zeile,1) = "Kein Start gefunden"; // Spalte B
            continue;
        }

        size_t aktuelleSpalteInSkala = FindeStartSpalteInZeitskala(ws,startDatum,startSchicht,spalteStartSkala);

        if (dictLetztesAusEinbau.find(kesselName)!=dictLetztesAusEinbau.end()) {
            size_t letzterAusEinbau = dictLetztesAusEinbau[kesselName];
            size_t schichtenBisEFK = BerechneSchichtenBisEFK(ws,zeile);
            if (schichtenBisEFK>0) {
                size_t gewünschterEFKStartSpalte = letzterAusEinbau + 2;
                aktuelleSpalteInSkala = gewünschterEFKStartSpalte - schichtenBisEFK;
                if (aktuelleSpalteInSkala < spalteStartSkala) aktuelleSpalteInSkala = spalteStartSkala;
            }
        }

        // Durch Spalten K..S gehen
        for (size_t spalteSchritt = 10; spalteSchritt <= 18; ++spalteSchritt) {
            double arbeitsstunden = 0.0;
            if (!Cells(ws,zeile,spalteSchritt).empty())
                arbeitsstunden = atof(Cells(ws,zeile,spalteSchritt).c_str());

            if (arbeitsstunden > 0) {
                int arbeitsschichten = (int)((arbeitsstunden/8.0)+0.9999); // RoundUp
                std::string abk = dictAbkuerzungen[spalteSchritt];

                for (int s=0;s<arbeitsschichten;++s) {

                    if (!wochenendeErlaubt) {
                        while (SchrittIstManuell(abk) && IstWochenendSchicht(ws,aktuelleSpalteInSkala)) {
                            ++aktuelleSpalteInSkala;
                        }
                    }

                    Cells(ws,zeile,aktuelleSpalteInSkala) = abk;
                    // Farbe kann in GUI / Grid gesetzt werden

                    ++aktuelleSpalteInSkala;
                }

                if (spalteSchritt==16) { // Q
                    dictLetztesAusEinbau[kesselName] = aktuelleSpalteInSkala-1;
                }
            }
        }
    }
}

4️⃣ Hilfsfunktionen
void ErmittleStartFuerKessel(Sheet &ws, const std::string &kesselName,
                             std::string &startDatum, std::string &startSchicht)
{
    startDatum = ""; startSchicht="";
    int blockSpalten[] = {4,7,10,13,16}; // 0-basiert
    for (size_t b=0;b<5;++b) {
        for (size_t r=1;r<=8;++r) {
            if (Cells(ws,r,blockSpalten[b])==kesselName) {
                startDatum = Cells(ws,r,blockSpalten[b]+1);
                startSchicht = Cells(ws,r,blockSpalten[b]+2);
                return;
            }
        }
    }
}

size_t FindeStartSpalteInZeitskala(Sheet &ws, const std::string &datum,
                                   const std::string &schicht, size_t startSpalte)
{
    size_t sp = startSpalte;
    for (;sp<ws[0].size();++sp) {
        if (Cells(ws,7,sp)==datum && Cells(ws,9,sp)==schicht) return sp;
    }
    return startSpalte;
}

bool SchrittIstManuell(const std::string &abk) {
    return abk=="VT_Best." || abk=="EFK" || abk=="IMP" || abk=="A/E-Bau" || abk=="AS";
}

bool IstWochenendSchicht(Sheet &ws, size_t spalte) {
    std::string tag = ws[8][spalte]; // Zeile 9
    std::string schicht = ws[9][spalte]; // Zeile 10
    for (size_t i=0;i<tag.size();++i) tag[i]=tolower(tag[i]);
    for (size_t i=0;i<schicht.size();++i) schicht[i]=toupper(schicht[i]);

    if (tag=="sa") return true;
    if (tag=="so") return schicht!="NS";
    if (tag=="fr") return schicht=="NS";
    return false;
}

int BerechneSchichtenBisEFK(Sheet &ws, size_t zeile) {
    int summe=0;
    for (size_t spalte=10;spalte<=12;++spalte) {
        double h=0.0;
        if (!Cells(ws,zeile,spalte).empty())
            h=atof(Cells(ws,zeile,spalte).c_str());
        if (h>0) summe += (int)((h/8.0)+0.9999);
    }
    return summe;
}


✅ Damit hast du komplett C++03-kompatiblen Code für die Zeitskala-Planung.

Sheet ist eine Matrix, ersetzt Excel.

Alle VBA-Objekte (Worksheet, Dictionary) sind durch std::vector / std::map ersetzt.

Farbinformationen kannst du in TStringGrid / GUI anwenden, wenn nötig.