#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include <map>
#include <string>
#include <cmath>
#include <vector>

#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}

// Hilfsfunktion für AutoFit von Spalten
void AutoFitColumns(Variant ws, int startCol, int endCol)
{
    for(int col=startCol; col<=endCol; col++)
    {
        try {
            ws.OleProperty("Columns", col).OleFunction("AutoFit");
        } catch(...) {}
    }
}

//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    try {
        CoInitialize(NULL);

        Variant xlApp = CreateOleObject("Excel.Application");
        xlApp.OlePropertySet("Visible", true);

        // Excel-Datei öffnen (Pfad anpassen!)
        Variant wb = xlApp.OleFunction("Workbooks").OleFunction("Open", "C:\\Pfad\\zu\\deinerDatei.xlsx");
        Variant wsQuelle = wb.OleProperty("Sheets").OleProperty("Kesselverteilung");
        Variant wsRegel  = wb.OleProperty("Sheets").OleProperty("Regeltabelle");
        Variant wsZiel;

        std::wstring kesselName = L"9";

        long letzteZeileRegel = wsRegel.OleProperty("Cells")
                                    .OleProperty(wsRegel.OleProperty("Rows").OleProperty("Count"),1)
                                    .OleFunction("End", -4162).OleProperty("Row"); // xlUp=-4162
        long letzteZeileQ = wsQuelle.OleProperty("Cells")
                                    .OleProperty(wsQuelle.OleProperty("Rows").OleProperty("Count"),17)
                                    .OleFunction("End",-4162).OleProperty("Row");

        // ===== Beste Regel ermitteln =====
        long bestR=0, bestStartZeile=0;
        double bestVolSum=0.0;

        for(long r=2; r<=letzteZeileRegel; r++)
        {
            Variant valKessel = wsRegel.OleProperty("Cells", r, 1).OleProperty("Value");
            if(valKessel.wstring() != kesselName) continue;

            double dmMin = wsRegel.OleProperty("Cells", r, 15).OleProperty("Value");
            double dmMax = wsRegel.OleProperty("Cells", r, 16).OleProperty("Value");
            double lMin  = wsRegel.OleProperty("Cells", r, 17).OleProperty("Value");
            double lMax  = wsRegel.OleProperty("Cells", r, 18).OleProperty("Value");
            Variant erlaubteHoehenDiff = wsRegel.OleProperty("Cells", r, 8).OleProperty("Value");
            std::wstring segmentTyp = wsRegel.OleProperty("Cells", r, 20).OleProperty("Value").wstring();

            for(long i=42; i<=letzteZeileQ; i++)
            {
                Variant valDM = wsQuelle.OleProperty("Cells", i, 17).OleProperty("Value");
                Variant valL  = wsQuelle.OleProperty("Cells", i, 20).OleProperty("Value");
                if(!VarIsNumeric(valDM) || !VarIsNumeric(valL)) continue;

                double dm = valDM;
                double l  = valL;

                if(dm>=dmMin && dm<=dmMax && l>=lMin && l<=lMax)
                {
                    double volSum=0.0, gewSum=0.0;
                    double maxVol = wsRegel.OleProperty("Cells", r, 11).OleProperty("Value");
                    double maxGew = wsRegel.OleProperty("Cells", r, 12).OleProperty("Value");
                    double minVol = wsRegel.OleProperty("Cells", r, 10).OleProperty("Value");

                    for(long j=42; j<=letzteZeileQ; j++)
                    {
                        Variant dVar   = wsQuelle.OleProperty("Cells", j, 17).OleProperty("Value");
                        Variant lnVar  = wsQuelle.OleProperty("Cells", j, 20).OleProperty("Value");
                        Variant volVar = wsQuelle.OleProperty("Cells", j, 30).OleProperty("Value");
                        Variant gewVar = wsQuelle.OleProperty("Cells", j, 29).OleProperty("Value");

                        if(VarIsNumeric(dVar) && VarIsNumeric(lnVar) && VarIsNumeric(volVar) && VarIsNumeric(gewVar))
                        {
                            double d = dVar;
                            double ln= lnVar;
                            double vol = volVar;
                            double gew = gewVar;

                            if(d>=dmMin && d<=dmMax && ln>=lMin && ln<=lMax)
                            {
                                if(volSum+vol<=maxVol && gewSum+gew<=maxGew)
                                {
                                    volSum += vol;
                                    gewSum += gew;
                                }
                                else break;
                            }
                        }
                    }

                    if(volSum>=minVol && volSum>bestVolSum)
                    {
                        bestVolSum = volSum;
                        bestR = r;
                        bestStartZeile = i;
                    }
                    break;
                }
            }
        }

        if(bestR==0)
        {
            ShowMessage("Keine passende Regel gefunden.");
            xlApp.OleFunction("Quit");
            CoUninitialize();
            return;
        }

        // ===== Zielblatt vorbereiten =====
        try {
            wsZiel = wb.OleProperty("Sheets").OleProperty("Auftragsvorauswahl");
            wsZiel.OleProperty("Cells").OleFunction("ClearContents");
        } catch(...) {
            wsZiel = wb.OleProperty("Sheets").OleFunction("Add");
            wsZiel.OlePropertySet("Name","Auftragsvorauswahl");
        }

        // ===== Kopfzeilen setzen =====
        Variant headers = VarArrayCreate({0,31}, varVariant);
        headers[0]="Zeile"; headers[1]="Ident"; headers[2]="Kundennr"; headers[3]="Name"; headers[4]="Material";
        headers[5]="Bezeichnung"; headers[6]="DM"; headers[7]="Länge"; headers[8]="Gewicht"; headers[9]="Volumen";
        headers[10]="Regelzeile"; headers[11]="Kesselname"; headers[12]="DM min"; headers[13]="DM max"; headers[14]="Länge min";
        headers[15]="Länge max"; headers[16]="Volumen min"; headers[17]="Volumen max"; headers[18]="Segmentanzahl"; headers[19]="Segment-Typ";
        headers[20]="Max Gewicht"; headers[21]="Kesselfläche mm²"; headers[22]="Packdichte"; headers[23]="Fläche Auftrag"; headers[24]="Höhendifferenz erlaubt";
        headers[25]="Quellzeile"; headers[26]="DM"; headers[27]="Länge"; headers[28]="Gewicht"; headers[29]="Volumen";
        headers[30]="Segment Nr"; headers[31]="Segment Neu";

        for(int c=0;c<=31;c++)
            wsZiel.OleProperty("Cells",1,c+1).OlePropertySet("Value",headers[c]);

        // ===== Aufträge übernehmen und Segmentlogik =====
        long zeileZiel = 2;
        long segNummer=1; double segFlaeche=0.0;
        Variant segmentTypGlobal = wsRegel.OleProperty("Cells",bestR,20).OleProperty("Value");
        long maxSegmente = wsRegel.OleProperty("Cells",bestR,19).OleProperty("Value");
        double maxFlaeche = wsRegel.OleProperty("Cells",bestR,11).OleProperty("Value");

        std::map<long,bool> verplantDict;

        for(long i=42;i<=letzteZeileQ;i++)
        {
            Variant valDM = wsQuelle.OleProperty("Cells", i, 17).OleProperty("Value");
            Variant valL  = wsQuelle.OleProperty("Cells", i, 20).OleProperty("Value");
            Variant valGew= wsQuelle.OleProperty("Cells", i, 29).OleProperty("Value");
            Variant valVol= wsQuelle.OleProperty("Cells", i, 30).OleProperty("Value");
            Variant valFlaeche= wsQuelle.OleProperty("Cells", i, 28).OleProperty("Value");

            if(!VarIsNumeric(valDM) || !VarIsNumeric(valL) || !VarIsNumeric(valGew) || !VarIsNumeric(valVol)) continue;

            double dm=valDM; double l=valL; double gew=valGew; double vol=valVol; double flaeche = VarIsNumeric(valFlaeche)?valFlaeche:0.0;
            double dmMin = wsRegel.OleProperty("Cells", bestR, 15).OleProperty("Value");
            double dmMax = wsRegel.OleProperty("Cells", bestR, 16).OleProperty("Value");
            double lMin  = wsRegel.OleProperty("Cells", bestR, 17).OleProperty("Value");
            double lMax  = wsRegel.OleProperty("Cells", bestR, 18).OleProperty("Value");

            if(dm<dmMin || dm>dmMax || l<lMin || l>lMax) continue;

            // Segmentlogik
            if(segmentTypGlobal==L"f") {
                if(segNummer>maxSegmente) break;
            } else if(segmentTypGlobal==L"v") {
                if(segFlaeche + flaeche > maxFlaeche) {
                    segNummer++;
                    if(segNummer>maxSegmente) break;
                    segFlaeche=flaeche;
                } else segFlaeche += flaeche;
            }

            // Werte in Zielblatt übernehmen
            wsZiel.OleProperty("Cells", zeileZiel, 1).OlePropertySet("Value",i); // Zeile
            wsZiel.OleProperty("Cells", zeileZiel, 32).OlePropertySet("Value",segNummer); // Segment Nr
            wsZiel.OleProperty("Cells", zeileZiel, 33).OlePropertySet("Value",segNummer); // Segment Neu

            verplantDict[i]=true;
            zeileZiel++;
        }

        // ===== AB:BB → BE:CF kopieren =====
        long maxZeileZiel = wsZiel.OleProperty("Cells").OleProperty("Rows").OleProperty("Count");
        maxZeileZiel = wsZiel.OleProperty("Cells", maxZeileZiel, 28).OleFunction("End", -4162).OleProperty("Row");

        wsZiel.OleProperty("Cells", 1, 57).OlePropertySet("Value","Segment Neu"); // Überschrift CF

        for(long i=2;i<=maxZeileZiel;i++)
        {
            Variant segNr = wsZiel.OleProperty("Cells", i, 32).OleProperty("Value"); // BB
            if(!VarIsEmpty(wsZiel.OleProperty("Cells", i, 27).OleProperty("Value")) && VarIsNumeric(segNr))
            {
                if(segmentTypGlobal==L"f")
                {
                    if(segNr<=maxSegmente)
                    {
                        for(int c=0;c<25;c++)
                            wsZiel.OleProperty("Cells", i, 57+c).OlePropertySet(
                                "Value",
                                wsZiel.OleProperty("Cells", i, 27+c).OleProperty("Value")
                            );
                        wsZiel.OleProperty("Cells", i, 81).OlePropertySet("Value", segNr); // Segment Neu
                    }
                }
                else if(segmentTypGlobal==L"v")
                {
                    if(segNr<=maxSegmente)
                    {
                        for(int c=0;c<25;c++)
                            wsZiel.OleProperty("Cells", i, 57+c).OlePropertySet(
                                "Value",
                                wsZiel.OleProperty("Cells", i, 27+c).OleProperty("Value")
                            );
                    }
                }
            }
        }

        // ===== Status AG in Quelle setzen =====
        for(long i=42;i<=letzteZeileQ;i++)
        {
            wsQuelle.OleProperty("Cells", i, 33).OlePropertySet("Value",
                verplantDict.find(i)!=verplantDict.end()? "verplant" : "nicht verteilt");
        }

        AutoFitColumns(wsZiel,1,84);

        ShowMessage(L"Auftragsvorauswahl für Kessel '" + kesselName + L"' abgeschlossen.");

        // wb.OleFunction("Save"); // optional speichern
        // xlApp.OleFunction("Quit");
        CoUninitialize();

    } catch(const Exception &e) {
        ShowMessage("Fehler: "+e.Message);
    }
}
