void __fastcall TForm1::Button1Click(TObject *Sender)
{
    try {
        CoInitialize(NULL);

        Variant xlApp = CreateOleObject("Excel.Application");
        xlApp.OlePropertySet("Visible", true);

        Variant wb = xlApp.OleFunction("Workbooks").OleFunction("Open",
            "C:\\Pfad\\zu\\deinerDatei.xlsx");

        Variant wsQuelle = wb.OleProperty("Sheets").OleProperty("Kesselverteilung");
        Variant wsRegel  = wb.OleProperty("Sheets").OleProperty("Regeltabelle");
        Variant wsZiel;

        std::wstring kesselName = L"9";

        long letzteZeileRegel = wsRegel.OleProperty("Cells")
            .OleProperty(wsRegel.OleProperty("Rows").OleProperty("Count"),1)
            .OleFunction("End", -4162).OleProperty("Row");

        long letzteZeileQ = wsQuelle.OleProperty("Cells")
            .OleProperty(wsQuelle.OleProperty("Rows").OleProperty("Count"),17)
            .OleFunction("End",-4162).OleProperty("Row");

        try {
            wsZiel = wb.OleProperty("Sheets").OleProperty("Auftragsvorauswahl");
            wsZiel.OleProperty("Cells").OleFunction("ClearContents");
        } catch(...) {
            wsZiel = wb.OleProperty("Sheets").OleFunction("Add");
            wsZiel.OlePropertySet("Name","Auftragsvorauswahl");
        }

        // Kopfzeilen
        Variant headers = VarArrayCreate({1,32}, varVariant);
        headers[1]="Zeile"; headers[2]="Ident"; headers[3]="Kundennr"; headers[4]="Name"; headers[5]="Material";
        headers[6]="Bezeichnung"; headers[7]="DM"; headers[8]="Länge"; headers[9]="Gewicht"; headers[10]="Volumen";
        headers[11]="Regelzeile"; headers[12]="Kesselname"; headers[13]="DM min"; headers[14]="DM max"; headers[15]="Länge min";
        headers[16]="Länge max"; headers[17]="Volumen min"; headers[18]="Volumen max"; headers[19]="Segmentanzahl"; headers[20]="Segment-Typ";
        headers[21]="Max Gewicht"; headers[22]="Kesselfläche mm²"; headers[23]="Packdichte"; headers[24]="Fläche Auftrag"; headers[25]="Höhendifferenz erlaubt";
        headers[26]="Quellzeile"; headers[27]="DM"; headers[28]="Länge"; headers[29]="Gewicht"; headers[30]="Volumen";
        headers[31]="Segment Nr"; headers[32]="Segment Neu";
        for(int c=1;c<=32;c++) wsZiel.OleProperty("Cells",1,c).OlePropertySet("Value",headers[c]);

        Variant quellRange = wsQuelle.OleProperty("Range",
            wsQuelle.OleProperty("Cells",42,2),
            wsQuelle.OleProperty("Cells",letzteZeileQ,30)
        );
        Variant quellDaten = quellRange.OleProperty("Value");

        Variant regelRange = wsRegel.OleProperty("Range",
            wsRegel.OleProperty("Cells",2,1),
            wsRegel.OleProperty("Cells",letzteZeileRegel,21)
        );
        Variant regelDaten = regelRange.OleProperty("Value");

        long resultRows = letzteZeileQ-42+1;
        Variant resultArray = VarArrayCreate({1,resultRows,1,32}, varVariant);

        // ===== Auswahl beste Regelzeile =====
        long bestR=0;
        double bestVolSum=0;
        long bestStartZeile=0;

        for(long r=1;r<=letzteZeileRegel-1;r++)
        {
            if(regelDaten[r][1].wstring()!=kesselName) continue;
            double dmMin=regelDaten[r][15], dmMax=regelDaten[r][16];
            double lMin=regelDaten[r][17], lMax=regelDaten[r][18];
            double erlaubteHoehenDiff = regelDaten[r][8];

            for(long i=1;i<=resultRows;i++)
            {
                double dm=quellDaten[i][17], l=quellDaten[i][20];
                if(dm<dmMin || dm>dmMax || l<lMin || l>lMax) continue;

                double volSum=0, gewSum=0;
                double minVol=regelDaten[r][10], maxVol=regelDaten[r][11], maxGew=regelDaten[r][12];

                for(long j=1;j<=resultRows;j++)
                {
                    double d=quellDaten[j][17], ln=quellDaten[j][20];
                    double vol=quellDaten[j][30], gew=quellDaten[j][29];
                    if(d>=dmMin && d<=dmMax && ln>=lMin && ln<=lMax &&
                       volSum+vol<=maxVol && gewSum+gew<=maxGew)
                    {
                        volSum+=vol;
                        gewSum+=gew;
                    }
                    else break;
                }

                if(volSum>=minVol && volSum>bestVolSum)
                {
                    bestVolSum=volSum;
                    bestR=r;
                    bestStartZeile=i;
                }
            }
        }

        if(bestR==0)
        {
            ShowMessage("Keine passende Regelzeile gefunden.");
            CoUninitialize();
            return;
        }

        // ===== Daten nach Regel übernehmen =====
        long zeileErgebnis=1;
        long segNummer=1;
        double segFlaeche=0.0;
        long maxSegmente=regelDaten[bestR][19];
        std::wstring segmentTypGlobal=regelDaten[bestR][20].wstring();
        double maxVol=regelDaten[bestR][11];

        std::map<long,bool> verplantDict;

        for(long i=1;i<=resultRows;i++)
        {
            double dm=quellDaten[i][17], l=quellDaten[i][20];
            if(dm<regelDaten[bestR][15] || dm>regelDaten[bestR][16] ||
               l<regelDaten[bestR][17] || l>regelDaten[bestR][18]) continue;

            if(segmentTypGlobal==L"f")
            {
                if(segNummer>maxSegmente) break;
            }
            else if(segmentTypGlobal==L"v")
            {
                double flaeche=quellDaten[i][28];
                if(segFlaeche+flaeche>maxVol)
                {
                    segNummer++;
                    if(segNummer>maxSegmente) break;
                    segFlaeche=flaeche;
                }
                else segFlaeche+=flaeche;
            }

            for(int c=1;c<=25;c++) resultArray[zeileErgebnis][c]=quellDaten[i][c];
            resultArray[zeileErgebnis][31]=segNummer;
            resultArray[zeileErgebnis][32]=segNummer;
            verplantDict[i]=true;
            zeileErgebnis++;
        }

        // ===== "v"-Segmentgewicht prüfen =====
        if(segmentTypGlobal==L"v")
        {
            double totalGewicht=0;
            for(long i=1;i<zeileErgebnis;i++)
                totalGewicht+=resultArray[i][29];

            double zielGewichtProSegment=totalGewicht/maxSegmente;
            double laufSumme=0;
            long aktSegment=1;
            for(long i=1;i<zeileErgebnis;i++)
            {
                double g=resultArray[i][29];
                if(laufSumme+g>zielGewichtProSegment && aktSegment<maxSegmente)
                {
                    aktSegment++;
                    laufSumme=g;
                }
                else laufSumme+=g;
                resultArray[i][32]=aktSegment;
            }
        }

        // ===== Ergebnis auf Blatt schreiben =====
        wsZiel.OleProperty("Range",
            wsZiel.OleProperty("Cells",2,57),
            wsZiel.OleProperty("Cells",1+zeileErgebnis,81)
        ).OlePropertySet("Value", resultArray);

        // ===== Status AG setzen =====
        for(long i=42;i<=letzteZeileQ;i++)
            wsQuelle.OleProperty("Cells", i, 33).OlePropertySet("Value",
                verplantDict.find(i)!=verplantDict.end()? "verplant" : "nicht verteilt");

        AutoFitColumns(wsZiel,1,84);
        ShowMessage(L"Auftragsvorauswahl für Kessel '" + kesselName + L"' abgeschlossen.");

        CoUninitialize();
    }
    catch(const Exception &e)
    {
        ShowMessage(e.Message);
        CoUninitialize();
    }
}
