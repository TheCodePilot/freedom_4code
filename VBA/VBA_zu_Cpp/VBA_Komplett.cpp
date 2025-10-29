void __fastcall TForm1::btnStartClick(TObject *Sender)
{
    try
    {
        if(!dlgDatei->Execute()) return;
        lblDatei->Caption = dlgDatei->FileName;

        CoInitialize(NULL);
        Variant xlApp = CreateOleObject("Excel.Application");
        xlApp.OlePropertySet("Visible", true);

        Variant wb = xlApp.OleFunction("Workbooks").OleFunction("Open", dlgDatei->FileName);
        Variant wsQuelle = wb.OleProperty("Sheets").OleProperty("Kesselverteilung");
        Variant wsRegel  = wb.OleProperty("Sheets").OleProperty("Regeltabelle");
        Variant wsZiel;

        // Zielblatt vorbereiten
        try {
            wsZiel = wb.OleProperty("Sheets").OleProperty("Auftragsvorauswahl");
            wsZiel.OleProperty("Cells").OleFunction("ClearContents");
        } catch(...) {
            wsZiel = wb.OleProperty("Sheets").OleFunction("Add");
            wsZiel.OlePropertySet("Name","Auftragsvorauswahl");
        }

        // Kopfzeilen StringGrid + Excel
        sgSegment->RowCount = 2;
        sgSegment->Cells[0][0]="Quellzeile"; sgSegment->Cells[1][0]="DM"; sgSegment->Cells[2][0]="Länge";
        sgSegment->Cells[3][0]="Gewicht"; sgSegment->Cells[4][0]="Segment";

        wsZiel.OleProperty("Range", wsZiel.OleProperty("Cells",1,1),
                                      wsZiel.OleProperty("Cells",1,5))
              .OlePropertySet("Value", VarArrayOf({ "Quellzeile","DM","Länge","Gewicht","Segment" }));

        // Letzte Zeilen bestimmen
        long letzteZeileQ = wsQuelle.OleProperty("Cells")
            .OleProperty(wsQuelle.OleProperty("Rows").OleProperty("Count"),17)
            .OleFunction("End",-4162).OleProperty("Row");
        long letzteZeileRegel = wsRegel.OleProperty("Cells")
            .OleProperty(wsRegel.OleProperty("Rows").OleProperty("Count"),1)
            .OleFunction("End",-4162).OleProperty("Row");

        std::wstring kesselName = L"9";
        long bestR=0; double bestVolSum=0; long bestStartZeile=0;

        // === Beste Regel ermitteln ===
        for(long r=2;r<=letzteZeileRegel;r++)
        {
            if(wsRegel.OleProperty("Cells",r,1).OleProperty("Value").wstring()!=kesselName) continue;

            double dmMin = wsRegel.OleProperty("Cells",r,15).OleProperty("Value");
            double dmMax = wsRegel.OleProperty("Cells",r,16).OleProperty("Value");
            double lMin = wsRegel.OleProperty("Cells",r,17).OleProperty("Value");
            double lMax = wsRegel.OleProperty("Cells",r,18).OleProperty("Value");
            double erlaubteHoehenDiff = wsRegel.OleProperty("Cells",r,8).OleProperty("Value");

            for(long i=42;i<=letzteZeileQ;i++)
            {
                double dm = wsQuelle.OleProperty("Cells",i,17).OleProperty("Value");
                double l  = wsQuelle.OleProperty("Cells",i,20).OleProperty("Value");
                if(dm<dmMin || dm>dmMax || l<lMin || l>lMax) continue;

                double volSum=0, gewSum=0;
                double minVol=wsRegel.OleProperty("Cells",r,10).OleProperty("Value");
                double maxVol=wsRegel.OleProperty("Cells",r,11).OleProperty("Value");
                double maxGew=wsRegel.OleProperty("Cells",r,12).OleProperty("Value");

                for(long j=42;j<=letzteZeileQ;j++)
                {
                    double d=wsQuelle.OleProperty("Cells",j,17).OleProperty("Value");
                    double ln=wsQuelle.OleProperty("Cells",j,20).OleProperty("Value");
                    double vol=wsQuelle.OleProperty("Cells",j,30).OleProperty("Value");
                    double gew=wsQuelle.OleProperty("Cells",j,29).OleProperty("Value");

                    if(d>=dmMin && d<=dmMax && ln>=lMin && ln<=lMax &&
                       volSum+vol<=maxVol && gewSum+gew<=maxGew)
                    {
                        volSum+=vol; gewSum+=gew;
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
            ShowMessage("Keine passende Regel gefunden");
            CoUninitialize(); return;
        }

        // === Segmentaufteilung f/v + Flächenberechnung + StringGrid ===
        long rowSG=1; // StringGrid-Zeilenindex
        long segNum_f=1, segNum_v=1;
        double laufGew_v=0;

        double S = wsRegel.OleProperty("Cells",bestR,19).OleProperty("Value"); // Fläche Segmente
        double U = wsRegel.OleProperty("Cells",bestR,21).OleProperty("Value"); // Fläche Auftrag

        for(long i=42;i<=letzteZeileQ;i++)
        {
            double dm = wsQuelle.OleProperty("Cells",i,17).OleProperty("Value");
            double l  = wsQuelle.OleProperty("Cells",i,20).OleProperty("Value");
            double gew= wsQuelle.OleProperty("Cells",i,29).OleProperty("Value");
            double flaecheAuftrag = wsQuelle.OleProperty("Cells",i,28).OleProperty("Value");

            std::wstring segTyp = wsRegel.OleProperty("Cells",bestR,20).OleProperty("Value").wstring();
            long maxSeg = wsRegel.OleProperty("Cells",bestR,19).OleProperty("Value");
            double maxGew = wsRegel.OleProperty("Cells",bestR,12).OleProperty("Value");

            long segNum=1;
            if(segTyp==L"f") {
                segNum=segNum_f;
                if(segNum_f<maxSeg) segNum_f++;
            } else if(segTyp==L"v") {
                if(laufGew_v+gew>maxGew) { segNum_v++; laufGew_v=gew; } else laufGew_v+=gew;
                if(segNum_v>maxSeg) segNum_v=maxSeg;
                segNum=segNum_v;
            }

            // StringGrid füllen
            rowSG++; sgSegment->RowCount = rowSG+1;
            sgSegment->Cells[0][rowSG] = IntToStr(i);
            sgSegment->Cells[1][rowSG] = FloatToStr(dm);
            sgSegment->Cells[2][rowSG] = FloatToStr(l);
            sgSegment->Cells[3][rowSG] = FloatToStr(gew);
            sgSegment->Cells[4][rowSG] = IntToStr(segNum);

            // Excel Zielblatt füllen
            wsZiel.OleProperty("Cells",rowSG+1,1).OlePropertySet("Value", i);
            wsZiel.OleProperty("Cells",rowSG+1,2).OlePropertySet("Value", dm);
            wsZiel.OleProperty("Cells",rowSG+1,3).OlePropertySet("Value", l);
            wsZiel.OleProperty("Cells",rowSG+1,4).OlePropertySet("Value", gew);
            wsZiel.OleProperty("Cells",rowSG+1,5).OlePropertySet("Value", segNum);

            // Flächenberechnung
            double flaecheJeSegment = U/S;
            double maxBelegbareFlaeche = flaecheJeSegment * segNum / 100; // analog VBA
            wsZiel.OleProperty("Cells",rowSG+1,6).OlePropertySet("Value", flaecheJeSegment);
            wsZiel.OleProperty("Cells",rowSG+1,7).OlePropertySet("Value", maxBelegbareFlaeche);
        }

        // Status AG setzen: verplant / nicht verteilt
        for(long i=42;i<=letzteZeileQ;i++)
        {
            Variant status = wsZiel.OleProperty("Range", wsZiel.OleProperty("Cells",2,1),
                                                      wsZiel.OleProperty("Cells",rowSG+1,1))
                                                .OleFunction("Find", i);
            if(!VarIsNull(status)) wsQuelle.OleProperty("Cells",i,33).OlePropertySet("Value","verplant");
            else wsQuelle.OleProperty("Cells",i,33).OlePropertySet("Value","nicht verteilt");
        }

        ShowMessage("Auftragsvorauswahl komplett abgeschlossen!");
        CoUninitialize();
    }
    catch(const Exception &e)
    {
        ShowMessage(e.Message); CoUninitialize();
    }
}
