Wenn Problem mit ClientDataSet oder ähnlichem DataSet
/////////////////////////////////////////////////////
procedure TNxGridDataLink.EditingChanged;
begin
  { Desc: occur on DataSet.Edit }
  inherited;
  { Do not edit cell and steal focus from other component }
  if not FGrid.Focused then Exit;
  if not Assigned(DataSource) or FChanging then Exit;
  if DataSet.State = dsEdit then
  begin
    try
      FCanEdit := True;
      FGrid.EditCell(FGrid.SelectedColumn, FGrid.SelectedRow);
    finally
      FCanEdit := False;
    end;
  end else
  begin
    FGrid.EndEditing;
  end;
end;




//Interessant das erste nach Close aber vor Frame Zerstörung
void __fastcall TFormXML::OnDestroy(TObject *Sender)
{
	if(ClientDataSet1->Active)
		ClientDataSet1->Close();
	if(ClientDataSet2->Active)
		ClientDataSet2->Close();
	if(ClientDataSet3->Active)
		ClientDataSet3->Close();
	if(ClientDataSet4->Active)
		ClientDataSet4->Close();
	if(ClientDataSet5->Active)
		ClientDataSet5->Close();

	//ShowMessage("OnDestroy");
}

void __fastcall TFormXML::Transfer2DB_C1()
{
//...
	AddOtherRowsDB_C1();
MyFrame->PracticalHeaderName();
//...
}

//------------------------

bool EditAccept;

void __fastcall PracticalHeaderName();
void __fastcall PracticalValues();

//------------------------

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
	Row = -1; Col = -1; Col4Red = -1;
	//es seien die Rechte gegeben

	NgDB->ReadOnly = false;
	EditAccept = false;
	Dummy4Focus->Visible = false;
	//Button3->Visible = false;
}

//Bitte im großen Projekt prüfen
__fastcall TFrame4XML::~TFrame4XML()
{
	//ShowMessage("Destruktor : TFrame4XML::~TFrame4XML()");

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

//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

//Zur Zeit nichts gezeichnet
void __fastcall TFrame4XML::NgDBOnCellClick(TObject *Sender, int ACol, int ARow)
{
	TField* Field;
	int Spalte = -1;
	bool gefunden = false;
	for (int i = 0; i < NgDB->Columns->Count ; ++i)
	{
		++Spalte;
		TColumnHeader* header = NgDB->Columns->Item[i]->Header;
		//Dies wäre der Object-Art-Name
		UnicodeString unistr = header->UnitName();
		Field = NgDB->Columns->Item[i]->Field;
		//Feldname wie im DataSet !!!
		unistr = Field->FieldName;
		Field->ReadOnly = false;
		//zum Vergleich aber hier nicht nötig. Dies sieht der User vom Grid
		AnsiString wstr = header->Caption;
		if(unistr == U"Ix"){  // Ix //Im wirklichen Szenario wäre dies die markedAsDestroyed-Spalte

			Col = Spalte; Row = ARow; Col4Red = Spalte;
			gefunden = true;
			break;
		}

	 }

	//NgDB->Columns->Column[Col]->Options << coEditing;

	 //wird nicht abgefangen wenn nichts gefunden wird bitte imm Programm prüfen !!!
	if(!gefunden)
		return;

	QC1->Connection = MSCon;
	DS->DataSet = QC1;
	NgDB1->DataSource = DS;

	String cellValue = NgDB->GetCellValue(Spalte,ARow);

	 //OK
	//TEST Vx sei Visible = false;
	//String cellValue = NgDB->GetCellValue(0,ARow);
	//OK

	String sql = "UPDATE [Nordwind].[dbo].[rrNordwind] SET Land = '" + cellValue + "' WHERE ID = 5";
	SendSQLLine(sql,QC1,1);

	NgDB->Repaint();
}
//---------------------------------------------------------------------------

//(4) Kein Konflikt mit Coloring
void __fastcall TFrame4XML::NgDBOnCellFormating(TObject *Sender, int ACol, int ARow,
		  WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)

{
	if (ARow >= 0) { // (5)Im Objekt-Explorer alles bold machen daan bleibt header bolt übrig
		FontStyle = TFontStyles();
	}

	if( (Value == L"500 V")||(Value == L"500  V") ) //(1)
	{
		//!!! Wichtig nicht Color nehmen sonst Frame gefärbt.
		TextColor = clRed;
		FontStyle << fsBold;
	}
	else
	{
		TextColor = clGreen;
		Font->Style >> fsBold;
	}

}
//---------------------------------------------------------------------------



//(2)
void __fastcall TFrame4XML::NgDBOnselectCell(TObject *Sender, int ACol, int ARow)
{

	if (ARow == NgDB->SelectedRow)  //Problem nur eine Zelle zun zeichnen
		NgDB->RefreshRow(NgDB->SelectedRow);
		//NgDB->Columns->Item[ACol]->Color = clYellow;  //(2)

}
//---------------------------------------------------------------------------

//(3)So eine ganze Zeile
void __fastcall TFrame4XML::NgDBOnCellColoring(TObject *Sender, int ACol, int ARow,
		  TColor &CellColor, TColor &GridColor, TCellState CellState)
{
		if(ARow == NgDB->SelectedRow) {
			CellColor = clSkyBlue;
			//NgDB->RefreshRow(NgDB->SelectedRow); (7)
		}
}

void __fastcall TFrame4XML::NgDBOnDrawHeader(TObject *Sender, int Index, TRect &HeaderRect)

{
	return;
	//ShowMessage("NgDBOnDrawHeader");
	TColumnHeader* header;
	for (int i = 0; i < NgDB->Columns->Count; ++i)
	{
		header = NgDB->Columns->Item[i]->Header;
		header->Color = clBlue;
		header->Font->Style << fsBold;
		header->Caption = "Nix11";
	}
}


//---------------------------------------------------------------------------
//Modell für praktische Spaltennamen
//---------------------------------------------------------------------------

//Test es gibt eine look-up-Tabelle
void __fastcall TFrame4XML::PracticalHeaderName()
{
	TField* Field;
	UnicodeString unistr;
	for (int i = 0; i < NgDB->Columns->Count ; ++i)
	{
		//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
		Field = NgDB->Columns->Item[i]->Field;
		//Feldname wie im DataSet !!!
		unistr = Field->FieldName;
		if(unistr == U"Vx")
			NgDB->Columns->Item[i]->Visible = false;
		TColumnHeader* header = NgDB->Columns->Item[i]->Header;
		AnsiString wstr = header->Caption;
		{
			header->Caption = "Nix " + IntToStr(i);
		}

		NgDB->Columns->Item[i]->Options << coEditing;
		NgDB->Columns->Item[i]->Options << coCanClick;
		NgDB->Columns->Item[i]->Options << coCanInput;
		//Damit geht es
	//NgDB->Columns->Item[i]->Editing; // ->Options << coEditing;
		NgDB->Columns->Item[i]->Enabled = true;

		//coCanClick, coCanInput,
	}
}

//Problem String in float-Darstellung
void __fastcall TFrame4XML::PracticalValues()
{
    TField* Field;
	UnicodeString unistr;

	for (int rows = 0; rows < NgDB->RowCount; ++rows)
	{

		for (int col = 0; col < NgDB->Columns->Count ; ++col)
		{
			//Hier wenn der Name des Feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
			Field = NgDB->Columns->Item[col]->Field;
			//Feldname wie im DataSet !!!
			unistr = Field->FieldName;
			if(unistr == U"Vx")
				NgDB->Columns->Item[col]->Visible = false;
			TColumnHeader* header = NgDB->Columns->Item[col]->Header;
			AnsiString wstr = header->Caption;
			{
				header->Caption = "Nix " + IntToStr(col);
			}
		}
	}
C2

//Wichtig für das Löschen
//NgDB->RowVisible



//---------------------------------------------------------------------------
//Nicht wichtig
//---------------------------------------------------------------------------











//Für rechte aber wir testen nur mit dem linken

void __fastcall TFrame4XML::NgDB1OnDrawHeader(TObject *Sender, int Index, TRect &HeaderRect)

{
	;
}

void __fastcall TFrame4XML::NgDB1CellBackGround(TObject *Sender, int ACol, int ARow,
		  TRect &CellRect, TCellState CellState, bool &DefaultDrawing)
{
	;
}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgDB1OnCellFormating(TObject *Sender, int ACol, int ARow,
          WideString Value, TColor &TextColor, TFontStyles &FontStyle, TCellState CellState)

{

	if ((Col > -1) && (Row > -1) && (ARow == Row) && (ACol == Col)) { // Überprüfe, ob es sich um die erste Zeile (Überschriftenzeile) handelt
		//FontStyle = TFontStyles();
		;
		//TextColor = clSkyBlue;
	}
	//if(ARow == -1)
	//	FontStyle << fsBold;
	//if(CellState.Contains(cpsTitle)
	//	FontStyle << fsBold;
}
//---------------------------------------------------------------------------




/*
Bemerkung (1)
	Ginge auch
	TField* Field;
	Field = NgDB->Columns->Item[ACol]->Field;
	WideString ansi = Field->AsWideString;
	if( (ansi == L"500 V")||(ansi == L"500  V") )

Bemerkung (2)
So ganze Spalte vom Select-Bereich
	void __fastcall TFrame4XML::NgDBOnselectCell(TObject *Sender, int ACol, int ARow)

{
//return;
	if (ARow == NgDB->SelectedRow)  //Problem nur eine Zelle zun zeichnen
		NgDB->Columns->Item[ACol]->Color = clYellow;
	//NgDB->Columns->
   //;
}

Bemerkung(5)
Im Objekt-Explorer alles bold machen daan bleibt header bolt übrig

Bemerkung(7)
Nicht hier es flick3ert dann !

*/


//hier Rechte-Vergabe nutzen RRR42
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgDBOnBeforeEdit(TObject *Sender, int ACol, int ARow,
		  bool &Accept)
{
	Accept = true;

//NgDB->Columns->Column[ACol]->Options << coEditing;
	//NgDB->Columns->Column[ACol]->AutoEdit = true;

}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgDBOnApplyEditText(TObject *Sender, int ACol, int ARow,
		  WideString &Value)
{
	;
}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgDBOnEdit(TObject *Sender, int ACol, int ARow, WideString Value)

{
	;
}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgDBOnEditAccept(TObject *Sender, int ACol, int ARow,
		  WideString Value, bool &Accept)
{
	if(Value == L"")
	{
		Accept = false;
		EditAccept = false;
	}
	else
	{
		Accept = true;  // :Accept = !true;  :So rein schreiben aber nicht akzeptiert nach Enter
		EditAccept = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgOnDblClick(TObject *Sender)
{
	NgDB->ReadOnly = false;
	if(Col > -1)
		NgDB->Columns->Item[Col]->Options << coEditing;
}
//---------------------------------------------------------------------------
//Später wenn Zeit
//---------------------------------------------------------------------------


/*
/ OnGetInplaceEdit event handler
void __fastcall TForm1::NextGrid61GetInplaceEdit(TObject *Sender, int ACol, int ARow, _di_INxInplaceEdit &InplaceEdit)
{
    if (ACol == 1)
    {
        // Set InplaceEdit var arg.
        InplaceEdit = new TListBoxEdit(this);
    }
}

// OnInplaceEditInsert event handler
void __fastcall TForm1::NextGrid61InplaceEditInsert(TObject *Sender, int ACol, int ARow, TComponent* Component)
{
	if (ACol == 1)
	{
		TListBoxEdit *listBoxEdit = dynamic_cast<TListBoxEdit*>(Component);
		if (listBoxEdit != nullptr)
		{
			listBoxEdit->Items->Add("George");
			listBoxEdit->Items->Add("Lucia");
			listBoxEdit->Items->Add("Michael");
			listBoxEdit->Items->Add("Ann");
			listBoxEdit->Items->Add("Ben");
		}
	}
}

// OnInplaceEditBounds event handler
void __fastcall TForm1::NextGrid61InplaceEditBounds(TObject *Sender, int ACol, int ARow, TComponent* Component, TRect &EditRect)
{
	if ((ACol == 1) && (NextGrid61->Columns->Items[ACol]->Width < 60))
	{
		EditRect.Right += 20;
	}
}

// OnAcceptEdit event handler
void __fastcall TForm1::NextGrid61AcceptEdit(TObject *Sender, int ACol, int ARow, WideString &Text, bool &Accept, bool &CanLeave)
{
	// Do not allow empty text to be transferred.
	if (Text.Trim() == L"")
	{
		Accept = false; // Value is not accepted
		Text = L"default"; // Inject default value into edit
		CanLeave = false; // Prevent leaving editing state
		MessageBeep(MB_ICONWARNING); // Alert user with sound
	}
}




*/
//Hier schliessen verhindern
void __fastcall TFrame4XML::NgDBOnEditExit(TObject *Sender)
{
	 if(!EditAccept)
	 {
		ShowMessage("NgDBOnEditExit");

		//NgDB->SelectedRow = 0;
		//Ng->C
		//NgDB->SelectedColumn = 0; //Columns->Item[0]->Selected = true;
		//NgDB->SetFocus();



		//TDataSetState TSS = DS->DataSet->State;
		//if(DS->DataSet->State == dsEdit) //coEditing)
			//Ng->InplaceEdit->EndEditing();
			//NgDB1->SetFocus(); //SetFocus
		//Dummy4Focus->Visible = !false;
		//Dummy4Focus->SetFocus();
			//Button3->SetFocus();
		{
			//DS->DataSet->Cancel();
			//ShowMessage("NgDBOnEditExit_Inner");

		}

	 }


}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgDBOnEndEditing(TObject *Sender, int ACol, int ARow,
		  WideString &Value, bool &Accept)
{
	;
}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgDBOnAfterEdit(TObject *Sender, int ACol, int ARow, WideString Value)

{
	if(!EditAccept)
		ShowMessage("NgDBOnAfterEdit");

}
//---------------------------------------------------------------------------

void __fastcall TFrame4XML::NgDBOnEditEnter(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------

//NEU
//Wo erster Aufbau und dann wieder-Aufbau ständig nie gelöscht zeigen ganz wichtig !!!
AnsiString ansi = "SELECT TOP (1000)[ID],[TEXT],[Name],[Email In Firma],[Email Privat],[Adresse],[Land],CASE WHEN MyBool IS NULL THEN 0 ELSE MyBool END AS MyBool, MyInt FROM [Nordwind].[dbo].[rrNordwind] WHERE MyBool != 1";
geht auch und besser
AnsiString ansi = "SELECT * FROM [Nordwind].[dbo].[rrNordwind] WHERE MyBool != 1";
SendSQLLine(ansi,QC1,1);


OK bis hier
2. Mai 2024