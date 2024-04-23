//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Frame01.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "NxCustomGrid"
#pragma link "NxCustomGridControl"
#pragma link "NxDBGrid"
#pragma link "NxScrollControl"
#pragma link "NxGrid"
#pragma link "NxColumns"
#pragma link "NxDBColumns"
#pragma link "DBAccess"
#pragma link "MemDS"
#pragma link "MSAccess"
#pragma resource "*.dfm"
TFrame4XML *Frame4XML;
//---------------------------------------------------------------------------




__fastcall TFrame4XML::TFrame4XML(TComponent* Owner)
	: TFrame(Owner)//, Form4XML(new TFormXML(this))
{
	BereitZumSQL_Quest();
}

//Bitte im großen Projekt prüfen
__fastcall TFrame4XML::~TFrame4XML()
{
	ShowMessage("Destruktor : TFrame4XML::~TFrame4XML()");

	if(QC1)
		delete QC1;
	QC1=NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFrame4XML::cbnXMLOnClick(TObject *Sender)
{
	//ShowMyForm();
	CreateAndShowForm();
}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::CreateAndShowForm()
{
	//TFormXML::instanceCount = 0;

	// Eine Instanz von TFormXML erhalten
	//TFormXML* form
	TFormXML* Form4XML = NULL;
	Application->CreateForm(__classid(TFormXML), &Form4XML);
	//Form4XML = TFormXML::getInstance(this); // Annahme: this ist ein Frame
	// Das Formular anzeigen
	Form4XML->MyFrame = this;    //Im großen Projekt global sein

//NgDB1->DataSource = Form4XML->DataSource5;
	Form4XML->ShowModal();

}

//----------------------------------------------------------------------------
//Teil zum testen in die Datenbank zu schreiben der Wechsel
void __fastcall TFrame4XML::IniDB()
{
	//MSCon = new TMSConnection(NULL);    nicht nötig von C++ Builder verwaltet
	try
	{
		MSCon->Server = "detoic008A";
		MSCon->Authentication = auWindows; //auServer;
		//MSCon->Username = ""; //nicht nötig da auWindows
		//MSCon->Password = ""; //nicht nötig da auWindows
		MSCon->Database = "Nordwind";
		MSCon->LoginPrompt = !False;
		MSCon->Connect();
	}
	catch (const Exception &e)
	{
		ShowMessage("Fehler Beim Aufbau der Datenbank: " + e.Message);
	}
}

//QSTab->SQL->Add("SELECT * FROM [Nordwind].[dbo].[rrNordwind]");

void __fastcall TFrame4XML::BereitZumSQL_Quest()
{
	QC1 = NULL;
	QC1 = new TMSQuery(NULL);
	//QSTab=NULL;
	//QSTab = new TMSQuery(NULL);

	IniDB();
	QC1->Connection = MSCon;
	DS->DataSet = QC1;
	//QSTab->Connection = MSCon;
	//DSRC->DataSet = QSTab;
	//Bemerkung das TNextDB ist schon im Designer mit DS verbunden

	//SendSQLLine("SELECT * FROM [Nordwind].[dbo].[rrNordwind]",QC1,1);
}


void __fastcall TFrame4XML::cbnSQL_Query(TObject *Sender)
{
    QC1->Connection = MSCon;
	DS->DataSet = QC1;
	NgDB1->DataSource = DS;
	SendSQLLine("SELECT * FROM [Nordwind].[dbo].[rrNordwind]",QC1,1);
}
//---------------------------------------------------------------------------

int _fastcall TFrame4XML::SendSQLLine(AnsiString Line, TMSQuery *SQ, int showmessage) // Nur gültig für Haupt-DB (MDE)
{
   /*	if(DBM.Connected==false)
	{
		if(showmessage)
				if(showmessage) ShowMessage("Keine Verbindung " + AnsiString(e.what()));
		return 1;
	}

	if(Form00_SU->Visible && Form00_SU->CBallowMonitorSQL->Checked) Form00_SU->MMdb->Lines->Add("     SQL: "+Line);

	if(SQ->Connection!=DBM.Connection) SQ->Connection=DBM.Connection;
	*/
	SQ->Connection = MSCon;

	SQ->Close();
	SQ->SQL->Clear();
	SQ->SQL->Add(Line);

	try
	{
	  SQ->ExecSQL();

	  if(Line.Pos("SELECT ")==1)
	  {
		 if   (SQ->Eof)
		 {
			if(showmessage) ShowMessage("SQLException Ende der Tabelle erreicht: "); // + AnsiString(e.what()));
				return 2;
		 }
		 else {SQ->First(); return 0;}
	  }
	  else                  return 0;
	}
	catch (...)
	{

		if(showmessage) ShowMessage("Allgemeines Problem mit SQL: "); // + AnsiString(e.what()));
		return 1;
	}
}

/*
TClientDataSet *CDS_Test;
	TMSDataSource *DS;
	TMSQuery *QC1;

	if(QC1==NULL)  {QC1=new TMSQuery(NULL);  DS->DataSet=QC1;}
	*/
