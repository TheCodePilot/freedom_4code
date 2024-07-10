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
#pragma link "Frame_AttachmentManager"
#pragma resource "*.dfm"
#include "Neu_XML_TDBGrid.h"
#include <vector>
TFrameX10K *FrameX10K;

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
namespace RR4X10K
{
	static void Create_XMLFrame()
	{
		FormXML = NULL;
		Application->CreateForm(__classid(TFormXML), &FormXML);
	}

	static	std::wstring ExtractBefore(const std::wstring& input, wchar_t* delimiter)
	{
		// Suchen nach dem Trennzeichen
		size_t delimiterPos = input.find(delimiter);

		// Überprüfen, ob das Trennzeichen gefunden wurde
		if (delimiterPos != std::wstring::npos)
		{
			// Extrahieren der Zeichen vor dem Trennzeichen
			return input.substr(0, delimiterPos);
		}
		else
		{
			// Wenn das Trennzeichen nicht gefunden wurde, gib die gesamte Eingabe zurück
			return input;
		}
	}

    // Funktion zur Konvertierung von std::wstring in double
	static double WstringToDouble(const std::wstring& str)
	{
		std::wstringstream wss(str);
		double result;
		wss >> result;
		if (wss.fail()) {
			//std::wcerr << L"Konvertierungsfehler bei double: " << str << std::endl;
			return -1.0; // oder eine andere geeignete Fehlerbehandlung
		}
		return result;
	}
}

//---------------------------------------------------------------------------
__fastcall TFrameX10K::TFrameX10K(TComponent* Owner)
	: TFrame(Owner)	//, FRattachm(new TFrameAttachmentManager(this))//, Form4XML(new TFormXML(this))   //RRR42
{

	ustrXMLFile = U"";
	
	QC1=NULL;                // Deklaration Tabellen Query
	QMA=NULL;                // Deklaration Messanschluss Query
	QTAP=NULL;               // Deklaration TAP Query
	QH=NULL;                 // Deklaration Header Query

	Initialized = false;       // Initialisierungs Bool auf false
	ReadOnly = true;         // Schreibgeschützt ?

	RowTable1 = -1; RowTable2 = -1; RowTable3 = -1;
	ColTable1 = -1; ColTable2 = -1; ColTable3 = -1;

	Col4RedTable1 = -1; Col4RedTable2 = -1; Col4RedTable3 = -1;

	RowEditTable1 = -1; RowEditTable2 = -1; RowEditTable3 = -1;

	RowSelectTable1 = -1; RowSelectTable2 = -1; RowSelectTable3 = -1;
	ColSelectTable1 = -1; ColSelectTable2 = -1; ColSelectTable3 = -1;


	xml4table1GUI = false;
	xml4table2GUI = false;
	xml4table3GUI = false;

	table1RememberOfXML.clear();
	table2RememberOfXML.clear();
	table3RememberOfXML.clear();

	XML_Name2DataBase_Name.clear();
	GeneratePairs4Header();
	InitTheHeader();

}

//=============================================================================================================================================
__fastcall TFrameX10K::~TFrameX10K()
//=============================================================================================================================================
{
	if(QC1)
		delete QC1;
	QTAP = NULL;

	if(QTAP)
		delete QTAP;
	QTAP = NULL;

	if(QMA)
		delete QMA;
	QMA = NULL;

	if(QH)
		delete QH;
	QH = NULL;
}
//=============================================================================================================================================
void __fastcall TFrameX10K::Activate(int ID, String OwnerFormName)
//=============================================================================================================================================
{
	ProtokolID = ID;  														 // Die ID ist eine globale Variable welche dem int

	Ini4Activate();

	if(!Initialized)                                                        // Ohne diese If Abfrage würden bei jedem Aufruf
	{																			// des Frames die Objekte neu initialisiert. Dies
		ComBoxMessg->Items->Add("Keine Angabe");

		// Neue Instanz von TMSQuery wird erzeugt
		if(QH==NULL)   {QH=new TMSQuery(NULL);   DS3->DataSet=QH;}
		Initialized=(QH!=NULL);        											// Initialisierungs Bool wird auf true gesetzt sodass

		// Abfrage der Messgeräte aus der Hilfsmitteltabelle und setzen der Vorauswahle (Index = 1)
		if(Form1->SendSQLLine("SELECT * FROM dbo.Hilfsmittel WHERE Typ='Messmittel' AND Protokoll_XTYP='X10K' ORDER BY Beschreibung",QH))
		{while(!QH->Eof){ ComBoxMessg->Items->Add(QH->FieldByName("Beschreibung")->AsString+" # "+QH->FieldByName("WNummer")->AsString.Trim()); QH->Next();}}

	}

	ImportDataFromDB();

	PracticalHeaderName(1);
	PracticalHeaderName(2);
	PracticalHeaderName(3);

	PDS.CHANGED=false;

	CheckReadOnly(ReadOnly);

	if(!ReadOnly)
	{
		HandleCountOfUnusedXMLRecords(1);
		HandleCountOfUnusedXMLRecords(2);
		HandleCountOfUnusedXMLRecords(3);
		CheckAllFieldsSet();
		DragImportFile();
	}
}

void __fastcall TFrameX10K::Ini4Activate()
{
	RowTable1 = -1; RowTable2 = -1; RowTable3 = -1;
	ColTable1 = -1; ColTable2 = -1; ColTable3 = -1;

	Col4RedTable1 = -1; Col4RedTable2 = -1; Col4RedTable3 = -1;

	RowEditTable1 = -1; RowEditTable2 = -1; RowEditTable3 = -1;

	RowSelectTable1 = -1; RowSelectTable2 = -1; RowSelectTable3 = -1;
	ColSelectTable1 = -1; ColSelectTable2 = -1; ColSelectTable3 = -1;


	xml4table1GUI = false;
	xml4table2GUI = false;
	xml4table3GUI = false;

	table1RememberOfXML.clear();
	table2RememberOfXML.clear();
	table3RememberOfXML.clear();

	XML_Name2DataBase_Name.clear();
	GeneratePairs4Header();
	InitTheHeader();

	ReadOnly = !Form7->CheckPriv2("X10K",0,0);  //"X10K",1 //Prüfen ob OK oder NCS  //gibt true bei Berechtigung zurück

	FRattachm->AutoScaleXmax=1200; //bezieht:
	FRattachm->AutoScaleYmax=1000;

	Left=5; Top=Form3->PNxwinBottom->Top-Height-5;
    memoInfo->ReadOnly = true;

	lbTable1CountOfXMLUnused->Caption = "";
	lbTable2CountOfXMLUnused->Caption = "";
	lbTable3CountOfXMLUnused->Caption = "";

	Visible=true;                                                           // Über diese Zeile wird das Frame sichtbar gemacht

	GRD->ColorBegin = GetGadientColor((TColor)COL_H_MAIN, COL_H, 30); 		// Farbschema wird dem Hintergrund TGradient
	GRD->ColorEnd  = Form3->GRDprot->ColorBegin;
}

//=============================================================================================================================================
	void __fastcall TFrameX10K::CheckReadOnly (bool ReadOnly)
//=============================================================================================================================================
{
	bool bOK = !ReadOnly;

	CBKuMess->Enabled = bOK;
	ComBoxMessg->Enabled = bOK;
	ComBoxAufbStatic->Enabled = bOK;

	FRattachm->Enabled = bOK;
//FRattachm->PNbottom->Enabled = bOK;
//FRattachm->PNpdf->Enabled = bOK;
//FRattachm->PNphoto->Enabled = bOK;
	FRattachm->Visible = bOK;
//	if(bOK == false)
//		FRattachm->Hide();
	spbToXML->Enabled = bOK;

	SpBtnC1->Enabled = bOK;
	SpBtnMA->Enabled = bOK;
	SpBtnTAP->Enabled = bOK;

	SpBtnC1_Delete->Enabled = bOK;
	SpBtnMA_Delete->Enabled = bOK;
	SpBtnTA_Delete->Enabled = bOK;
	table1RecZurDB->Enabled = bOK;
	table2RecZurDB->Enabled = bOK;
	table3RecZurDB->Enabled = bOK;


	if(bOK)
	{
		//ComBoxMessg->Parent = this;   //RRR42 sollte hier nicht nötig sein
		ComBoxMessg->Style = csDropDownList;
		ComBoxAufbStatic->Style = csDropDownList;
	}

	EditOrt->Enabled = bOK;
	NuEdTempU->Enabled = bOK;
	NuEdTempDfg->Enabled = bOK;
	NuEdFeuchte->Enabled = bOK;

	Memo1->Enabled = bOK;

	//Die Grids werden jeweils über
	//table1OnBeforeEdit gesperrt oder freigegeben
}

//=============================================================================================================================================
	void __fastcall TFrameX10K::CreateNewRecord     (int ID, int TestArt, TMSQuery *QTestArt)  // Kann verwendet werden um eine neue Zeile über Code zu generieren, hierbei wird  die Prod_Id übergeben
//=============================================================================================================================================
{
   if(ProtokolID<1) return;

   bool bSendSQLLine = false;

   if(!xml4table1GUI && (TestArt == 1) )
   {
		CreateNewRecordDB(ID, 1, QTestArt);
		//Neuaufbau mit beachtung ob gelöscht gekennzeichnet
		bSendSQLLine = Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =1 AND markedASdestroyed != 1 ",QC1);	PracticalHeaderName(1);
		if(bSendSQLLine)
			table1->Repaint();
   }

   if(!xml4table2GUI && (TestArt == 2) )
   {
		CreateNewRecordDB(ID, 2, QTestArt);
		//Neuaufbau mit beachtung ob gelöscht gekennzeichnet
		bSendSQLLine = Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =2 AND markedASdestroyed != 1",QMA);    PracticalHeaderName(2);
		if(bSendSQLLine)
			table2->Repaint();
   }

   if(!xml4table3GUI && (TestArt == 3) )
   {
		CreateNewRecordDB(ID, 3, QTestArt);
		//Neuaufbau mit beachtung ob gelöscht gekennzeichnet
		bSendSQLLine = Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =3 AND markedASdestroyed != 1",QTAP);   PracticalHeaderName(3);
		if(bSendSQLLine)
			table3->Repaint();
   }

 }

//=============================================================================================================================================
	void __fastcall TFrameX10K::CreateNewRecordDB     (int ID, int TestArt, TMSQuery *QTestArt)  // Kann verwendet werden um eine neue Zeile über Code zu generieren, hierbei wird  die Prod_Id übergeben
//=============================================================================================================================================
{
	int markedASdestroyed = 0;
	bool bSendSQLLine = false;
	TDateTime  DTnow = Now().FormatString("dd.mm.yyyy hh:nn:ss");

	if(TestArt == 1)
		QTestArt->SQL->Text = "SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =1 AND markedASdestroyed != 1 ";
	else if(TestArt == 2)
		QTestArt->SQL->Text = "SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =2 AND markedASdestroyed != 1 ";
	else if(TestArt == 3)
		QTestArt->SQL->Text = "SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =3 AND markedASdestroyed != 1 ";


	QTestArt->Open();
	QTestArt->Append();
	//----------------------------------

	QTestArt->FieldByName("PROT_ID")->AsInteger = ProtokolID;
	QTestArt->FieldByName("TS")->AsDateTime = DTnow;

	//2 mal smallint
	QTestArt->FieldByName("Test_Art")->AsInteger = TestArt;
	QTestArt->FieldByName("markedASdestroyed")->AsInteger = markedASdestroyed;

	//großes Projekt
	if(NuEdFeuchte->Value != 0.0)
		QTestArt->FieldByName("Feuchte")->AsFloat = NuEdFeuchte->Value;

	if(NuEdTempU->Value != 0.0)
		QTestArt->FieldByName("TempU")->AsFloat = NuEdTempU->Value;

	if(NuEdTempDfg->Value != 0.0)
		QTestArt->FieldByName("TempDfg")->AsFloat = NuEdTempDfg->Value;

	if(Memo1->Text != L"")
		QTestArt->FieldByName("Kommentar")->AsString = Memo1->Text;

	//----------------------------------
	QTestArt->Post();
	QTestArt->Close();
}

 //=============================================================================================================================================
	void __fastcall TFrameX10K::ImportDataFromDB()     // Importiert den Datensatz aus der MS Tabelle. Die ID Nummer ist durch die Variable
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

	 TMSQuery* Qtemp1 = NULL;
	 TMSDataSource *DStemp1 = NULL;
	 Qtemp1 = new TMSQuery(NULL);
	 DStemp1 = new TMSDataSource(NULL);

	 TMSQuery* Qtemp2 = NULL;
	 TMSDataSource *DStemp2 = NULL;
	 Qtemp2 = new TMSQuery(NULL);
	 DStemp2 = new TMSDataSource(NULL);

	 TMSQuery* Qtemp3 = NULL;
	 TMSDataSource *DStemp3 = NULL;
	 Qtemp3 = new TMSQuery(NULL);
	 DStemp3 = new TMSDataSource(NULL);

	 if(Form1->SendSQLLine("SELECT * FROM dbo.PROTOKOL WHERE ID="+IntToStr(ProtokolID),QH))
	  {
			CBKuMess 	->Checked	= QH->FieldByName("UNIB1")->AsBoolean;
			ComBoxAufbStatic  ->ItemIndex = QH->FieldByName("UNIINT2")->AsInteger;
			ComBoxMessg ->ItemIndex = QH->FieldByName("UNIINT3")->AsInteger;

			EditOrt 	->Text      = QH->FieldByName("UNIT1")->AsAnsiString;
			NuEdTempU	->Value 		= QH->FieldByName("UNIF1")->AsFloat;    //(1)
			NuEdTempDfg ->Value 		= QH->FieldByName("UNIF2")->AsFloat;    //(1)
			NuEdFeuchte ->Value 		= QH->FieldByName("UNIF3")->AsFloat;    //(1)

			Memo1->Text = QH->FieldByName("MEMO1")->AsString;

	  }

	 if(ProtokolID>0)
	  {

		if(table1)
		{
			table1->BeginUpdate();
			table1->DataSource = DStemp1;
			DStemp1->DataSet=Qtemp1;
			table1->EndUpdate();

			if(QC1)
				delete QC1;
			QC1 = NULL;
		}


		if(table2)
		{
			table2->BeginUpdate();
			table2->DataSource = DStemp2;
			DStemp2->DataSet=Qtemp2;
			table2->EndUpdate();

			if(QMA)
				delete QMA;
			QMA = NULL;
		}

		if(table3)
		{
			table3->BeginUpdate();
			table3->DataSource = DStemp3;
			DStemp3->DataSet=Qtemp3;
			table3->EndUpdate();

			if(QTAP)
				delete QTAP;
			QTAP = NULL;
		}

		if(QC1==NULL)  {QC1=new TMSQuery(NULL);  table1->DataSource = DS; DS->DataSet=QC1;}              // Wenn der Zeiger QC1=0, soll eine neue Instanz von
		if(QMA==NULL)  {QMA=new TMSQuery(NULL);  table2->DataSource = DS1; DS1->DataSet=QMA;}   			// TMSQuery mit dem Namen QC1 erzeugt und mit der
		if(QTAP==NULL) {QTAP=new TMSQuery(NULL); table3->DataSource = DS2; DS2->DataSet=QTAP;}            // Datenquelle DS verbunden werden.

		Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =1 AND markedASdestroyed != 1 ",QC1);	PracticalHeaderName(1);
		Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =2 AND markedASdestroyed != 1",QMA);	PracticalHeaderName(2);
		Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =3 AND markedASdestroyed != 1",QTAP); PracticalHeaderName(3);

	  }

	  if(Qtemp1)
		delete Qtemp1;
	  if(DStemp1)
		delete DStemp1;
	  if(Qtemp2)
		delete Qtemp2;
	  if(DStemp2)
		delete DStemp2;
	  if(Qtemp3)
		delete Qtemp3;
	  if(DStemp3)
		delete DStemp3;
}

//=============================================================================================================================================
	void __fastcall TFrameX10K::ExportDataToDB()     // Exportiert den Datensatz aus der GUI in die MS Tabelle. Die ID Nummer ist durch
//=============================================================================================================================================
{
	PDS.UNIB1   = CBKuMess->Checked;
	PDS.UNIINT2 = ComBoxAufbStatic->ItemIndex;
	PDS.UNIINT3 = ComBoxMessg->ItemIndex;
	PDS.UNIT1   = EditOrt->Text;
	//Die 3 unten noch überlegen ob dies nötig weil schon in X10K
	PDS.UNIF1   = 	NuEdTempU->Value;
	PDS.UNIF2	= 	NuEdTempDfg->Value;
	PDS.UNIF3	= 	NuEdFeuchte->Value;

	PDS.MEMO = Memo1->Text;
	TDateTime  DTnow = Now().FormatString("dd.mm.yyyy hh:nn:ss");
	PDS.TIMESTAMP = DTnow;
}
//=============================================================================================================================================
void __fastcall TFrameX10K::CBKuMessClick(TObject *Sender)       // Änderung der Vorauswahl bei Kudenmessung
//=============================================================================================================================================
{
	SetChangeFlag();
}
//=============================================================================================================================================
	void __fastcall TFrameX10K::SetChangeFlag()     //  Wird sofort aktiv wenn geklickt oder geändert wird. //RRR42 Morgen
//=============================================================================================================================================
{
	PDS.CHANGED = true;
}

//=============================================================================================================================================
	bool __fastcall TFrameX10K::CheckAllFieldsSet()     // Alle Pflichtfelder gesetzt ?
//=============================================================================================================================================
{
	bool bOK = false;
    //Um kürzer zu halten die Bedingung
	int i = ComBoxAufbStatic->ItemIndex;
	int j = ComBoxMessg->ItemIndex;

	if( (i == 0) || (j == 0) || (NuEdTempU->Value == 0.0) || (NuEdTempDfg->Value == 0.0) || (NuEdFeuchte->Value == 0.0) )
	{
		bOK = false;
		memoInfo->Color = clRed;
		memoInfo->Text = "Nicht OK";
		//Wenn ein Wert unsinnig ist wird es keine neue Speicherung geben !!!
		PDS.CHANGED = false;
	}
	else if ( (i > 0) && (j > 0) && (NuEdTempU->Value != 0.0) && (NuEdTempDfg->Value != 0.0) && (NuEdFeuchte->Value != 0.0) )
	{
		bOK = true;
		memoInfo->Color = clGreen;
		memoInfo->Text = "OK";
	}

	spbToXML->Visible = bOK;

	SpBtnC1->Visible = bOK;
	SpBtnMA->Visible = bOK;
	SpBtnTAP->Visible = bOK;

	SpBtnC1_Delete->Visible = bOK;
	SpBtnMA_Delete->Visible = bOK;
	SpBtnTA_Delete->Visible = bOK;
		
	table1RecZurDB->Visible = bOK;
	table2RecZurDB->Visible = bOK;
	table3RecZurDB->Visible = bOK;

	table1->ReadOnly = !bOK;
	table2->ReadOnly = !bOK;
	table3->ReadOnly = !bOK;

	Memo1->Enabled = bOK;
	return bOK;
}

//=============================================================================================================================================
	void __fastcall TFrameX10K::InitTheHeader()
//=============================================================================================================================================
{
	// Vorauswahl Aufbau  (Index = 1)
	ComBoxAufbStatic->Items->Add("Keine Angabe");
	ComBoxAufbStatic->Items->Add("Kran");
	ComBoxAufbStatic->Items->Add("Böcke");
	ComBoxAufbStatic->Items->Add("Montagegestell");
	ComBoxAufbStatic->Items->Add("Sonstiges");
	ComBoxAufbStatic->ItemIndex = 1; //1; früher //Test war 4

	NuEdTempU->Value = 0,0;
	NuEdTempDfg->Value = 0,0;
	NuEdFeuchte->Value = 0,0;
	EditOrt->Text = "";

}

//---------------------------------------------------------------------------
//=============================================================================================================================================
void __fastcall TFrameX10K::table4DblClick(TObject *Sender)
//=============================================================================================================================================
{
 ;
}
//=============================================================================================================================================
void __fastcall TFrameX10K::SpBtnC1Click(TObject *Sender)
{
	table1->DataSource = DS; DS->DataSet = QC1; CreateNewRecord( ProtokolID, 1, QC1); PracticalHeaderName(1);
}

void __fastcall TFrameX10K::SpBtnMAClick(TObject *Sender)
{
	table2->DataSource = DS1; DS1->DataSet = QMA; CreateNewRecord( ProtokolID, 2, QMA); PracticalHeaderName(2);
}

void __fastcall TFrameX10K::SpBtnTAPClick(TObject *Sender)
{
	table3->DataSource = DS2; DS2->DataSet = QTAP; CreateNewRecord( ProtokolID, 3, QTAP); PracticalHeaderName(3);
}
//---------------------------------------------------------------------------


//RRRTEST
void __fastcall TFrameX10K::ShowFormXML()
{
}

void __fastcall TFrameX10K::CreateAndShowForm()
{
	ustrXMLFile = U"";
	RR4X10K::Create_XMLFrame();
	if(FormXML)
	{
		FormXML->pFX10K = this;
		if (FormXML->OpenTextFileDialog->Execute())
		{
			ustrXMLFile = FormXML->OpenTextFileDialog->FileName;
		}

		if(!ustrXMLFile.IsEmpty())
		{
			FormXML->ShowModal();
			HandleCountOfUnusedXMLRecords(FormXML->ComboBoxDBGridUserChoice->ItemIndex+1);
			CheckAllFieldsSet();
	}
		else
			ShowMessage("Sie haben die Aktion abgebrochen. Ohne die Wahl einer XML-Datei werden keine Daten geladen.");
	}
	else
		ShowMessage("Problem XML-Manager auf zu bauen");
}

void __fastcall TFrameX10K::spbToXML1Click(TObject *Sender)
{
	if(!xml4table1GUI)
	{
		table1RememberOfXML.clear();
	}

	if(!xml4table2GUI)
	{
		table2RememberOfXML.clear();
	}

	if(!xml4table3GUI)
	{
		table3RememberOfXML.clear();
	}

	CreateAndShowForm();
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table1OnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

	//Normalfall
	if ( (ARow >= 0) && (GetCountOfUnusedXMLRecords(1) == 0) )
	{
		FontStyle = TFontStyles();
			return;
	}

	//Der Fall weiterhin noch nicht beachtet
	if ((ARow >= 0) && (xml4table1GUI))
	{

		if(GetCountOfUnusedXMLRecords(1) > 0)
		{
			int index = ARow;
			std::pair<int, int> pairSelected = table1RememberOfXML[index];

			if((ARow >= 0) && (pairSelected.first == 1) && (xml4table1GUI))
			{
				FontStyle = TFontStyles();
				TextColor = clWebGold; //clCream;
				return;
			}

			//Der Fall inzwischen beachtet, also zur Datenbank mit dem TSpeedButton Namens table1RecZurDB
			if( (ARow >= 0) && (pairSelected.first == 0) && (xml4table1GUI) )
			{
				FontStyle = TFontStyles();
			}
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2OnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

	//Normalfall
	if ( (ARow >= 0) && (GetCountOfUnusedXMLRecords(2) == 0) )
	{
		FontStyle = TFontStyles();
			return;
	}

	//Der Fall weiterhin noch nicht beachtet
	if ((ARow >= 0) && (xml4table2GUI))
	{

		if(GetCountOfUnusedXMLRecords(2) > 0)
		{
			int index = ARow;
			std::pair<int, int> pairSelected = table2RememberOfXML[index];

			if((ARow >= 0) && (pairSelected.first == 1) && (xml4table2GUI))
			{
				FontStyle = TFontStyles();
				TextColor = clWebGold; //clCream;
				return;
			}

			//Der Fall inzwischen beachtet, also zur Datenbank mit dem TSpeedButton Namens table1RecZurDB
			if( (ARow >= 0) && (pairSelected.first == 0) && (xml4table2GUI) )
			{
				FontStyle = TFontStyles();
			}
		}
	}


}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

    //Normalfall
	if ( (ARow >= 0) && (GetCountOfUnusedXMLRecords(3) == 0) )
	{
		FontStyle = TFontStyles();
			return;
	}

	//Der Fall weiterhin noch nicht beachtet
	if ((ARow >= 0) && (xml4table3GUI))
	{

		if(GetCountOfUnusedXMLRecords(3) > 0)
		{
			int index = ARow;
			std::pair<int, int> pairSelected = table3RememberOfXML[index];

			if((ARow >= 0) && (pairSelected.first == 1) && (xml4table3GUI))
			{
				FontStyle = TFontStyles();
				TextColor = clWebGold; //clCream;
				return;
			}

			//Der Fall inzwischen beachtet, also zur Datenbank mit dem TSpeedButton Namens table1RecZurDB
			if( (ARow >= 0) && (pairSelected.first == 0) && (xml4table3GUI) )
			{
				FontStyle = TFontStyles();
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFrameX10K::table1OnCellClick(TObject *Sender, int ACol, int ARow)
{
	if(ReadOnly)
		return;

    if( (ARow < 0) || (ACol < 0) )
		return;

	table1->Columns->Item[ACol]->Options << coEditing;   //RRR42 schauen ob wieder rein
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
//table1->RefreshRow(ARow);

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2OnCellClick(TObject *Sender, int ACol, int ARow)
{
	if(ReadOnly)
		return;

    if( (ARow < 0) || (ACol < 0) )
		return;

	table2->Columns->Item[ACol]->Options << coEditing;   //RRR42 schauen ob wieder rein
	int Spalte = -1;
	//table1->Columns->Item[8]->Field->SetFieldType(ftInteger); //DataType = ftInteger;
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
//table2->RefreshRow(ARow);

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnCellClick(TObject *Sender, int ACol, int ARow)

{
    if(ReadOnly)
		return;

    if( (ARow < 0) || (ACol < 0) )
		return;

	table3->Columns->Item[ACol]->Options << coEditing;   //RRR42 schauen ob wieder rein
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
//table3->RefreshRow(ARow);
}
//---------------------------------------------------------------------------


void __fastcall TFrameX10K::SpBtnC1Click_Delete(TObject *Sender)
{
	MarkAsDelete4Table(1);
	int coutsOfUnusedXMLRows = HandleCountOfUnusedXMLRecords(1);
	if(coutsOfUnusedXMLRows == 0)
	{
		if(table1)
		{
			bool bSendSQLLine = false;

			TMSQuery* Qtemp1 = NULL;
			TMSDataSource *DStemp1 = NULL;
			Qtemp1 = new TMSQuery(NULL);
			DStemp1 = new TMSDataSource(NULL);

			table1->BeginUpdate();
			table1->DataSource = DStemp1;
			DStemp1->DataSet=Qtemp1;
			table1->EndUpdate();


			if(QC1)
				delete QC1;
			QC1 = NULL;

			if(QC1==NULL)  {QC1=new TMSQuery(NULL);  table1->DataSource = DS; DS->DataSet=QC1;}              // Wenn der Zeiger QC1=0, soll eine neue Instanz von

			bSendSQLLine = Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =1 AND markedASdestroyed != 1 ",QC1);   PracticalHeaderName(1);
			if(bSendSQLLine)
				table1->Repaint();

			if(Qtemp1)
				delete Qtemp1;
			if(DStemp1)
				delete DStemp1;
		}
	}
	
}

//---------------------------------------------------------------------------

void __fastcall TFrameX10K::SpBtnMAClick_Delete(TObject *Sender)
{
	MarkAsDelete4Table(2);

	int coutsOfUnusedXMLRows = HandleCountOfUnusedXMLRecords(2);
	if(coutsOfUnusedXMLRows == 0)
	{
		if(table2)
		{
			bool bSendSQLLine = false;

			TMSQuery* Qtemp1 = NULL;
			TMSDataSource *DStemp1 = NULL;
			Qtemp1 = new TMSQuery(NULL);
			DStemp1 = new TMSDataSource(NULL);

			table2->BeginUpdate();
			table2->DataSource = DStemp1;
			DStemp1->DataSet=Qtemp1;
			table2->EndUpdate();

			if(QMA)
				delete QMA;
			QMA = NULL;

			if(QMA == NULL)  {QMA = new TMSQuery(NULL);  table2->DataSource = DS1; DS1->DataSet = QMA;}              // Wenn der Zeiger QC1=0, soll eine neue Instanz von

			bSendSQLLine = Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =2 AND markedASdestroyed != 1 ",QMA);   PracticalHeaderName(2);
			if(bSendSQLLine)
				table2->Repaint();

			if(Qtemp1)
				delete Qtemp1;
			if(DStemp1)
				delete DStemp1;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::SpBtnTAPClick_Delete(TObject *Sender)
{
	MarkAsDelete4Table(3);

	int coutsOfUnusedXMLRows = HandleCountOfUnusedXMLRecords(3);
	if(coutsOfUnusedXMLRows == 0)
	{
		if(table3)
		{
			bool bSendSQLLine = false;

			TMSQuery* Qtemp1 = NULL;
			TMSDataSource *DStemp1 = NULL;
			Qtemp1 = new TMSQuery(NULL);
			DStemp1 = new TMSDataSource(NULL);

			table3->BeginUpdate();
			table3->DataSource = DStemp1;
			DStemp1->DataSet=Qtemp1;
			table3->EndUpdate();

			if(QTAP)
				delete QTAP;
			QTAP = NULL;

			if(QTAP == NULL)  {QTAP = new TMSQuery(NULL);  table3->DataSource = DS2; DS2->DataSet = QTAP;}              // Wenn der Zeiger QC1=0, soll eine neue Instanz von

			bSendSQLLine = Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =3 AND markedASdestroyed != 1 ",QTAP);  PracticalHeaderName(3);
			if(bSendSQLLine)
				table3->Repaint();

			if(Qtemp1)
				delete Qtemp1;
			if(DStemp1)
				delete DStemp1;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::HandleIfDelete(int tableNr) //RRR999
{
	//Hilfsvektor
	std::vector < std::pair < int, int > > VorSelectRememberOfXML;
	VorSelectRememberOfXML.clear();

	//Hilfsvektor
	std::vector < std::pair < int, int > > NachSelectRememberOfXML;
	NachSelectRememberOfXML.clear();

	if(table1 && (tableNr == 1) )
	{
		int testselect = table1->SelectedRow;
		if(table1->SelectedRow > -1)
		{
			int index = -1;
			//Im Grid löschen
			int indexAfterThisMinusOneIndexPos = table1->SelectedRow;

			table1->DeleteRow(table1->SelectedRow);

			for(std::vector<std::pair<int, int> >::iterator it =
				table1RememberOfXML.begin(); it < (table1RememberOfXML.begin() + indexAfterThisMinusOneIndexPos) ; ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				VorSelectRememberOfXML.push_back(tempPairs);
			}

			//Fall wegen C++ Builder SelectedRow noch nicht aktualisiert
			int countofIndex = 0;
			for(std::vector<std::pair<int, int> >::iterator it =
				table1RememberOfXML.begin(); it != table1RememberOfXML.end(); ++it)
			{
				//Sich alle vor dem zu löschenden merken
				++countofIndex;

			}

			if(countofIndex < (indexAfterThisMinusOneIndexPos + 1) ) //table1->SelectedRow)
			{
				return;
			}


			for(std::vector<std::pair<int, int> >::iterator it =
				table1RememberOfXML.begin() + indexAfterThisMinusOneIndexPos + 1; it != table1RememberOfXML.end(); ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				NachSelectRememberOfXML.push_back(tempPairs);
			}

			table1RememberOfXML.clear();
			
			for(std::vector<std::pair<int, int> >::iterator it =
				VorSelectRememberOfXML.begin(); it != VorSelectRememberOfXML.end() ; ++it)
			{

				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				table1RememberOfXML.push_back(tempPairs);
			}

			for(std::vector<std::pair<int, int> >::iterator it =
				NachSelectRememberOfXML.begin(); it != NachSelectRememberOfXML.end() ; ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				table1RememberOfXML.push_back(tempPairs);
			}

			VorSelectRememberOfXML.clear();
			NachSelectRememberOfXML.clear();
		}
	}

	if(table2 && (tableNr == 2) )
	{
		if(table2->SelectedRow > -1)
		{
			int index = -1;
			//Im Grid löschen
			table2->DeleteRow(table2->SelectedRow);


			//Aber im Remember nicht die anderen vergessen !!!
			int indexAfterThisMinusOneIndexPos = table2->SelectedRow;

			for(std::vector<std::pair<int, int> >::iterator it =
				table2RememberOfXML.begin(); it < (table2RememberOfXML.begin() + indexAfterThisMinusOneIndexPos) ; ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				VorSelectRememberOfXML.push_back(tempPairs);
			}


			//Fall wegen C++ Builder SelectedRow noch nicht aktualisiert
			int countofIndex = 0;
			for(std::vector<std::pair<int, int> >::iterator it =
				table2RememberOfXML.begin(); it != table2RememberOfXML.end(); ++it)
			{
				//Sich alle vor dem zu löschenden merken
				++countofIndex;

			}

			if(countofIndex < (indexAfterThisMinusOneIndexPos + 1) ) //table1->SelectedRow)
			{
				return;
			}

			for(std::vector<std::pair<int, int> >::iterator it =
				table2RememberOfXML.begin() + indexAfterThisMinusOneIndexPos + 1; it != table2RememberOfXML.end(); ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				NachSelectRememberOfXML.push_back(tempPairs);
			}

			table2RememberOfXML.clear();

			//------------------------------

			for(std::vector<std::pair<int, int> >::iterator it =
				VorSelectRememberOfXML.begin(); it != VorSelectRememberOfXML.end() ; ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				table2RememberOfXML.push_back(tempPairs);
			}

			for(std::vector<std::pair<int, int> >::iterator it =
				NachSelectRememberOfXML.begin(); it != NachSelectRememberOfXML.end() ; ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				table2RememberOfXML.push_back(tempPairs);
			}

			VorSelectRememberOfXML.clear();
			NachSelectRememberOfXML.clear();

		}
	}

	if(table3 && (tableNr == 3) )
	{
		if(table3->SelectedRow > -1)
		{
			int index = -1;
			//Im Grid löschen
			table3->DeleteRow(table3->SelectedRow);


			//Aber im Remember nicht die anderen vergessen !!!
			int indexAfterThisMinusOneIndexPos = table3->SelectedRow;

            for(std::vector<std::pair<int, int> >::iterator it =
				table3RememberOfXML.begin(); it < (table3RememberOfXML.begin() + indexAfterThisMinusOneIndexPos) ; ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				VorSelectRememberOfXML.push_back(tempPairs);
			}

            	//Fall wegen C++ Builder SelectedRow noch nicht aktualisiert
			int countofIndex = 0;
			for(std::vector<std::pair<int, int> >::iterator it =
				table3RememberOfXML.begin(); it != table3RememberOfXML.end(); ++it)
			{
				//Sich alle vor dem zu löschenden merken
				++countofIndex;

			}

			if(countofIndex < (indexAfterThisMinusOneIndexPos + 1) ) //table1->SelectedRow)
			{
				return;
			}

			for(std::vector<std::pair<int, int> >::iterator it =
				table3RememberOfXML.begin() + indexAfterThisMinusOneIndexPos + 1; it != table3RememberOfXML.end(); ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				NachSelectRememberOfXML.push_back(tempPairs);
			}

			table3RememberOfXML.clear();

			//------------------------------

			for(std::vector<std::pair<int, int> >::iterator it =
				VorSelectRememberOfXML.begin(); it != VorSelectRememberOfXML.end() ; ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				table3RememberOfXML.push_back(tempPairs);
			}

			for(std::vector<std::pair<int, int> >::iterator it =
				NachSelectRememberOfXML.begin(); it != NachSelectRememberOfXML.end() ; ++it)
			{
				//Sich alle vor dem zu löschenden merken
				std::pair<int, int> tempPairs = *it;
				table3RememberOfXML.push_back(tempPairs);
			}

			VorSelectRememberOfXML.clear();
			NachSelectRememberOfXML.clear();
		}
	}
}

//RRR999 auch hier die anderen Tables erqweitern
void __fastcall TFrameX10K::MarkAsDelete4Table(int TestArt)
{
	//String marker4Destroy = L"true";
	int marker4Destroy = 1;
	String cellValue = "";
	bool bTest = false;

	if(TestArt == 1)
	{

		if(table1->RowCount == 0)
			return;

		if(xml4table1GUI)
		{
			HandleIfDelete(1);
			table1->Repaint();
			return;
		}

		//ID zu erhalten
		if(	(ColTable1 > -1) && (RowTable1 > -1) )
		{
			cellValue = table1->GetCellValue(ColTable1, RowTable1);
		}
		else
			return;

		String sql = "UPDATE [MDE].[dbo].[X10K] SET markedASdestroyed = " + IntToStr(marker4Destroy) + " WHERE ID = " + cellValue;

		Form1->SendSQLLine(sql,QC1);
	}
	else if(TestArt == 2)
	{
		if(table2->RowCount == 0)
			return;

		if(xml4table2GUI)
		{
			HandleIfDelete(2);
			table2->Repaint();
			return;
		}

		//ID zu erhalten
		if(	(ColTable2 > -1) && (RowTable2 > -1) )
		{
			cellValue = table2->GetCellValue(ColTable2, RowTable2);
		}
		else
			return;

		String sql = "UPDATE [MDE].[dbo].[X10K] SET markedASdestroyed = " + IntToStr(marker4Destroy) + " WHERE ID = " + cellValue;

		Form1->SendSQLLine(sql,QMA);
	}
	else if(TestArt == 3)
	{
		if(table3->RowCount == 0)
			return;

		if(xml4table3GUI)
		{
			HandleIfDelete(3);
			table3->Repaint();
			return;
		}

		//ID zu erhalten
		if(	(ColTable3 > -1) && (RowTable3 > -1) )
		{
			cellValue = table3->GetCellValue(ColTable3, RowTable3);
		}
		else
			return;

		String sql = "UPDATE [MDE].[dbo].[X10K] SET markedASdestroyed = " + IntToStr(marker4Destroy) + " WHERE ID = " + cellValue;

		Form1->SendSQLLine(sql,QTAP);
	}

}

//Erstmal nur Kennzeichnen ob gelöscht

void __fastcall TFrameX10K::table1OnSelectCell(TObject *Sender, int ACol, int ARow)
{
	if( (ARow < 0) || (ACol < 0) )
	{
		SpBtnC1_Delete->Enabled = false;
		table1RecZurDB->Enabled = false;
		return;

	}
	if(ReadOnly)
		return;

	RowSelectTable1 = ARow; ColSelectTable1 = ACol;
	if( (table1->SelectedRow >=0) && (ARow == table1->SelectedRow) )
	{

		table1->RefreshRow(table1->SelectedRow);
		SpBtnC1_Delete->Enabled = true;
		table1RecZurDB->Enabled = true;
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2OnSelectCell(TObject *Sender, int ACol, int ARow)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

	if(ReadOnly)
		return;

	RowSelectTable2 = ARow; ColSelectTable2 = ACol;
	if( (table2->SelectedRow >=0) && (ARow == table2->SelectedRow) ) //RRR999
		table2->RefreshRow(table2->SelectedRow);
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnSelectCell(TObject *Sender, int ACol, int ARow)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

	if(ReadOnly)
		return;

	RowSelectTable3 = ARow; ColSelectTable3 = ACol;
	if( (table3->SelectedRow >=0) && (ARow == table3->SelectedRow) ) //RRR999
		table3->RefreshRow(table3->SelectedRow);
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::Table1OnCellColoring(TObject *Sender, int ACol, int ARow,
		  TColor &CellColor, TColor &GridColor, TCellState CellState)
{
	if( (ARow < 0) || (ACol < 0) )
		return;

	if(!xml4table1GUI)
	{
		if( ARow == table1->SelectedRow )
			CellColor = clSkyBlue;
	}
	else
	{
		//Fall da weil nicht gelöscht aber mal schauen ob schon beachtet
		if ( GetCountOfUnusedXMLRecords(1) > 0 )
		{
			int index =	 ARow;
			std::pair<int, int> pairSelected = table1RememberOfXML[index];
							  // 1: also noch zu beachten. 0: ist beachtet worden
			if( pairSelected.first == 1 )
			{
				table1->Canvas->Brush->Color = clWindow;
			}
			else
			{                                  //grün es wurde diese zeile in die Datenbank geschrieben
				table1->Canvas->Brush->Color = clGreen;
			}

		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::Table2OnCellColoring(TObject *Sender, int ACol, int ARow,
		  TColor &CellColor, TColor &GridColor, TCellState CellState)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

	if(!xml4table2GUI)
	{
		if( ARow == table2->SelectedRow )
			CellColor = clSkyBlue;
	}
	else
	{
		//Fall da weil nicht gelöscht aber mal schauen ob schon beachtet
		if ( GetCountOfUnusedXMLRecords(2) > 0 )
		{
			int index =	 ARow;
			std::pair<int, int> pairSelected = table2RememberOfXML[index];
							  // 1: also noch zu beachten. 0: ist beachtet worden
			if( pairSelected.first == 1 )
			{
				table2->Canvas->Brush->Color = clWindow;
			}
			else
			{                                  //grün es wurde diese zeile in die Datenbank geschrieben
				table2->Canvas->Brush->Color = clGreen;
			}

		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::Table3OnCellColoring(TObject *Sender, int ACol, int ARow,
		  TColor &CellColor, TColor &GridColor, TCellState CellState)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

	if(!xml4table3GUI)
	{
		if( ARow == table3->SelectedRow )
			CellColor = clSkyBlue;
	}
	else
	{
		//Fall da weil nicht gelöscht aber mal schauen ob schon beachtet
		if ( GetCountOfUnusedXMLRecords(3) > 0 )
		{
			int index =	 ARow;
			std::pair<int, int> pairSelected = table3RememberOfXML[index];
							  // 1: also noch zu beachten. 0: ist beachtet worden
			if( pairSelected.first == 1 )
			{
				table3->Canvas->Brush->Color = clWindow;
			}
			else
			{                                  //grün es wurde diese zeile in die Datenbank geschrieben
				table3->Canvas->Brush->Color = clGreen;
			}

		}
	}
}
//---------------------------------------------------------------------------

//Hier evntuell das Problem mit den Spinwheels beim Editieren
//Was wenn auch im normalen Modus Tabelle zu laden //RRR999  //Achtung alle 3 Tables !!!
void __fastcall TFrameX10K::PracticalHeaderName(int tableNr)
{

	TField* Field;
	UnicodeString unistr;

	if(tableNr == 1)
	{
		for (int i = 0; i < table1->Columns->Count ; ++i)
		{
			//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
			Field = table1->Columns->Item[i]->Field;
			//Feldname wie im DataSet !!!
			unistr = Field->FieldName;

			if( (unistr == U"Test_Art")||(unistr == U"markedASdestroyed")||(unistr == U"TempU")||(unistr == U"TempDfg")||(unistr == U"Feuchte")||(unistr == U"Kommentar") )
				{	table1->Columns->Item[i]->Visible = false;	}

			if(!ReadOnly)
			{
				table1->Columns->Item[i]->Options << coEditing;   //RRR42 schauen ob wieder rein
				table1->Columns->Item[i]->Enabled = true;
			}

		}
	}

	if(tableNr == 2)
	{
		for (int i = 0; i < table2->Columns->Count ; ++i)
		{
			//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
			Field = table2->Columns->Item[i]->Field;
			//Feldname wie im DataSet !!!
			unistr = Field->FieldName;

			if( (unistr == U"Test_Art")||(unistr == U"markedASdestroyed")||(unistr == U"ID")||(unistr == U"TempU")||(unistr == U"TempDfg")||(unistr == U"Feuchte")||(unistr == U"Kommentar") )
				{	table2->Columns->Item[i]->Visible = false;	}

			if(!ReadOnly)
			{
				table2->Columns->Item[i]->Options << coEditing;   //RRR42 schauen ob wieder rein
				table2->Columns->Item[i]->Enabled = true;
			}

		}
	}

	if(tableNr == 3)
	{
		for (int i = 0; i < table3->Columns->Count ; ++i)
		{
			//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
			Field = table3->Columns->Item[i]->Field;
			//Feldname wie im DataSet !!!
			unistr = Field->FieldName;

			if( (unistr == U"Test_Art")||(unistr == U"markedASdestroyed")||(unistr == U"ID")||(unistr == U"TempU")||(unistr == U"TempDfg")||(unistr == U"Feuchte")||(unistr == U"Kommentar") )
				{	table3->Columns->Item[i]->Visible = false;	}

			if(!ReadOnly)
			{
				table3->Columns->Item[i]->Options << coEditing;   //RRR42 schauen ob wieder rein
				table3->Columns->Item[i]->Enabled = true;
			}

		}
	}
}

void __fastcall TFrameX10K::table1OnBeforeEdit(TObject *Sender, int ACol, int ARow,
		  bool &Accept)
{
	Accept = !ReadOnly;
	//Auch dann nur wenn alles vom header ausgefüllt wurde
	Accept = CheckAllFieldsSet();

	if(!Accept)
		return;

    if( (ARow < 0) || (ACol < 0) )
		return;

		TField* Field;
		UnicodeString unistr;

		for (int i = 0; i < table1->Columns->Count ; ++i)
		{
			//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
			Field = table1->Columns->Item[i]->Field;
			//Feldname wie im DataSet !!!
			unistr = Field->FieldName;
			table1->Columns->Item[i]->Options << coEditing;   //RRR42 schauen ob wieder rein
			table1->Columns->Item[i]->Enabled = true;

		}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2OnBeforeEdit(TObject *Sender, int ACol, int ARow,
		  bool &Accept)
{
	Accept = !ReadOnly;
	//Auch dann nur wenn alles vom header ausgefüllt wurde
	Accept = CheckAllFieldsSet();

	if(!Accept)
		return;

    if( (ARow < 0) || (ACol < 0) )
		return;

		TField* Field;
		UnicodeString unistr;

		for (int i = 0; i < table2->Columns->Count ; ++i)
		{
			//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
			Field = table2->Columns->Item[i]->Field;
			//Feldname wie im DataSet !!!
			unistr = Field->FieldName;
			table2->Columns->Item[i]->Options << coEditing;   //RRR42 schauen ob wieder rein
			table2->Columns->Item[i]->Enabled = true;

		}

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnBeforeEdit(TObject *Sender, int ACol, int ARow,
		  bool &Accept)
{
	Accept = !ReadOnly;
	//Auch dann nur wenn alles vom header ausgefüllt wurde
	Accept = CheckAllFieldsSet();

	if(!Accept)
		return;

    if( (ARow < 0) || (ACol < 0) )
		return;

		TField* Field;
		UnicodeString unistr;

		for (int i = 0; i < table3->Columns->Count ; ++i)
		{
			//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
			Field = table3->Columns->Item[i]->Field;
			//Feldname wie im DataSet !!!
			unistr = Field->FieldName;
			table3->Columns->Item[i]->Options << coEditing;   //RRR42 schauen ob wieder rein
			table3->Columns->Item[i]->Enabled = true;

		}

}
//---------------------------------------------------------------------------

//Hier könnten noch eine Reihe an Prüfungen erfolgen !!!
void __fastcall TFrameX10K::table1OnEditAccept(TObject *Sender, int ACol, int ARow,
          WideString Value, bool &Accept)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

	if(Value == L"")
	{
		Accept = false;
	}
	else
	{
		Accept = true;  // :Accept = !true;  :So rein schreiben aber nicht akzeptiert nach Enter
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2OnEditAccept(TObject *Sender, int ACol, int ARow,
		  WideString Value, bool &Accept)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

    if(Value == L"")
	{
		Accept = false;
	}
	else
	{
		Accept = true;  // :Accept = !true;  :So rein schreiben aber nicht akzeptiert nach Enter
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnEditAccept(TObject *Sender, int ACol, int ARow,
		  WideString Value, bool &Accept)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

    if(Value == L"")
	{
		Accept = false;
	}
	else
	{
		Accept = true;  // :Accept = !true;  :So rein schreiben aber nicht akzeptiert nach Enter
	}
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

//RRR42 hier wenn es geht ein test machen
//=============================================================================================================================================
void __fastcall TFrameX10K::DragImportFile()    //Aufruf in:  TFrameMEMO::Activate(int ID, String OwnerFormName
//=============================================================================================================================================
{	// Um das Attachmentframe nutzen zu können muss der DboName sowie der DboKeyName übergeben werden
	String YYYY=PDS.TIMESTAMP.FormatString("yyyy");
	FRattachm->DboName="PROT_X10K_"+YYYY; FRattachm->DboKeyName="ID";	//Hier PROT_X10K_2024
	FRattachm->LoadList(IntToStr(ProtokolID),true); //void __fastcall TFrameMEMO::Activate(int ID, String OwnerFormName) //aus: FR_Memo ->Activate(PDS.PK_NR,"");
	FRattachm->AfterResize();
}


//1. Noch schauen wieder nicht Zahlenwerte raus nehmen
//2. wenn vom xml dann die ersten beiden eine Nummer geben dann 3. beachten wirlich
//3.Problem xml kennt keine ID und keine Prot_ID hier so wie ein neuen generieren

//Zunächst nur table1 !!!
//=============================================================================================================================================
void __fastcall TFrameX10K::CompleteXMLAndSend(int TestArt)
//=============================================================================================================================================
{
    int GridStart = 1;

	if( (TestArt == 1) && (xml4table1GUI) )
		GridStart = 0;
	if( (TestArt == 2) && (xml4table2GUI) )
		GridStart = 0;
	if( (TestArt == 3) && (xml4table3GUI) )
		GridStart = 0;

	bool bSendSQLLine = false;

    //int countTabSpalten4DB = 26;
	int markedASdestroyed = 0;
	bool gefunden = false;

	UnicodeString dbValue = L"";
	std::wstring dbName = L"";
	std::wstring dbNameOverHeaderCationOfGrid = L"";
	TColumnHeader* header;
	WideString headerCaption = L"";
	WideString uniValue = L"";
	std::wstring wuniValue = L"";

	if(TestArt == 1)
	{
		bSendSQLLine = Form1->SendSQLLine("SELECT * FROM dbo.X10K",QC1);

		if(!bSendSQLLine)
		{
			memoInfo->Color = clSkyBlue;
			memoInfo->Text = "Fehler: XML Daten können nicht versendet werden";
			return;
		}

		QC1->Open();
		QC1->Append();
	//----------------------------------

		QC1->FieldByName("PROT_ID")->AsInteger = ProtokolID;

		for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
				XML_Name2DataBase_Name.begin() + 1; it < (XML_Name2DataBase_Name.begin() + 5); ++it)
		{
			std::pair<std::wstring, std::wstring> compare = *it;
			std::wstring wcfirst = compare.first;
			std::wstring wcsecond = compare.second;

			gefunden = false;
			//Innere Schleife
			for(int indexGrid = GridStart; indexGrid < table1->Columns->Count; ++indexGrid)
			{
				header = table1->Columns->Item[indexGrid]->Header;
				headerCaption = L"";
				headerCaption = header->Caption;

				if(RRTools::WStrToAnsiString(wcsecond) == headerCaption)
				{
					gefunden = true;
					//Der Wert der vom User Editierten Zelle
					uniValue = table1->Columns->Item[indexGrid]->Field->AsWideString;  //RRR42 bisher
					std::wstring wtemp = RR4X10K::ExtractBefore(RRTools::AnsiToWide(uniValue), L" ");
					wuniValue = wtemp;
					uniValue = RRTools::WStrToAnsiString(wtemp);
					//___RRRTest
					String TESTuniValue = L"";
					TESTuniValue = uniValue;

					//double dconvert = StrToFloat(TESTuniValue);
					//___RRRTest
					break;
				}
			}

			//Erst nach der inneren Schleife wenn gefunden!!!
			if(gefunden)
			{
				QC1->FieldByName(RRTools::WStrToAnsiString(wcsecond))->AsFloat = RR4X10K::WstringToDouble(wuniValue);
			}
		}

		TDateTime  DTnow = Now().FormatString("dd.mm.yyyy hh:nn:ss");
		QC1->FieldByName("TS")->AsDateTime = DTnow;

		//2 mal smallint
		QC1->FieldByName("Test_Art")->AsInteger = TestArt;
		QC1->FieldByName("markedASdestroyed")->AsInteger = markedASdestroyed;

		//Von Index 8 an IX
		//davon alle bis Q float
		for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
				XML_Name2DataBase_Name.begin() + 8; it < XML_Name2DataBase_Name.end() - 4; ++it)
		{
			std::pair<std::wstring, std::wstring> compare = *it;
			std::wstring wcfirst = compare.first;
			std::wstring wcsecond = compare.second;

			gefunden = false;
			//Innere Schleife
			for(int indexGrid = GridStart; indexGrid < table1->Columns->Count; ++indexGrid)
			{
				header = table1->Columns->Item[indexGrid]->Header;
				headerCaption = L"";
				headerCaption = header->Caption;

				if(RRTools::WStrToAnsiString(wcsecond) == headerCaption)
				{
					gefunden = true;
					//Der Wert der vom User Editierten Zelle
					uniValue = table1->Columns->Item[indexGrid]->Field->AsWideString;  //RRR42 bisher
					std::wstring wtemp = RR4X10K::ExtractBefore(RRTools::AnsiToWide(uniValue), L" ");
					wuniValue = wtemp;
					uniValue = RRTools::WStrToAnsiString(wtemp);
					break;
				}
			}

			//Erst nach der inneren Schleife wenn gefunden!!!
			if(gefunden)
			{
				//double dconvert = StrToFloat(uniValue);
				//QC1->FieldByName(RRTools::WStrToAnsiString(wcsecond))->AsFloat =  dconvert; // RR4X10K::WstringToDouble(wuniValue);
				QC1->FieldByName(RRTools::WStrToAnsiString(wcsecond))->AsFloat = RR4X10K::WstringToDouble(wuniValue);
			}
		}

		if(NuEdFeuchte->Value != 0.0)
			QC1->FieldByName("Feuchte")->AsFloat = NuEdFeuchte->Value;
		if(NuEdTempU->Value != 0.0)
			QC1->FieldByName("TempU")->AsFloat = NuEdTempU->Value;
		if(NuEdTempDfg->Value != 0.0)
			QC1->FieldByName("TempDfg")->AsFloat = NuEdTempDfg->Value;
		if(Memo1->Text != L"")
			QC1->FieldByName("Kommentar")->AsString = Memo1->Text;

	//----------------------------------
		QC1->Post();
		QC1->Close();
	}

	if(TestArt == 2)
	{
		bSendSQLLine = Form1->SendSQLLine("SELECT * FROM dbo.X10K",QMA);

		if(!bSendSQLLine)
		{
			memoInfo->Color = clSkyBlue;
			memoInfo->Text = "Fehler: XML Daten können nicht versendet werden";
			return;
		}

		QMA->Open();
		QMA->Append();
	//----------------------------------

		QMA->FieldByName("PROT_ID")->AsInteger = ProtokolID;

		for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
				XML_Name2DataBase_Name.begin() + 1; it < (XML_Name2DataBase_Name.begin() + 5); ++it)
		{
			std::pair<std::wstring, std::wstring> compare = *it;
			std::wstring wcfirst = compare.first;
			std::wstring wcsecond = compare.second;

			gefunden = false;
			//Innere Schleife
			for(int indexGrid = GridStart; indexGrid < table2->Columns->Count; ++indexGrid)
			{
				header = table2->Columns->Item[indexGrid]->Header;
				headerCaption = L"";
				headerCaption = header->Caption;

				if(RRTools::WStrToAnsiString(wcsecond) == headerCaption)
				{
					gefunden = true;
					//Der Wert der vom User Editierten Zelle
					uniValue = table2->Columns->Item[indexGrid]->Field->AsWideString;  //RRR42 bisher
					std::wstring wtemp = RR4X10K::ExtractBefore(RRTools::AnsiToWide(uniValue), L" ");
					wuniValue = wtemp;
					uniValue = RRTools::WStrToAnsiString(wtemp);
					break;
				}
			}

			//Erst nach der inneren Schleife wenn gefunden!!!
			if(gefunden)
			{
				QMA->FieldByName(RRTools::WStrToAnsiString(wcsecond))->AsFloat = RR4X10K::WstringToDouble(wuniValue);
			}
		}

		TDateTime  DTnow = Now().FormatString("dd.mm.yyyy hh:nn:ss");
		QMA->FieldByName("TS")->AsDateTime = DTnow;

		//2 mal smallint
		QMA->FieldByName("Test_Art")->AsInteger = TestArt;
		QMA->FieldByName("markedASdestroyed")->AsInteger = markedASdestroyed;

		//Von Index 8 an IX
		//davon alle bis Q float
		for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
				XML_Name2DataBase_Name.begin() + 8; it < XML_Name2DataBase_Name.end() - 4; ++it)
		{
			std::pair<std::wstring, std::wstring> compare = *it;
			std::wstring wcfirst = compare.first;
			std::wstring wcsecond = compare.second;

			gefunden = false;
			//Innere Schleife
			for(int indexGrid = GridStart; indexGrid < table2->Columns->Count; ++indexGrid)
			{
				header = table2->Columns->Item[indexGrid]->Header;
				headerCaption = L"";
				headerCaption = header->Caption;

				if(RRTools::WStrToAnsiString(wcsecond) == headerCaption)
				{
					gefunden = true;
					//Der Wert der vom User Editierten Zelle
					uniValue = table2->Columns->Item[indexGrid]->Field->AsWideString;  //RRR42 bisher
					std::wstring wtemp = RR4X10K::ExtractBefore(RRTools::AnsiToWide(uniValue), L" ");
					wuniValue = wtemp;
					uniValue = RRTools::WStrToAnsiString(wtemp);
					break;
				}
			}

			//Erst nach der inneren Schleife wenn gefunden!!!
			if(gefunden)
			{
				QMA->FieldByName(RRTools::WStrToAnsiString(wcsecond))->AsFloat = RR4X10K::WstringToDouble(wuniValue);
			}
		}

		if(NuEdFeuchte->Value != 0.0)
			QMA->FieldByName("Feuchte")->AsFloat = NuEdFeuchte->Value;
		if(NuEdTempU->Value != 0.0)
			QMA->FieldByName("TempU")->AsFloat = NuEdTempU->Value;
		if(NuEdTempDfg->Value != 0.0)
			QMA->FieldByName("TempDfg")->AsFloat = NuEdTempDfg->Value;
		if(Memo1->Text != L"")
			QMA->FieldByName("Kommentar")->AsString = Memo1->Text;

	//----------------------------------
		QMA->Post();
		QMA->Close();
	}

	if(TestArt == 3)
	{
		bSendSQLLine = Form1->SendSQLLine("SELECT * FROM dbo.X10K",QTAP);

		if(!bSendSQLLine)
		{
			memoInfo->Color = clSkyBlue;
			memoInfo->Text = "Fehler: XML Daten können nicht versendet werden";
			return;
		}

		QTAP->Open();
		QTAP->Append();
	//----------------------------------

		QTAP->FieldByName("PROT_ID")->AsInteger = ProtokolID;

		for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
				XML_Name2DataBase_Name.begin() + 1; it < (XML_Name2DataBase_Name.begin() + 5); ++it)
		{
			std::pair<std::wstring, std::wstring> compare = *it;
			std::wstring wcfirst = compare.first;
			std::wstring wcsecond = compare.second;

			gefunden = false;
			//Innere Schleife
			for(int indexGrid = GridStart; indexGrid < table3->Columns->Count; ++indexGrid)
			{
				header = table3->Columns->Item[indexGrid]->Header;
				headerCaption = L"";
				headerCaption = header->Caption;

				if(RRTools::WStrToAnsiString(wcsecond) == headerCaption)
				{
					gefunden = true;
					//Der Wert der vom User Editierten Zelle
					uniValue = table3->Columns->Item[indexGrid]->Field->AsWideString;  //RRR42 bisher
					std::wstring wtemp = RR4X10K::ExtractBefore(RRTools::AnsiToWide(uniValue), L" ");
					wuniValue = wtemp;
					uniValue = RRTools::WStrToAnsiString(wtemp);
					break;
				}
			}

			//Erst nach der inneren Schleife wenn gefunden!!!
			if(gefunden)
			{
				QTAP->FieldByName(RRTools::WStrToAnsiString(wcsecond))->AsFloat = RR4X10K::WstringToDouble(wuniValue);
			}
		}

		TDateTime  DTnow = Now().FormatString("dd.mm.yyyy hh:nn:ss");
		QTAP->FieldByName("TS")->AsDateTime = DTnow;

		//2 mal smallint
		QTAP->FieldByName("Test_Art")->AsInteger = TestArt;
		QTAP->FieldByName("markedASdestroyed")->AsInteger = markedASdestroyed;

		//Von Index 8 an IX
		//davon alle bis Q float
		for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
				XML_Name2DataBase_Name.begin() + 8; it < XML_Name2DataBase_Name.end() - 4; ++it)
		{
			std::pair<std::wstring, std::wstring> compare = *it;
			std::wstring wcfirst = compare.first;
			std::wstring wcsecond = compare.second;

			gefunden = false;
			//Innere Schleife
			for(int indexGrid = GridStart; indexGrid < table3->Columns->Count; ++indexGrid)
			{
				header = table3->Columns->Item[indexGrid]->Header;
				headerCaption = L"";
				headerCaption = header->Caption;

				if(RRTools::WStrToAnsiString(wcsecond) == headerCaption)
				{
					gefunden = true;
					//Der Wert der vom User Editierten Zelle
					uniValue = table3->Columns->Item[indexGrid]->Field->AsWideString;  //RRR42 bisher
					std::wstring wtemp = RR4X10K::ExtractBefore(RRTools::AnsiToWide(uniValue), L" ");
					wuniValue = wtemp;
					uniValue = RRTools::WStrToAnsiString(wtemp);
					break;
				}
			}

			//Erst nach der inneren Schleife wenn gefunden!!!
			if(gefunden)
			{
				QTAP->FieldByName(RRTools::WStrToAnsiString(wcsecond))->AsFloat = RR4X10K::WstringToDouble(wuniValue);
			}
		}

		if(NuEdFeuchte->Value != 0.0)
			QTAP->FieldByName("Feuchte")->AsFloat = NuEdFeuchte->Value;
		if(NuEdTempU->Value != 0.0)
			QTAP->FieldByName("TempU")->AsFloat = NuEdTempU->Value;
		if(NuEdTempDfg->Value != 0.0)
			QTAP->FieldByName("TempDfg")->AsFloat = NuEdTempDfg->Value;
		if(Memo1->Text != L"")
			QTAP->FieldByName("Kommentar")->AsString = Memo1->Text;

	//----------------------------------
		QTAP->Post();
		QTAP->Close();
	}

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
	std::pair<std::wstring , std::wstring> data8(L"Ix", L"Ix");     // Index 8
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
	std::pair<std::wstring , std::wstring> data14(L"P10kV", L"P10KV");
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
	std::pair<std::wstring , std::wstring> data21(L"Q", L"Q");  // Index 21
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


//Achtung geht nur wenn nicht im xml weil ja erstmal die ID fehlt
//Nur für Nicht XML !!!
void __fastcall TFrameX10K::table1OnAfterEdit(TObject *Sender, int ACol, int ARow,
		  WideString Value)
{
	if( (ARow < 0) || (ACol < 0) )
		return;

	if(xml4table1GUI)
		return;

	int TestArt = 1;

	String cellIDValue = "";

	TColumnHeader* header;
	String headerCaption = L"";

	String uniValue = L"";
	std::wstring wuniValue = L"";

	header = table1->Columns->Item[ACol]->Header;
	headerCaption = L"";
	headerCaption = header->Caption;

	cellIDValue = table1->GetCellValue(0, ARow);

	uniValue = table1->Columns->Item[ACol]->Field->AsString;
	double dconvert = StrToFloat(uniValue);

	if(cellIDValue != "")
	{

		QC1->SQL->Text = "SELECT * FROM X10K WHERE ID=" + cellIDValue + " AND Test_Art ="+IntToStr(TestArt);

		QC1->Open();
		QC1->Edit();
		QC1->FieldByName(headerCaption)->AsFloat = dconvert;

		if(NuEdFeuchte->Value != 0.0)
			QC1->FieldByName("Feuchte")->AsFloat = NuEdFeuchte->Value;
		if(NuEdTempU->Value != 0.0)
			QC1->FieldByName("TempU")->AsFloat = NuEdTempU->Value;
		if(NuEdTempDfg->Value != 0.0)
			QC1->FieldByName("TempDfg")->AsFloat = NuEdTempDfg->Value;
		if(Memo1->Text != L"")
			QC1->FieldByName("Kommentar")->AsString = Memo1->Text;

		QC1->Post();
		QC1->Close();

		//Neu Aufbau nach dem Update
		QC1->SQL->Text = "SELECT TOP 100 PERCENT * FROM X10K WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt);

		QC1->Open();
	}
}

void __fastcall TFrameX10K::table2OnAfterEdit(TObject *Sender, int ACol, int ARow,
		  WideString Value)
{
	if( (ARow < 0) || (ACol < 0) )
		return;

	if(xml4table2GUI)
		return;

	int TestArt = 2;

    String cellIDValue = "";

	TColumnHeader* header;
	String headerCaption = L"";

	String uniValue = L"";
	std::wstring wuniValue = L"";

	header = table2->Columns->Item[ACol]->Header;
	headerCaption = L"";
	headerCaption = header->Caption;

	cellIDValue = table2->GetCellValue(0, ARow);

	uniValue = table2->Columns->Item[ACol]->Field->AsString;
	double dconvert = StrToFloat(uniValue);

	if(cellIDValue != "")
	{

		QMA->SQL->Text = "SELECT * FROM X10K WHERE ID=" + cellIDValue + " AND Test_Art ="+IntToStr(TestArt);

		QMA->Open();
		QMA->Edit();
		QMA->FieldByName(headerCaption)->AsFloat = dconvert;

		if(NuEdFeuchte->Value != 0.0)
			QMA->FieldByName("Feuchte")->AsFloat = NuEdFeuchte->Value;
		if(NuEdTempU->Value != 0.0)
			QMA->FieldByName("TempU")->AsFloat = NuEdTempU->Value;
		if(NuEdTempDfg->Value != 0.0)
			QMA->FieldByName("TempDfg")->AsFloat = NuEdTempDfg->Value;
		if(Memo1->Text != L"")
			QMA->FieldByName("Kommentar")->AsString = Memo1->Text;

		QMA->Post();
		QMA->Close();

		//Neu Aufbau nach dem Update
		QMA->SQL->Text = "SELECT TOP(200) * FROM X10K WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt);

		QMA->Open();
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnAfterEdit(TObject *Sender, int ACol, int ARow,
		  WideString Value)
{
    if( (ARow < 0) || (ACol < 0) )
		return;

	if(xml4table3GUI)
		return;

	int TestArt = 3;

	String cellIDValue = "";

	TColumnHeader* header;
	String headerCaption = L"";

	String uniValue = L"";
	std::wstring wuniValue = L"";

	header = table3->Columns->Item[ACol]->Header;
	headerCaption = L"";
	headerCaption = header->Caption;

	cellIDValue = table3->GetCellValue(0, ARow);

	uniValue = table3->Columns->Item[ACol]->Field->AsString;
	double dconvert = StrToFloat(uniValue);

	QTAP->SQL->Text = "SELECT * FROM X10K WHERE ID=" + cellIDValue + " AND Test_Art ="+IntToStr(TestArt);

	QTAP->Open();
	QTAP->Edit();
	QTAP->FieldByName(headerCaption)->AsFloat = dconvert;

	if(NuEdFeuchte->Value != 0.0)
		QTAP->FieldByName("Feuchte")->AsFloat = NuEdFeuchte->Value;
	if(NuEdTempU->Value != 0.0)
		QTAP->FieldByName("TempU")->AsFloat = NuEdTempU->Value;
	if(NuEdTempDfg->Value != 0.0)
		QTAP->FieldByName("TempDfg")->AsFloat = NuEdTempDfg->Value;
	if(Memo1->Text != L"")
		QTAP->FieldByName("Kommentar")->AsString = Memo1->Text;

	QTAP->Post();
	QTAP->Close();

	//Neu Aufbau nach dem Update
	QTAP->SQL->Text = "SELECT TOP(200) * FROM X10K WHERE PROT_ID="+IntToStr(ProtokolID)+" AND Test_Art ="+IntToStr(TestArt);

	QTAP->Open();
}

//---------------------------------------------------------------------------
//RRR42 hier schauen was bei nornalen nicht XML-Datensatz
void __fastcall TFrameX10K::table1RecZurDBOnClick(TObject *Sender)
{
	//Beim test erstmal raus sollte besser im großen Projekt getesttet werden
	if(!CheckAllFieldsSet())
		return;

	if(xml4table1GUI)
	{
		CompleteXMLAndSend(1);
		if(table1->SelectedRow >= 0)
		{
			int index = table1->SelectedRow;
			std::pair<int, int> pairSelected = table1RememberOfXML[index];
			pairSelected.first = 0;
			pairSelected.second = 0;
			table1RememberOfXML[index] = pairSelected;
			table1->RefreshRow(table1->SelectedRow);
			//Hier noch die edituerte Row im table1RememberOfXML entsprechend first auf 0 setzen.
//HandleCountOfUnusedXMLRecords(1);     //RRRDOPPELT
			//------------------------------------------
			int coutsOfUnusedXMLRows = HandleCountOfUnusedXMLRecords(1);
			if(coutsOfUnusedXMLRows == 0)
			{
				if( table1 )
				{
					TMSQuery* Qtemp1 = NULL;
					TMSDataSource *DStemp1 = NULL;
					Qtemp1 = new TMSQuery(NULL);
					DStemp1 = new TMSDataSource(NULL);

					table1->BeginUpdate();
					table1->DataSource = DStemp1;
					DStemp1->DataSet=Qtemp1;
					table1->EndUpdate();


					if(QC1)
						delete QC1;
					QC1 = NULL;

					if(QC1==NULL)  {QC1=new TMSQuery(NULL);  table1->DataSource = DS; DS->DataSet=QC1;}              // Wenn der Zeiger QC1=0, soll eine neue Instanz von

					Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =1 AND markedASdestroyed != 1 ",QC1);

					if(Qtemp1)
						delete Qtemp1;
					if(DStemp1)
						delete DStemp1;
				}
			}
			//------------------------------------------
		}
		else
		{
			ShowMessage("TEST: table1RecZurDBOnClick: Keine gültige selectRow");
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2RecZurDBOnClick(TObject *Sender)
{
	//Beim test erstmal raus sollte besser im großen Projekt getesttet werden
	if(!CheckAllFieldsSet())
		return;

	if(xml4table2GUI)
	{
		CompleteXMLAndSend(2);
		if(table2->SelectedRow >= 0)
		{
			int index = table2->SelectedRow;
			std::pair<int, int> pairSelected = table2RememberOfXML[index];
			pairSelected.first = 0;
			pairSelected.second = 0;
			table2RememberOfXML[index] = pairSelected;
			table2->RefreshRow(table2->SelectedRow);
			//Hier noch die edituerte Row im table1RememberOfXML entsprechend first auf 0 setzen.
//HandleCountOfUnusedXMLRecords(2); //RRRDOPPELT
			//------------------------------------------
			int coutsOfUnusedXMLRows = HandleCountOfUnusedXMLRecords(2);
			if(coutsOfUnusedXMLRows == 0)
			{
				if( table2 )
				{
					TMSQuery* Qtemp1 = NULL;
					TMSDataSource *DStemp1 = NULL;
					Qtemp1 = new TMSQuery(NULL);
					DStemp1 = new TMSDataSource(NULL);

					table2->BeginUpdate();
					table2->DataSource = DStemp1;
					DStemp1->DataSet=Qtemp1;
					table2->EndUpdate();


					if(QMA)
						delete QMA;
					QMA = NULL;


					if(QMA==NULL)  {QMA=new TMSQuery(NULL);  table2->DataSource = DS1; DS1->DataSet=QMA;}   			// TMSQuery mit dem Namen QC1 erzeugt und mit der

					Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =2 AND markedASdestroyed != 1 ",QMA);

					if(Qtemp1)
						delete Qtemp1;
					if(DStemp1)
						delete DStemp1;
				}
			}
			//------------------------------------------
		}
		else
		{
			ShowMessage("TEST: table2RecZurDBOnClick: Keine gültige selectRow");
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3RecZurDBOnClick(TObject *Sender)
{
    //Beim test erstmal raus sollte besser im großen Projekt getesttet werden
	if(!CheckAllFieldsSet())
		return;

	if(xml4table3GUI)
	{
		CompleteXMLAndSend(3);
		if(table3->SelectedRow >= 0)
		{
			int index = table3->SelectedRow;
			std::pair<int, int> pairSelected = table3RememberOfXML[index];
			pairSelected.first = 0;
			pairSelected.second = 0;
			table3RememberOfXML[index] = pairSelected;
			table3->RefreshRow(table3->SelectedRow);
			//Hier noch die edituerte Row im table3RememberOfXML entsprechend first auf 0 setzen.
//HandleCountOfUnusedXMLRecords(1);
			//------------------------------------------
			int coutsOfUnusedXMLRows = HandleCountOfUnusedXMLRecords(3);
			if(coutsOfUnusedXMLRows == 0)
			{
				if( table3 )
				{

					TMSQuery* Qtemp1 = NULL;
					TMSDataSource *DStemp1 = NULL;
					Qtemp1 = new TMSQuery(NULL);
					DStemp1 = new TMSDataSource(NULL);

					table3->BeginUpdate();
					table3->DataSource = DStemp1;
					DStemp1->DataSet=Qtemp1;
					table3->EndUpdate();


					if(QTAP)
						delete QTAP;
					QTAP = NULL;

					if(QTAP==NULL) {QTAP=new TMSQuery(NULL); table3->DataSource = DS2; DS2->DataSet=QTAP;}

					Form1->SendSQLLine("SELECT * FROM MDE.dbo.X10K WHERE PROT_ID="+IntToStr(ProtokolID) + " AND Test_Art =3 AND markedASdestroyed != 1 ",QTAP);

					if(Qtemp1)
						delete Qtemp1;
					if(DStemp1)
						delete DStemp1;
				}
			}
			//------------------------------------------
		}
		else
		{
			ShowMessage("TEST: table3RecZurDBOnClick: Keine gültige selectRow");
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::ComBoxMessgOnClick(TObject *Sender)
{
    if(!CheckAllFieldsSet())
		return;
	SetChangeFlag();
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::ComBoxAufStaticOnClick(TObject *Sender)
{
	if(!CheckAllFieldsSet())
		return;
	SetChangeFlag();
}
//---------------------------------------------------------------------------


void __fastcall TFrameX10K::EditOrtOnChange(TObject *Sender)
{
    if(!CheckAllFieldsSet())
		return;
	SetChangeFlag();
}
//---------------------------------------------------------------------------

//RRR999
int __fastcall TFrameX10K::HandleCountOfUnusedXMLRecords(int tableNr)
{
	int ifBTNtoXMLEnabled = 0;
	if(tableNr == 1)
	{
		int index = 0;

		for(std::vector<std::pair<int, int> >::iterator it =
			table1RememberOfXML.begin(); it != table1RememberOfXML.end(); ++it)
		{
			std::pair<int, int> compare = *it;

			if(compare.first == 1)
			{
				++index;
			}
		}


		if(!ReadOnly)
		{

			if(index > 0)
				lbTable1CountOfXMLUnused->Caption = "Anzahl nicht bearbeiteter XML-Einträge :" + IntToStr(index);
			else
				lbTable1CountOfXMLUnused->Caption = "";


			if(xml4table1GUI)
			{
				if(index == 0)
				{
					xml4table1GUI = false;
					SpBtnC1->Enabled = true;
					table1RecZurDB->Enabled = false;
				}
				else if(index > 0)
				{
					SpBtnC1->Enabled = false;
					table1RecZurDB->Enabled = true;
				}
			}
			else
			{
				SpBtnC1->Enabled = true;
				table1RecZurDB->Enabled = false;
			}


			ifBTNtoXMLEnabled = GetCountOfUnusedXMLRecords(1) + GetCountOfUnusedXMLRecords(2) + GetCountOfUnusedXMLRecords(3);

			if(ifBTNtoXMLEnabled == 0)
				spbToXML->Enabled = true;
		}
		return index;
	}

	if(tableNr == 2)
	{
		int index = 0;

		for(std::vector<std::pair<int, int> >::iterator it =
			table2RememberOfXML.begin(); it != table2RememberOfXML.end(); ++it)
		{
			std::pair<int, int> compare = *it;

			if(compare.first == 1)
			{
				++index;
			}
		}

		if(!ReadOnly)
		{

			if(index > 0)
				lbTable2CountOfXMLUnused->Caption = "Anzahl nicht bearbeiteter XML-Einträge :" + IntToStr(index);
			else
				lbTable2CountOfXMLUnused->Caption = "";

			if(xml4table2GUI)
			{
				if(index == 0)
				{
					xml4table2GUI = false;
					SpBtnMA->Enabled = true;
					table2RecZurDB->Enabled = false;
				}
				else if(index > 0)
				{
					SpBtnMA->Enabled = false;
					table2RecZurDB->Enabled = true;
				}
			}
			else
			{
				SpBtnMA->Enabled = true;
				table2RecZurDB->Enabled = false;
			}


			ifBTNtoXMLEnabled = GetCountOfUnusedXMLRecords(1) + GetCountOfUnusedXMLRecords(2) + GetCountOfUnusedXMLRecords(3);

			if(ifBTNtoXMLEnabled == 0)
				spbToXML->Enabled = true;
		}
		return index;
	}

	if(tableNr == 3)
	{
		int index = 0;

		for(std::vector<std::pair<int, int> >::iterator it =
			table3RememberOfXML.begin(); it != table3RememberOfXML.end(); ++it)
		{
			std::pair<int, int> compare = *it;

			if(compare.first == 1)
			{
				++index;
			}
		}

		if(!ReadOnly)
		{

			if(index > 0)
				lbTable3CountOfXMLUnused->Caption = "Anzahl nicht bearbeiteter XML-Einträge :" + IntToStr(index);
			else
				lbTable3CountOfXMLUnused->Caption = "";

			if(xml4table3GUI)
			{
				if(index == 0)
				{
					xml4table3GUI = false;
					SpBtnTAP->Enabled = true;
					table3RecZurDB->Enabled = false;
				}
				else if(index > 0)
				{
					SpBtnTAP->Enabled = false;
					table3RecZurDB->Enabled = true;
				}
			}
			else
			{
				SpBtnTAP->Enabled = true;
				table3RecZurDB->Enabled = false;

			}

			ifBTNtoXMLEnabled = GetCountOfUnusedXMLRecords(1) + GetCountOfUnusedXMLRecords(2) + GetCountOfUnusedXMLRecords(3);

			if(ifBTNtoXMLEnabled == 0)
				spbToXML->Enabled = true;

		}
		return index;
	}

	return -1; //Wenn Fehler : Nr. des table gibt es nicht; sonst damit immer ein return stattfidet. CLang fordert das
}

int __fastcall TFrameX10K::GetCountOfUnusedXMLRecords(int tableNr)
{
	if(tableNr == 1)
	{
		int index = 0;
		for(std::vector<std::pair<int, int> >::iterator it =
			table1RememberOfXML.begin(); it != table1RememberOfXML.end(); ++it)
		{
			std::pair<int, int> compare = *it;

			if(compare.first == 1)
			{
				++index;
			}
		}
		return index;
	}

	if(tableNr == 2)
	{
		int index = 0;
		for(std::vector<std::pair<int, int> >::iterator it =
			table2RememberOfXML.begin(); it != table2RememberOfXML.end(); ++it)
		{
			std::pair<int, int> compare = *it;

			if(compare.first == 1)
			{
				++index;
			}
		}
		return index;
	}

	if(tableNr == 3)
	{
		int index = 0;
		for(std::vector<std::pair<int, int> >::iterator it =
			table3RememberOfXML.begin(); it != table3RememberOfXML.end(); ++it)
		{
			std::pair<int, int> compare = *it;

			if(compare.first == 1)
			{
				++index;
			}
		}
		return index;
	}

	return -1; //wenn fehler ungültige table-number
}

void __fastcall TFrameX10K::MyMessageBox()   //MyMessageBox
{
// MessageBox anzeigen
	int response = MessageBox(
		Handle,                       // Handle zum Eigentümerfenster
		L"Möchten Sie fortfahren?",   // Nachrichtentext
		L"Bestätigung",               // Titel der MessageBox
		MB_YESNOCANCEL | MB_ICONQUESTION // Buttons: MB_OKCANCEL OK und Cancel, Symbol: Fragezeichen
    );

	switch(response)
	{
	// Reaktion auf die Antwort
		case IDYES:
		ShowMessage("Sie haben auf OK geklickt.");
		break;

		case IDNO:
		ShowMessage("Sie haben auf Nein geklickt.");
		break;

		case IDCANCEL:
		ShowMessage("Sie haben auf Abbrechen geklickt.");
		break;

	}

}

void __fastcall TFrameX10K::NuEdTempUOnChange(TObject *Sender)
{
	if(!CheckAllFieldsSet())
		return;
	SetChangeFlag();
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::NuEdTempDfgOnChange(TObject *Sender)
{
    if(!CheckAllFieldsSet())
		return;
	SetChangeFlag();
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::NuEdFeuchteOnChange(TObject *Sender)
{
	if(!CheckAllFieldsSet())
		return;
	SetChangeFlag();
}
//---------------------------------------------------------------------------


void __fastcall TFrameX10K::AttachManagerOnDropFiles(TObject *Sender)
{
	if(Sender != NULL)
		FRattachm->DropFilesDropFiles(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::Memo1OnChange(TObject *Sender)
{
    if(!CheckAllFieldsSet())
		return;
	SetChangeFlag();
}
//---------------------------------------------------------------------------


void __fastcall TFrameX10K::table1OnVerticalScroll(TObject *Sender, int Position)
{
	int tempPosition = Position;
		if(xml4table1GUI)
	{
        //beim einmaligen ersten erstellen. SElect vom User fehlt.
		if(tempPosition == 0)
		{
			SpBtnC1_Delete->Enabled = false;
			table1RecZurDB->Enabled = false;
			//tempPosition = 1;
		}
		//table1->DataSource->DataSet->RecNo = tempPosition;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table2OnVerticalScroll(TObject *Sender, int Position)

{
	int tempPosition = Position;
	if(tempPosition == 0)
		tempPosition = 1;
	if(xml4table2GUI)
		table2->DataSource->DataSet->RecNo = tempPosition;
}
//---------------------------------------------------------------------------

void __fastcall TFrameX10K::table3OnVerticalScroll(TObject *Sender, int Position)

{
    int tempPosition = Position;
	if(tempPosition == 0)
		tempPosition = 1;
	if(xml4table3GUI)
		table3->DataSource->DataSet->RecNo = tempPosition;
}
//---------------------------------------------------------------------------

/*
NOTIZ
//=============================================================================================================================================
void __fastcall TForm18::LIgruppenClick(TObject *Sender)
//=============================================================================================================================================
{
int m,n;
AnsiString apl,name,txt,group;

	name=""; group=""; LIapls->Items->Clear(); LIstoerungen->Items->Clear(); ListeKurz8->Clear(); ListeApls->Clear();
	LIapls->ItemIndex=-1; LIgruppen2->ItemIndex=-1; PNemail->Visible=false;

	if(DBM.Connected && LIgruppen->ItemIndex>=0)
	{
		group=LIgruppen->Items->Strings[LIgruppen->ItemIndex];

		DBM.SendSQLLine("SELECT * FROM dbo.APLs WHERE Name <> 'Gruppe' AND Gruppe='"+group+"'", NULL);
		while(!DBM.GetNextRow(NULL))
		{
		  apl  =DBM.Query->FieldByName("APL")->AsString.Trim();
		  name =DBM.Query->FieldByName("Name")->AsString;
		  LIapls->Items->Add(apl+"   "+name);
		  ListeApls->Add(apl);
		}

		if(LIapls->Items->Count==0)
		{
		   LIapls->Items->Add("Gruppe "+LIgruppen->Items->Strings[LIgruppen->ItemIndex]);
		   ListeApls    ->Add("Gruppe "+LIgruppen->Items->Strings[LIgruppen->ItemIndex]);
		}

		DBM.SendSQLLine("SELECT * FROM dbo.APLs WHERE Name = 'Gruppe' AND Gruppe='"+group+"'", NULL);  // Kürzel für gewählte Gruppe ermitteln
		if(!DBM.GetNextRow(NULL)) group=DBM.Query->FieldByName("APL")->AsString;
		else                      group="";

		if(group!="")
		{
		  DBM.SendSQLLine("SELECT * FROM dbo.SONDERPROZ1 WHERE BetrGruppen = 'ALLE' OR BetrGruppen LIKE '%"+group.Trim()+"%' ORDER BY KURZ8", NULL);
		  while(!DBM.GetNextRow(NULL))                                                            // vormals auch KBFEHLER für Abweichungsmeldungen
		  {
			ListeKurz8         ->Add(DBM.Query->FieldByName("KURZ8")  ->AsString);
			LIstoerungen->Items->Add(DBM.Query->FieldByName("TEXT500")->AsString);
		  }

		}

	}

	UpdateScreen(Sender);
}
*/


