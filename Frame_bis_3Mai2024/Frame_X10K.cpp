//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Frame_X10K.h"
#include "Unit1_DataBaseLayer.h"

#include "Utility.h"
#include "Unit00_Main.h"
#include "_ExternalVars.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "DBAccess"
#pragma link "MSAccess"
#pragma link "NxCustomGrid"
#pragma link "NxCustomGridControl"
#pragma link "NxDBGrid"
#pragma link "NxScrollControl"
#pragma link "NxColumns"
#pragma link "NxDBColumns"
#pragma link "PBNumEdit"
#pragma link "Gradient"
#pragma link "PJDropFiles"
#pragma resource "*.dfm"
#include "Neu_XML_TDBGrid.h"
#include <vector>
TFrameX10K *FrameX10K;

namespace RR4X10K
{
	static void Create_XMLFrame(){
		FormXML = NULL;
		Application->CreateForm(__classid(TFormXML), &FormXML);
	}
}

//---------------------------------------------------------------------------
__fastcall TFrameX10K::TFrameX10K(TComponent* Owner)
	: TFrame(Owner), FRattachm(new TFrameAttachmentManager(this))//, Form4XML(new TFormXML(this))   //RRR42
{
		QC1=NULL;                // Deklaration Tabellen Query
		QMA=NULL;                // Deklaration Messanschluss Query
		QTAP=NULL;               // Deklaration TAP Query
		QH=NULL;                 // Deklaration Header Query

		Initialized = false;       // Initialisierungs Bool auf false
		ReadOnly = true;         // Schreibgeschützt ?
		TBuBear->Visible = false; // Schaltfläche "Bearbeiten" false

	RowTable1 = -1; RowTable2 = -1; RowTable3 = -1;
	ColTable1 = -1; ColTable2 = -1; ColTable3 = -1;

	Col4RedTable1 = -1; Col4RedTable2 = -1; Col4RedTable3 = -1;

	RowEditTable1 = -1; RowEditTable2 = -1; RowEditTable3 = -1;
	ColEditTable1 = -1; ColEditTable2 = -1; ColEditTable3 = -1;

	prepare4table1GUI = false;
	prepare4table2GUI = false;
	prepare4table3GUI = false;
	xml4table1GUI = false;
	xml4table2GUI = false;
	xml4table3GUI = false;

	table1RememberOfXML.clear();
	table2RememberOfXML.clear();
	table3RememberOfXML.clear();

	XML_Name2DataBase_Name.clear();

	EditAccept = false;

    GeneratePairs4Header();
//FRattachm = new TFrameAttachmentManager(this);
}

//=============================================================================================================================================
__fastcall TFrameX10K::~TFrameX10K()
//=============================================================================================================================================
{
	//ShowMessage("~TFrameX10K()");
	//if(FRattachm)
	//	delete FRattachm;
}
//=============================================================================================================================================
void __fastcall TFrameX10K::Activate(int ID, String OwnerFormName)
//=============================================================================================================================================
{
		ShowFormXML();
		TBuBear->Visible = Form7->CheckPriv2("X10K",1,0);
		ReadOnly = !Form7->CheckPriv2("X10K",1,0);   //Prüfen ob OK oder NCS  //gibt true bei Berechtigung zurück

		FRattachm->AutoScaleXmax=1200; //bezieht:
		FRattachm->AutoScaleYmax=1000;

		Left=5; Top=Form3->PNxwinBottom->Top-Height-5;

		if(QC1==NULL)  {QC1=new TMSQuery(NULL);  table1->DataSource = DS; DS->DataSet=QC1;}              // Wenn der Zeiger QC1=0, soll eine neue Instanz von
		if(QMA==NULL)  {QMA=new TMSQuery(NULL);  table2->DataSource = DS1; DS1->DataSet=QMA;}   			// TMSQuery mit dem Namen QC1 erzeugt und mit der
		if(QTAP==NULL) {QTAP=new TMSQuery(NULL); table3->DataSource = DS2; DS2->DataSet=QTAP;}            // Datenquelle DS verbunden werden.
		if(QH==NULL)   {QH=new TMSQuery(NULL);   DS3->DataSet=QH;}


		Visible=true;                                                           // Über diese Zeile wird das Frame sichtbar gemacht
		ProtokolID=ID;                                                          // Die ID ist eine globale Variable welche dem int
																				// ProtokolID zugewiesen wird

		GRD->ColorBegin=GetGadientColor((TColor)COL_H_MAIN, COL_H, 30); 		// Farbschema wird dem Hintergrund TGradient
		GRD->ColorEnd  =Form3->GRDprot->ColorBegin;                             // zugewiesen


		if(!Initialized)                                                        // Ohne diese If Abfrage würden bei jedem Aufruf
	{																			// des Frames die Objekte neu initialisiert. Dies
																				// führt zu mehrfach Einträgen in den ComboBoxen

		QH=new TMSQuery(NULL);                                                  // Neue Instanz von TMSQuery wird erzeugt
		Initialized=(QH!=NULL);        											// Initialisierungs Bool wird auf true gesetzt sodass
																				// diese Methode bei erneuten Aufrufen mit der gleichen
																				// ID nicht mehr abgearbeitet wird.


		// Abfrage der Messgeräte aus der Hilfsmitteltabelle und setzen der Vorauswahle (Index = 1)
		if(Form1->SendSQLLine("SELECT * FROM dbo.Hilfsmittel WHERE Typ='Messmittel' AND Protokoll_XTYP='X10K' ORDER BY Beschreibung",QH))
		{while(!QH->Eof){ ComBoxMessg->Items->Add(QH->FieldByName("Beschreibung")->AsString+" # "+QH->FieldByName("WNummer")->AsString.Trim()); QH->Next();}}

		// Vorauswahl Messgeräte
		ComBoxMessg->ItemIndex = 1;

		// Vorauswahl Aufbau  (Index = 1)
		ComBoxAufb->ItemIndex = 1;

	}

	 ImportDataFromDB();
	 DragImportFile();
	 PracticalHeaderName(); //RRR42

}
//=============================================================================================================================================
	void __fastcall TFrameX10K::CheckReadOnly (bool ReadOnly)
//=============================================================================================================================================
{
	;
}

  /*

       	if (ReadOnly != false)
	{
		   CBKuMess		->	Enabled =0;
		   ComBoxMessg 	->	Enabled =0;
		   ComBoxAufb   ->	Enabled =0;
		   EditOrt      ->	Enabled =0;
		   NuEdTempU    ->	Enabled =0;
		   NuEdTempDfg  ->	Enabled =0;
		   NuEdFeuchte  ->	Enabled =0;

	  //	   DbNumDate->Color =((TColor)COL_X10K);
		   DbNumDate->Options>>coEditing;
	}
	else
	{
		   CBKuMess		->	Enabled =1;
		   ComBoxMessg 	->	Enabled =1;
		   ComBoxAufb   ->	Enabled =1;
		   EditOrt      ->	Enabled =1;
		   NuEdTempU    ->	Enabled =1;
		   NuEdTempDfg  ->	Enabled =1;
		   NuEdFeuchte  ->	Enabled =1;

		   DbNumDate->Color =clWindow;
		   DbNumDate->Options<<coEditing;
		   NxDBDateColumn3->Color =clWindow;
		   NxDBDateColumn3->Options<<coEditing;
	 }


    */
//=============================================================================================================================================
	void __fastcall TFrameX10K::CreateNewRecord     (int ID, int TestArt, TMSQuery *QTestArt, int markedASdestroyed)  // Kann verwendet werden um eine neue Zeile über Code zu generieren, hierbei wird  die Prod_Id übergeben
//=============================================================================================================================================
{
   if(ProtokolID<1) return;
 //-----------------------------------Neu---------------------------
 //String sql ="INSERT INTO X10K (PROT_ID,Utest,Uist,Frequenz,Cp,tan,TS,Test_Art) VALUES ("+IntToStr(ProtokolID)+",0,0,0,0,0,0,"+IntToStr(TestArt)+")";
//String sql ="INSERT INTO X10K (PROT_ID,Utest,Uist,Frequenz,tan,TS,Test_Art,markedASdestroyed, TempU, TempDfg, Feuchte, Kommentar ) VALUES ("+IntToStr(ProtokolID) + ",0,0,0,0,0,"+IntToStr(TestArt) + "," + IntToStr(markedASdestroyed)+ "," + NuEdTempU->Text + "," + NuEdTempDfg->Text + "," + NuEdFeuchte->Text + "," + " " + ")";
   String sql ="INSERT INTO X10K (PROT_ID,Utest,Uist,Frequenz,tan,TS, Test_Art, markedASdestroyed, Ix,CompMeas1,CompMeas2,Cp,PF,Pt,P10KV,Qt,St,Zabs,Zphase,Rp,Rs,Q, TempU, TempDfg, Feuchte, Kommentar ) VALUES ("+IntToStr(ProtokolID) + ",0,0,0,0,0,"+IntToStr(TestArt) + "," + IntToStr(markedASdestroyed)+ "," + "0,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0" +  NuEdTempU->Text + "," + NuEdTempDfg->Text + "," + NuEdFeuchte->Text + "," + " " + ")";

   Form1->SendSQLLine(sql,QTestArt);

   Form1->SendSQLLine("SELECT TOP(200) * FROM X10k WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt)+"",QTestArt);
   if(TestArt == 1){
		xml4table1GUI = false;
	}
   else if(TestArt == 2){
		xml4table2GUI = false;
	}
   else if(TestArt == 3){
		xml4table3GUI = false;
	}


 }
//=============================================================================================================================================
	void __fastcall TFrameX10K::ImportDataFromDB    (void)     // Importiert den Datensatz aus der MS Tabelle. Die ID Nummer ist durch die Variable
//=============================================================================================================================================
{

	 /*     Kundenmessung 	= UNIB1                 ||      Test_Art:	C1  = 1         ||      Messgerät
			Read Only       = UNIB2                 ||                  MA  = 2         ||
			Messgerät       = UININI1               ||                  TAP = 3
			Aufbau          = UNIINT2
			Ort             = UNIT1
			Umgebungstemp   = UNIF1
			Dfg.Temp        = UNIF2
			Feuchte         = UNIF3     */


	  if(Form1->SendSQLLine("SELECT * FROM dbo.PROTOKOL WHERE ID="+IntToStr(ProtokolID),QH))
	  {
			CBKuMess 	->Checked	= QH->FieldByName("UNIB1")->AsBoolean;
			ComBoxAufb  ->ItemIndex = QH->FieldByName("UNIINT2")->AsInteger;
			EditOrt 	->Text      = QH->FieldByName("UNIT1")->AsAnsiString;
			NuEdTempU	->Text 		= QH->FieldByName("UNIF1")->AsFloat;
			NuEdTempDfg ->Text 		= QH->FieldByName("UNIF2")->AsFloat;
			NuEdFeuchte ->Text 		= QH->FieldByName("UNIF3")->AsFloat;
			PnlHeader->Enabled      = !(QH->FieldByName("UNIB2")->AsBoolean);      //Überprüfung ob Schreibgeschützt


			if (QH->FieldByName("UNIB1")->AsBoolean) ComBoxMessg -> ItemIndex = -1;      // Wenn Kundenmessung dann Messgerät nicht bekannt
		   // #TODO1  Hier muss noch überlegt werden wie und auf welche Art und Weise das MEssgerät genersich abgespeichert werden soll
	  }


		 //Einlese der Tabelle 1 (C1)
		if(ProtokolID>0 && Form1->SendSQLLine("SELECT * FROM dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =1 AND markedASdestroyed != 1 ",QC1));

		//Einlese der Tabelle 1 (MA)
		if(ProtokolID>0 && Form1->SendSQLLine("SELECT * FROM dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =2 AND markedASdestroyed != 1",QMA));

		//Einlese der Tabelle 1 (TAP)
		if(ProtokolID>0 && Form1->SendSQLLine("SELECT * FROM dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =3 AND markedASdestroyed != 1",QTAP));



}




//=============================================================================================================================================
	void __fastcall TFrameX10K::ExportDataToDB      (void)     // Exportiert den Datensatz aus der GUI in die MS Tabelle. Die ID Nummer ist durch
//=============================================================================================================================================
 {
	;
}
//=============================================================================================================================================
void __fastcall TFrameX10K::CBKuMessClick(TObject *Sender)       // Änderung der Vorauswahl bei Kudenmessung
//=============================================================================================================================================
{
	  if (CBKuMess->Checked) { ComBoxMessg->ItemIndex = 0; ComBoxAufb->ItemIndex = 4;}
	  else                   { ComBoxMessg->ItemIndex = 1; ComBoxAufb->ItemIndex = 1;}
}
//=============================================================================================================================================
	void __fastcall TFrameX10K::SetChangeFlag       (void)     //  Wird sofort aktiv wenn geklickt oder geändert wird.
//=============================================================================================================================================
{
	;
}

//=============================================================================================================================================
	void __fastcall TFrameX10K::CheckAllFieldsSet   (void)     // Alle Pflichtfelder gesetzt ?
//=============================================================================================================================================
{
	;
}


//=============================================================================================================================================
	void __fastcall TFrameX10K::BuildDefaultTable   (void)
//=============================================================================================================================================
{
	;
}

	//	TBuBear->Visible = Form7->CheckPriv2("X10K",1,0); //Check Priv für Bearbeiten Button

/*
	//table1          -> Color = clWindow;
	table2			-> 	Enabled =1;
	table3			-> 	Enabled =1;



	  DbNumDate->Color =clWindow;
	  NxDBDateColumn3->Color =clWindow;

	  NxDBDateColumn3->Options<<coEditing;




		// ERstellung von Protokollen auch möglich wenn Statusmeldung = ausgeliefert
                      */

//---------------------------------------------------------------------------
//=============================================================================================================================================
void __fastcall TFrameX10K::table4DblClick(TObject *Sender)
//=============================================================================================================================================
{
 ;
}
//=============================================================================================================================================
void __fastcall TFrameX10K::SpBtnC1Click(TObject *Sender)   { table1->DataSource = DS; DS->DataSet = QC1; CreateNewRecord( ProtokolID, 1, QC1, 0) ;}
void __fastcall TFrameX10K::SpBtnMAClick(TObject *Sender)   { table2->DataSource = DS1; DS1->DataSet = QMA; CreateNewRecord( ProtokolID, 2, QMA, 0) ;}
void __fastcall TFrameX10K::SpBtnTAPClick(TObject *Sender)  { table3->DataSource = DS2; DS2->DataSet = QTAP; CreateNewRecord( ProtokolID, 3, QTAP, 0);}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::IchBin()
{
	ShowMessage("Ich bin der Frame X10K");
}

void __fastcall TFrameX10K::ShowFormXML()
{
	;
}

void __fastcall TFrameX10K::CreateAndShowForm()
{
	RR4X10K::Create_XMLFrame();
	if(FormXML)
	{
		//FormXML->IchBin();
		FormXML->pFX10K = this;
		FormXML->ShowModal();
	}
	else
		ShowMessage("Problem XML-Manager auf zu bauen");

}
void __fastcall TFrameX10K::spbToXMLClick(TObject *Sender)
{
	CreateAndShowForm();
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table1OnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)

{

    if ((ARow >= 0) && (!xml4table1GUI)) { // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
		FontStyle = TFontStyles();
		//TextColor = clSkyBlue;
	}

	if((ARow >= 0) && (xml4table1GUI)){
		FontStyle = TFontStyles();
		TextColor = clWebGold; //clCream;
		return;
	}
	/*FontStyle = TFontStyles();

	if((ARow >= 0) && (xml4table1GUI)){

		TextColor = clWebGold; //clCream;
		FontStyle = TFontStyles();//table1->RefreshRow(ARow);
		return;
	}

	if ((ARow >= 0) && (ARow != RowTable1)) {  //(RowTable1 > -1) &&
		TextColor = clBlack; //RRR42
		FontStyle = TFontStyles() << fsBold; // >> fsBold;
		//table1->RefreshRow(ARow);
		return;
	}
	*/
	/*
	//Fall wenn angeklickt um zu löschen
	else if((prepare4table1GUI) && (RowTable1 > -1) && (ARow == RowTable1) ){
		FontStyle = TFontStyles(); // << fsBold;
		TextColor = clOlive; //clFuchsia;
			if ((prepare4table1GUI) && (Col4RedTable1 > -1) && (RowTable1 > -1) && (ARow == RowTable1) && (ACol == Col4RedTable1)) {  // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
				FontStyle = TFontStyles(); // << fsBold;
				TextColor = clRed;
		}
	} */
	/*else if ((ARow >= 0) && (ARow != RowTable1)) {  //(RowTable1 > -1) &&
		FontStyle = TFontStyles(); // >> fsBold;
	} */

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2OnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)

{
	if ((ARow >= 0) && (!xml4table1GUI)) { // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
		FontStyle = TFontStyles();
		//TextColor = clSkyBlue;
	}

	if((ARow >= 0) && (xml4table1GUI)){
		FontStyle = TFontStyles();
		TextColor = clWebGold; //clCream;
		return;
	}

	//if(xml4table2GUI)
	//	TextColor = clCream;
	//Fall wenn angeklickt um zu löschen
	if((prepare4table2GUI) && (RowTable2 > -1) && (ARow == RowTable2) )
		TextColor = clFuchsia;

	if ((prepare4table2GUI) && (Col4RedTable2 > -1) && (RowTable2 > -1) && (ARow == RowTable2) && (ACol == Col4RedTable2))  // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
		TextColor = clRed;
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)

{
	if (ARow >= 0) { // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
		FontStyle = TFontStyles();
		//TextColor = clSkyBlue;
	}
	if(xml4table3GUI)
		TextColor = clCream;
	//Fall wenn angeklickt um zu löschen
	if((prepare4table3GUI) && (RowTable3 > -1) && (ARow == RowTable3) )
		TextColor = clFuchsia;

	if ((prepare4table3GUI) && (Col4RedTable3 > -1) && (RowTable3 > -1) && (ARow == RowTable3) && (ACol == Col4RedTable3))  // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
		TextColor = clRed;

}
//---------------------------------------------------------------------------
void __fastcall TFrameX10K::table1OnCellClick(TObject *Sender, int ACol, int ARow)

{
	int Spalte = -1;
	//table1->Columns->Item[8]->Field->SetFieldType(ftInteger); //DataType = ftInteger;
	//Spalte suchen markedASdestroyed der löschmarker steht
	for (int i = 0; i < table1->Columns->Count ; ++i)
	{
		++Spalte;
		TColumnHeader* header = table1->Columns->Item[i]->Header;
		AnsiString wstr = header->Caption;
		if(wstr == L"ID"){


			//Zelle sich so merken
			ColTable1 = Spalte; RowTable1 = ARow; Col4RedTable1 = Spalte;
			break;
		}

	}
	table1->RefreshRow(ARow);

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2OnCellClick(TObject *Sender, int ACol, int ARow)

{
	//QMA
	int Spalte = -1;

	//Spalte suchen markedASdestroyed der löschmarker steht
	for (int i = 0; i < table2->Columns->Count ; ++i)
	{
		++Spalte;
		TColumnHeader* header = table2->Columns->Item[i]->Header;
		AnsiString wstr = header->Caption;
		if(wstr == L"ID"){


			//Zelle sich so merken
			ColTable2 = Spalte; RowTable2 = ARow; Col4RedTable2 = Spalte;
			break;
		}

	 }

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnCellClick(TObject *Sender, int ACol, int ARow)

{
	//QTAP
	int Spalte = -1;

	//Spalte suchen markedASdestroyed der löschmarker steht
	for (int i = 0; i < table3->Columns->Count ; ++i)
	{
		++Spalte;
		TColumnHeader* header = table3->Columns->Item[i]->Header;
		AnsiString wstr = header->Caption;
		if(wstr == L"ID"){


			//Zelle sich so merken
			ColTable3 = Spalte; RowTable3 = ARow; Col4RedTable3 = Spalte;
			break;
		}

	 }
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::SpBtnC1Click_Delete(TObject *Sender)
{
	if(!prepare4table1GUI)
	{
		ShowMessage("Bitte erst die Zeile mit dem kleinen Schalter über diesen rot markieren. Erst dann ist die Zeile für den Löschvorgang.");
	}
	xml4table1GUI = false;
	MarkAsDelete4Table(1);
	prepare4table1GUI = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::SpBtnMAClick_Delete(TObject *Sender)
{
	xml4table2GUI = false;
	MarkAsDelete4Table(2);
    prepare4table2GUI = false;
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::SpBtnTAPClick_Delete(TObject *Sender)
{
	xml4table3GUI = false;
	MarkAsDelete4Table(3);
    prepare4table3GUI = false;

}
//---------------------------------------------------------------------------

//String sql ="INSERT INTO X10K (PROT_ID,Utest,Uist,Frequenz,Cp,tan,TS,Test_Art) VALUES ("+IntToStr(ProtokolID)+",0,0,0,0,0,0,"+IntToStr(TestArt)+")";

void __fastcall TFrameX10K::MarkAsDelete4Table(int TestArt)
{
	//QC1->Connection = MSCon;
	//DS->DataSet = QC1;

	String marker4Destroy = L"true";

	if(TestArt == 1)
	{
		if(!prepare4table1GUI){
			ShowMessage("Bitte erst in der Tabelle C1 eine Zeile zum löschen markieren");
			return;
		}

		//ID zu erhalten
		String cellValue = table1->GetCellValue(ColTable1, RowTable1);
		String sql = "UPDATE [MDE].[dbo].[X10K] SET markedASdestroyed = '" + marker4Destroy + "' WHERE ID = " + cellValue;
		Form1->SendSQLLine(sql,QC1);
		Form1->SendSQLLine("SELECT TOP(200) * FROM X10k WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt)+"",QC1);
		table1->Repaint();
		prepare4table1GUI = false;
	}
	else if(TestArt == 2)
	{
		if(!prepare4table2GUI){
			ShowMessage("Bitte erst in der Tabelle MA eine Zeile zum löschen markieren");
			return;
		}

		//ID zu erhalten
		String cellValue = table2->GetCellValue(ColTable2, RowTable2);
		String sql = "UPDATE [MDE].[dbo].[X10K] SET markedASdestroyed = '" + marker4Destroy + "' WHERE ID = " + cellValue;
		Form1->SendSQLLine(sql,QMA);
		Form1->SendSQLLine("SELECT TOP(200) * FROM X10k WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt)+"",QMA);
		table2->Repaint();
		prepare4table2GUI = false;
	}
	else if(TestArt == 3)
	{
		if(!prepare4table3GUI){
			ShowMessage("Bitte erst in der Tabelle TAP eine Zeile zum löschen markieren");
			return;
		}

		//ID zu erhalten
		String cellValue = table3->GetCellValue(ColTable3, RowTable3);
		String sql = "UPDATE [MDE].[dbo].[X10K] SET markedASdestroyed = '" + marker4Destroy + "' WHERE ID = " + cellValue;
		Form1->SendSQLLine(sql,QTAP);
		Form1->SendSQLLine("SELECT TOP(200) * FROM X10k WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt)+"",QTAP);
		table3->Repaint();
		prepare4table3GUI = false;
	}

}

//Erstmal nur Kennzeichnen ob gelöscht
void __fastcall TFrameX10K::SpBtnC1Click_Prepare(TObject *Sender)
{
	prepare4table1GUI = !prepare4table1GUI; //true;
		table1->RefreshRow(table1->SelectedRow);
	//Eventuell RefreshRow machen.
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::SpBtnMAClick_Prepare(TObject *Sender)
{
	prepare4table2GUI = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::SpBtnTAPClick_Prepare(TObject *Sender)
{
	prepare4table3GUI = true;
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table1OnSelectCell(TObject *Sender, int ACol, int ARow)

{
	if (ARow == table1->SelectedRow)  //Problem nur eine Zelle zun zeichnen
		table1->RefreshRow(table1->SelectedRow);
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::Table1OnCellColoring(TObject *Sender, int ACol, int ARow,
          TColor &CellColor, TColor &GridColor, TCellState CellState)
{
	if( !prepare4table1GUI && (ARow == table1->SelectedRow) )
	{
		CellColor = clSkyBlue;
			//table1->RefreshRow(table1->SelectedRow); (7)
	}
	else if( prepare4table1GUI && (ARow == table1->SelectedRow) )
	{
		CellColor = clRed;
    }
}
//---------------------------------------------------------------------------

//Problem String in float-Darstellung
void __fastcall TFrameX10K::PracticalValues(int tableNumber )
{
    return; //RRR42
	TField* Field;
	UnicodeString unistr;

	if(tableNumber == 1)
	{
		for (int rows = 0; rows < table1->RowCount; ++rows)
		{

			for (int col = 0; col < table1->Columns->Count ; ++col)
			{
				//Hier wenn der Name des Feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
				Field = table1->Columns->Item[col]->Field;
				//Feldname wie im DataSet !!!
				unistr = Field->FieldName;
				if(unistr == U"Vx")
					table1->Columns->Item[col]->Visible = false;
				TColumnHeader* header = table1->Columns->Item[col]->Header;
				AnsiString wstr = header->Caption;
				{
					header->Caption = "Nix " + IntToStr(col);
				}
			}
		}
	}
}

void __fastcall TFrameX10K::PracticalHeaderName()
{
	TField* Field;
	UnicodeString unistr;
	for (int i = 0; i < table1->Columns->Count ; ++i)
	{
		//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
		Field = table1->Columns->Item[i]->Field;
		//Feldname wie im DataSet !!!
		unistr = Field->FieldName;
		if( (unistr == U"markedASdestroyed")||(unistr == U"ID") )
			table1->Columns->Item[i]->Visible = false;
		TColumnHeader* header = table1->Columns->Item[i]->Header;
		AnsiString wstr = header->Caption;
		{
			//Hier lookup für praktiche Namen   RRR42
				//header->Caption = "Nix " + IntToStr(i);
		}


		if(!ReadOnly)
		{
			table1->Columns->Item[i]->Options << coEditing;   //RRR42 schauen ob wieder rein
			//table1->Columns->Item[i]->Options << coCanClick;  RRR42   // Für anklickbare Spaltenüberschriften
			//table1->Columns->Item[i]->Options << coCanInput;  RRR42 schauen ob wieder rein
			//Damit geht es
			//table1->Columns->Item[i]->Editing; // ->Options << coEditing;
			table1->Columns->Item[i]->Enabled = true;



		}

	}
}

void __fastcall TFrameX10K::table1OnBeforeEdit(TObject *Sender, int ACol, int ARow,
		  bool &Accept)
{
	Accept = !ReadOnly;
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table1OnEditAccept(TObject *Sender, int ACol, int ARow,
          WideString Value, bool &Accept)
{
	TField* Field;
	UnicodeString unistr = U"";
	if(Value == L"")
	{
		Accept = false;
		EditAccept = false;
	}
	else
	{   //"Update Nordwind.dbo.rrNordwind set TEXT = 'Unit5:TEXT__1' where ID = 1"
		Accept = true;  // :Accept = !true;  :So rein schreiben aber nicht akzeptiert nach Enter
		EditAccept = true; //wird evntuell später

		/*
		//ShowMessage("1");
		//Die richtige ID
		String cellValue = table1->GetCellValue(ACol, ARow);
		//Wert der aktuelle editierte Zelle
		String cellValueEdit = table1->GetCellValue(ACol, ARow);
		//ShowMessage("2");
		Field = table1->Columns->Item[ACol]->Field;
		//Feldname wie im DataSet !!!
		String colName = Field->FieldName;
		//ShowMessage("3");
		return;   //RRR42         Nur Test
		String sql = "UPDATE [MDE].[dbo].[X10K] SET" + colName + " = '" + cellValueEdit + "' WHERE ID = " + cellValue;
		Form1->SendSQLLine(sql,QC1);

		//Erstmal nicht neu aufbauen
		//Form1->SendSQLLine("SELECT TOP(200) * FROM X10k WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt)+"",QC1);
	*/
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table1OnDblClick(TObject *Sender)
{
	table1->ReadOnly = false;
	table1RememberOfXML.clear();
	table2RememberOfXML.clear();
	table3RememberOfXML.clear();

	if(ColTable1 > -1)
		table1->Columns->Item[ColTable1]->Options << coEditing;
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::IniTable1RememberOfXML()
{
	for( int i = 0; i < table1->RowCount; ++i)
	{
		std::pair<int, int> data(1, 0);
		table1RememberOfXML.push_back(data);
	}
}

void __fastcall TFrameX10K::IniTable2RememberOfXML()
{
	for( int i = 0; i < table2->RowCount; ++i)
	{
		std::pair<int, int> data(1, 0);
		table2RememberOfXML.push_back(data);
	}
}

void __fastcall TFrameX10K::IniTable3RememberOfXML()
{
	for( int i = 0; i < table3->RowCount; ++i)
	{
		std::pair<int, int> data(1, 0);
		table3RememberOfXML.push_back(data);
	}
}

//=============================================================================================================================================
void __fastcall TFrameX10K::DragImportFile()    //Aufruf in:  TFrameMEMO::Activate(int ID, String OwnerFormName
//=============================================================================================================================================
{	// Um das Attachmentframe nutzen zu können muss der DboName sowie der DboKeyName übergeben werden
	String YYYY=PDS.TIMESTAMP.FormatString("yyyy");
	FRattachm->DboName="PROT_X10K_"+YYYY; FRattachm->DboKeyName="ID";	//Hier PROT_X10K_2024
	FRattachm->LoadList(IntToStr(ProtokolID),true); //void __fastcall TFrameMEMO::Activate(int ID, String OwnerFormName) //aus: FR_Memo ->Activate(PDS.PK_NR,"");
	FRattachm->AfterResize();
}

//Sucht sich die Spalte mit ID wenn ColEditTable1 aber -1 dann nicht gefunden
//=============================================================================================================================================
void __fastcall TFrameX10K::FindIDColumn()
//=============================================================================================================================================
{
	ColEditTable1 = -1;
	TField* Field;
	int col = -1;
	for(int i = 0; i < table1->Columns->Count; ++i)
	{
		++col;
		Field = table1->Columns->Item[col]->Field;
		//Feldname wie im DataSet !!!
		String colName = Field->FieldName;
		if(colName == "ID")
		{
			ColEditTable1 = col;
			break;
		}
	}
}

//=============================================================================================================================================
void __fastcall TFrameX10K::CompleteXMLAndSend(int TestArt)
//=============================================================================================================================================
{
	//hier nun schauen was nicht belegt ist
	//Ich kenne die Anzahl an Spalten
	// wenn 19 dann return nichts auffüllen

	//zunächst sei auch hier kein Wert vorhanden
	//RRR42

	int countTabSpalten4DB = 26;
	int markedASdestroyed = 0;

	//Frage kann ich das dynamisch erfragen // RRR42
	std::wstring ersterSQL_AusdruckAnteil = L"(";
	for( int i = 0; i < countTabSpalten4DB; ++i)
	{
		ersterSQL_AusdruckAnteil += GetDataBaseNameOverIndex(i) + L",";
	}

	ersterSQL_AusdruckAnteil += L") VALUES (";


//GetPracticalName();
//hier muß der Stzring aufgebaut werden.
		//String sql = "INSERT INTO X10K" + ersterSQL_AusdruckAnteil + IntToStr(ProtokolID) + ",0,0,0,0,0," + IntToStr(TestArt) + "," + IntToStr(markedASdestroyed) + "," + "0,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0," +  NuEdTempU->Text + "," + NuEdTempDfg->Text + "," + NuEdFeuchte->Text + "," + " " + ")";
//String sql ="INSERT INTO X10K (PROT_ID,Utest,Uist,Frequenz,tan,TS, Test_Art, markedASdestroyed, Ix,CompMeas1,CompMeas2,Cp,PF,Pt,P10KV,Qt,St,Zabs,Zphase,Rp,Rs,Q, TempU, TempDfg, Feuchte, Kommentar ) VALUES ("+IntToStr(ProtokolID) + ",0,0,0,0,0,"+IntToStr(TestArt) + "," + IntToStr(markedASdestroyed)+ "," + "0,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0" +  NuEdTempU->Text + "," + NuEdTempDfg->Text + "," + NuEdFeuchte->Text + "," + " " + ")";


   return;  // RRR42 erstmal noch nicht
   //String sql ="INSERT INTO X10K (PROT_ID,Utest,Uist,Frequenz,tan,TS,Test_Art,markedASdestroyed, TempU, TempDfg, Feuchte, Kommentar ) VALUES ("+IntToStr(ProtokolID) + ",0,0,0,0,0,"+IntToStr(TestArt) + "," + IntToStr(markedASdestroyed)+ "," + NuEdTempU->Text + "," + NuEdTempDfg->Text + "," + NuEdFeuchte->Text + "," + " " + ")";
   //Form1->SendSQLLine(sql,QTestArt);

   //Form1->SendSQLLine("SELECT TOP(200) * FROM X10k WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt)+"",QTestArt);



	//;
}

std::wstring __fastcall TFrameX10K::GetDataBaseNameOverIndex(int index)
{
	//std::vector < std::pair < std::wstring, std::wstring > > XML_Name2DataBase_Name;
	std::pair < std::wstring, std::wstring > data = XML_Name2DataBase_Name[index];
	return data.second;
}



//Einmal etwas GetName wo mehr Zeit im Format4XML  Vom XML zum DB-Namen !!!
//=============================================================================================================================================
void __fastcall TFrameX10K::GeneratePairs4Header()
//=============================================================================================================================================
{
	std::pair<std::wstring , std::wstring> data(L"PROT_ID", L"PROT_ID");
	XML_Name2DataBase_Name.push_back(data);
	std::pair<std::wstring , std::wstring> data1(L"V0", L"Utest");
	XML_Name2DataBase_Name.push_back(data1);
	std::pair<std::wstring , std::wstring> data2(L"Vx", L"Uist");
	XML_Name2DataBase_Name.push_back(data2);
	std::pair<std::wstring , std::wstring> data3(L"f", L"Frequenz");
	XML_Name2DataBase_Name.push_back(data3);
	std::pair<std::wstring , std::wstring> data4(L"DF", L"tan");
	XML_Name2DataBase_Name.push_back(data4);
	std::pair<std::wstring , std::wstring> data5(L"TS", L"TS");
	XML_Name2DataBase_Name.push_back(data5);
	std::pair<std::wstring , std::wstring> data6(L"Test_Art", L"Test_Art");
	XML_Name2DataBase_Name.push_back(data6);
	std::pair<std::wstring , std::wstring> data7(L"markedASdestroyed", L"markedASdestroyed");
	XML_Name2DataBase_Name.push_back(data7);
	std::pair<std::wstring , std::wstring> data8(L"Ix", L"Ix");
	XML_Name2DataBase_Name.push_back(data8);
	std::pair<std::wstring , std::wstring> data9(L"CompMeas1", L"CompMeas1");
	XML_Name2DataBase_Name.push_back(data9);
	std::pair<std::wstring , std::wstring> data10(L"CompMeas2", L"CompMeas2");
	XML_Name2DataBase_Name.push_back(data10);
	std::pair<std::wstring , std::wstring> data11(L"Cp", L"Cp");
	XML_Name2DataBase_Name.push_back(data11);
	std::pair<std::wstring , std::wstring> data12(L"PF", L"PF");
	XML_Name2DataBase_Name.push_back(data12);
	std::pair<std::wstring , std::wstring> data13(L"Pt", L"Pt");
	XML_Name2DataBase_Name.push_back(data13);
	std::pair<std::wstring , std::wstring> data14(L"P10KV", L"P10KV");
	XML_Name2DataBase_Name.push_back(data14);
	std::pair<std::wstring , std::wstring> data15(L"Qt", L"Qt");
	XML_Name2DataBase_Name.push_back(data15);
	std::pair<std::wstring , std::wstring> data16(L"St", L"St");
	XML_Name2DataBase_Name.push_back(data16);
	std::pair<std::wstring , std::wstring> data17(L"Zabs", L"Zabs");
	XML_Name2DataBase_Name.push_back(data17);
	std::pair<std::wstring , std::wstring> data18(L"Zphase", L"Zphase");
	XML_Name2DataBase_Name.push_back(data18);
	std::pair<std::wstring , std::wstring> data19(L"Rp", L"Rp");
	XML_Name2DataBase_Name.push_back(data19);
	std::pair<std::wstring , std::wstring> data20(L"Rs", L"Rs");
	XML_Name2DataBase_Name.push_back(data20);
	std::pair<std::wstring , std::wstring> data21(L"Q", L"Q");
	XML_Name2DataBase_Name.push_back(data21);
	std::pair<std::wstring , std::wstring> data22(L"TempU", L"TempU");
	XML_Name2DataBase_Name.push_back(data22);
	std::pair<std::wstring , std::wstring> data23(L"TempDfg", L"TempDfg");
	XML_Name2DataBase_Name.push_back(data23);
	std::pair<std::wstring , std::wstring> data24(L"Feuchte", L"Feuchte");
	XML_Name2DataBase_Name.push_back(data24);
	std::pair<std::wstring , std::wstring> data25(L"Kommentar", L"Kommentar");
	XML_Name2DataBase_Name.push_back(data25);

}

//=============================================================================================================================================
void __fastcall TFrameX10K::FTest4()
//=============================================================================================================================================
{
	;
}

//=============================================================================================================================================
void __fastcall TFrameX10K::FTest5()
//=============================================================================================================================================
{
	;
}

//=============================================================================================================================================
void __fastcall TFrameX10K::FTest6()
//=============================================================================================================================================
{
	;
}

//um von der aktuellen RecNo den header des Grid zu bekommen
std::wstring __fastcall TFrameX10K::GetPracticalName(std::wstring xml_Name)
{
	std::wstring dummy = L"";
	for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
			XML_Name2DataBase_Name.begin(); it != XML_Name2DataBase_Name.end(); ++it)
	{
		std::pair<std::wstring, std::wstring> compare = *it;
		if(compare.first == xml_Name)
		{
			return compare.second;
			break;
		}
	}
	return dummy; //wenn nicht OK

}


//um von der aktuellen RecNo bzw Row den wert bekommen
std::wstring __fastcall TFrameX10K::GetDataBaseName(std::wstring db_Name)
{
	std::wstring dummy = L"";
	for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
			XML_Name2DataBase_Name.begin(); it != XML_Name2DataBase_Name.end(); ++it)
	{
		std::pair<std::wstring, std::wstring> compare = *it;
		if(compare.second == db_Name)
		{
			return compare.first;
			break;
		}
	}
	return dummy; //wenn nicht OK
}


void __fastcall TFrameX10K::table1OnEditExit(TObject *Sender)
{
	//ShowMessage("table1OnEditExit:");
}
//---------------------------------------------------------------------------

//Achtung geht nur wenn nicht im xml weil ja erstmal die ID fehlt
void __fastcall TFrameX10K::table1OnEndEditing(TObject *Sender, int ACol, int ARow,
		  WideString &Value, bool &Accept)
{
    TField* Field;
	UnicodeString unistr = U"";
	if(Value == L"")
	{
		Accept = false;
		EditAccept = false;
		return;
	}
	else
	{   //"Update Nordwind.dbo.rrNordwind set TEXT = 'Unit5:TEXT__1' where ID = 1"
		Accept = true;  // :Accept = !true;  :So rein schreiben aber nicht akzeptiert nach Enter
		EditAccept = true; //wird evntuell später
		RowEditTable1 = ARow;
		//Suchen der richtigen Spalte von ID hier etwas irritierend ColEditTable1 genannt
		ColEditTable1 = -1;
		FindIDColumn();


		//ShowMessage("1");
		//Die richtige ID
		String cellIDValue = table1->GetCellValue(ColEditTable1, RowEditTable1);										   //
		ShowMessage("table1OnEndEditing: Die Zeile lautet : " + IntToStr(RowEditTable1) + " Die Spalte lautet : " + IntToStr(ColEditTable1) + "und der wert der ID lautet: " + cellIDValue );

		//Wert der aktuelle editierte Zelle
		String cellValueEdit = table1->GetCellValue(ACol, ARow);
		//ShowMessage("2");
		Field = table1->Columns->Item[ACol]->Field;
		//Feldname wie im DataSet !!!
		String colName = Field->FieldName;
		//ShowMessage("3");
		return;   //RRR42         Nur Test
		String sql = "UPDATE [MDE].[dbo].[X10K] SET" + colName + " = '" + cellValueEdit + "' WHERE ID = " + cellIDValue;
		Form1->SendSQLLine(sql,QC1);

		//Erstmal nicht neu aufbauen
		//Form1->SendSQLLine("SELECT TOP(200) * FROM X10k WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt)+"",QC1);
	}


	//String cellValue = table1->GetCellValue(ACol, ARow);
	//ShowMessage("table1OnEndEditing: Die Zeile lautet : " + IntToStr(ARow) + " Die Spalte lautet : " + IntToStr(ACol) + "und der wert lautet: " + cellValue );

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table1OnAfterEdit(TObject *Sender, int ACol, int ARow,
		  WideString Value)
{
	//ShowMessage("table1OnAfterEdit: Die Zeile lautet : " + IntToStr(ARow) + " Die Spalte lautet : " + IntToStr(ACol) );

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table1RecZurDBOnClick(TObject *Sender)
{
	CompleteXMLAndSend(1);
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2RecZurDBOnClick(TObject *Sender)
{
	CompleteXMLAndSend(2);
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3RecZurDBOnClick(TObject *Sender)
{
	CompleteXMLAndSend(3);
}
//---------------------------------------------------------------------------

