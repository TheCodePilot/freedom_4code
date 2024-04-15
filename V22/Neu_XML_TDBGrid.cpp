//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Neu_XML_TDBGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormXML *FormXML;
//---------------------------------------------------------------------------
//V12_Demo
#include <algorithm>
#include <SqlExpr.hpp>
#include <DBClient.hpp>
#include <vector>

	//#include <atomic> //leider von bcc32 nicht unterstützt :(
__fastcall TFormXML::TFormXML(TComponent* Owner)
	: TForm(Owner), xmlProcess(new RRClasses::XMLData()),
	  OpenTextFileDialog(new TOpenTextFileDialog(this))
{
	testcounter = -1;
	ersteSpalte = false;
	cbnLöscheSpalten = false;
	AnsiString clickEreignisCard_Item = L"";
	clickValueCard_Item = L"";
	clickMeasureCard_Item = L"";

	xmlData = dynamic_cast<RRClasses::XMLData*>(xmlProcess.get());
	if(!xmlData){
		ShowMessage("TFormXML RRClasses::XMLData* xmlData = dynamic_cast<RRClasses::XMLData*>(xmlProcess): Problem");
		return;
	}
	//ComboBox
    ComboBoxDBGridUserChoice->Parent = this;
	ComboBoxDBGridUserChoice->Style = csDropDownList;
	ComboBoxDBGridUserChoice->Items->Add("C1");
	ComboBoxDBGridUserChoice->Items->Add("MA");
	ComboBoxDBGridUserChoice->Items->Add("TAP");
	ComboBoxDBGridUserChoice->ItemIndex = 0;
	//ComboBox



	fromDBGrid4XMLCard_Item = false;
	OpenTextFileDialog->InitialDir = GetCurrentDir();
	OpenTextFileDialog->Filter = "XML Files (*.xml)|*.xml|All Files (*.*)|*.*";
	UnicodeString ustr = U"";
	if (OpenTextFileDialog->Execute())
	{
		ustr = OpenTextFileDialog->FileName;
	}

	Memo1->ReadOnly = true;
	Memo1->Clear();

	if(!ustr.IsEmpty())
	{
		this->BorderStyle = bsDialog;

		std::wstring wstr(ustr.w_str());
		DataSource1->DataSet = ClientDataSet1;
		DBGrid4XMLin->DataSource = DataSource1;
		DBGrid4XMLin->ReadOnly = true;
		DBGrid4XMLin->DefaultDrawing = false;
		ClientDataSet1->FieldDefs->Clear();
		ClientDataSet1->FieldDefs->Add("Name", ftString, 50);
		ClientDataSet1->FieldDefs->Add("Datum/Uhrzeit", ftString, 255);
		ClientDataSet1->CreateDataSet();
		//Das zweite
		DataSource2->DataSet = ClientDataSet2;
		DBGrid4XMLCard_Item->DataSource = DataSource2;
		DBGrid4XMLCard_Item->ReadOnly = true; //RR142
		ClientDataSet2->FieldDefs->Clear();
		ClientDataSet2->FieldDefs->Add("Name der Messgroesse", ftString, 50);
		ClientDataSet2->FieldDefs->Add("Wert mit Einheit", ftString, 255);
		ClientDataSet2->CreateDataSet();
		//Das dritte
		DataSource3->DataSet = ClientDataSet3;
		DBGridUserChoice->DataSource = DataSource3;
		DBGridUserChoice->ReadOnly = true;
		ClientDataSet3->FieldDefs->Clear();
		xmlProcess->LoadXMLDocument(wstr);
		try
		{
			XML2Grid(xmlProcess.get());
		}
			catch (const Exception &e)
		{
			ShowMessage("Beim einlesen der XML-Daten. An Position" + e.Message);

		}
	}
	else
	{
		ShowMessage("Sie haben die Aktion abgebrochen. Ohne die Wahl einer XML-Datei werden keine Daten geladen");
		//RR42: Noch schöner Abbau oder erneut Dialog hinein
		//Am besten auch die Form abbauen oder Button File Dialog mal sehen
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormXML::DBGrid4XMLinOnCellClick(TColumn *Column)
{
	Memo1->Clear();
	IniCard_Item();
	DBGridUserChoice->Refresh();

	if(xmlProcess)
	{
		if(xmlData)
		{
			xmlData->DummyIfThis();  //RRR42

			AnsiString clickEreignis = DBGrid4XMLin->Columns->Items[0]->Field->AsString;
			AnsiString clickDatum = DBGrid4XMLin->Columns->Items[1]->Field->AsString;

			std::wstring ereignis = RRTools::AnsiToWide(clickEreignis);
			std::wstring datum = RRTools::AnsiToWide(clickDatum);
			int i = DBGrid4XMLin->DataSource->DataSet->RecNo;// GetRecNo(); // Eins - basiert mit 1 beginnend

			if(xmlData->AnalyzeDOM_ProofTyp(i-1) == 0)//	Index der Speicherung bei XMLData 0 - basiert
			{
				xmlData->AnalyzeDOM_PicEventUwish(ereignis, datum, 0); //bei 1: Nur Jede Art von Messwert einmalig

				DBGrid4XMLCard_Item->DataSource->DataSet->Next();
				std::vector<std::pair<std::wstring, std::wstring> > data = xmlData->GetXMLInfo();

				for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it= data.begin(); it != data.end(); ++it)
				{
					const std::pair < std::wstring, std::wstring>& pair = *it;
					ClientDataSet2->Append();
					ClientDataSet2->FieldByName("Name der Messgroesse")->AsString = pair.first.c_str();
					ClientDataSet2->FieldByName("Wert mit Einheit")->AsString = pair.second.c_str();
					ClientDataSet2->Post();
				}

				DBGrid4XMLCard_Item->DataSource->DataSet->RecNo = 1;
			}
			else
			{
				std::wstring wcomment = xmlData->GetCommentIfType18();
				AnsiString ansi = RRTools::WStrToAnsiString(wcomment);

				ClientDataSet2->EmptyDataSet();
				Memo1->Clear();
				Memo1->Lines->Add(ansi);

			}

		}
	
 	}
}
//---------------------------------------------------------------------------
//=============================================================================================================================================
void __fastcall TFormXML::FillTDBGridFromXML(RRClasses::XMLBasic* xmlProcess)
//=============================================================================================================================================
{
	if(xmlProcess)
	{
		DBGrid4XMLin->DataSource->DataSet->Next();
		if(xmlData)
		{
			std::vector<std::pair<std::wstring, std::wstring> > data = xmlData->GetXMLInfo();
			for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it = data.begin(); it != data.end(); ++it)
			{
				const std::pair < std::wstring, std::wstring>& pair = *it;
				ClientDataSet1->Append();
				ClientDataSet1->FieldByName("Name")->AsString = pair.first.c_str();
				ClientDataSet1->FieldByName("Datum/Uhrzeit")->AsString = pair.second.c_str();
				ClientDataSet1->Post();
			}
	   }
	   
	}
}

//=============================================================================================================================================
void __fastcall TFormXML::XML2Grid(RRClasses::XMLBasic* xmlProcess)
//=============================================================================================================================================
{
	if(xmlData)
	{
		xmlData->CollectCards_Items();
		xmlData->AnalyzeDOM_PicAllEvents();
		FillTDBGridFromXML(xmlData);
	}
}

void __fastcall TFormXML::DBGrid4XMLinDrawColumnCell(TObject *Sender, const TRect &Rect,
		  int DataCol, TColumn *Column, TGridDrawState State)
{
	if(Column->Index ==1)
	{
		DBGrid4XMLin->Canvas->Brush->Color = TColor(RGB(225,225,225)); //(192,192,192));// clGray;
	}
	else
	{
		DBGrid4XMLin->Canvas->Brush->Color = clWindow;
	}

	DBGrid4XMLin->DefaultDrawColumnCell(Rect, DataCol, Column, State);

}
//---------------------------------------------------------------------------
//=============================================================================================================================================
//Hier wird das Drag und Drop Manuell erreicht. Entscheidend die bool-Variable: fromDBGrid4XMLCard_Item
//=============================================================================================================================================

void __fastcall TFormXML::DBGrid4XMLCard_ItemOnCellKlick(TColumn *Column)
{
	currentRowCard_Item = DBGrid4XMLCard_Item->DataSource->DataSet->RecNo;
	clickMeasureCard_Item = DBGrid4XMLCard_Item->Columns->Items[0]->Field->AsString;
	clickValueCard_Item = DBGrid4XMLCard_Item->Columns->Items[1]->Field->AsString;

	if (AllreadyChosed())
	{
		//User hat das schon angeklickt
		fromDBGrid4XMLCard_Item = false;
		return;
	}
	else
		fromDBGrid4XMLCard_Item = true;  //RRR142
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::DBGridUserChoiceMouseEnter(TObject *Sender)
{
	if(fromDBGrid4XMLCard_Item == true)
	{
		//-----------------RRR42
		std::pair<int,std::wstring> data(currentRowCard_Item,RRTools::AnsiToWide(clickMeasureCard_Item)); //data.second: 0 dann alles OK
		selectedRowsDBGrid4XMLCard_Item.push_back(data);
		DBGrid4XMLCard_Item->Invalidate();
		testcounter = 0;
		//-----------------RRR42

		ersteSpalte = false;
		cbnLöscheSpalten = false;
		fromDBGrid4XMLCard_Item = false;
		NewColumnName();
		UpdateDBGridUserChoice();
	}
}

//=============================================================================================================================================
//Hilfsfunktionen
//=============================================================================================================================================
void __fastcall TFormXML::NewColumnName()
{
//ShowMessage("NewColumnName() : Start");
	if(cbnLöscheSpalten)
	{
		cbnLöscheSpalten = false;
		ShowMessage("NewColumnName() : Returner");
		return;
	}

	bool columnNameIsDeleted = false;
	bool neuerEintrag = true;
	bool comeback = false;

	int dummy = 42;
	int index = -1;
	int indexComeBack = -1;

	std::wstring wstrComeBack =L"";


	//deleted noch abfangen
	std::vector<std::pair<std::wstring, int> > dataMaxFromColumOne;
	for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
	{
		++index;
		const std::pair < std::wstring, int>& pair4UserDeleted = *it1;  //bis hier wohl OK

		//Test
	if(pair4UserDeleted.second == 10)
	{
		comeback = true;
		wstrComeBack = pair4UserDeleted.first;
		indexComeBack = index;
	}
		if((pair4UserDeleted.second == 0)||(pair4UserDeleted.second == 1))
		{
			std::pair<std::wstring, int> info(pair4UserDeleted.first, dummy);
			dataMaxFromColumOne.push_back(info);
		}

		if(neuerEintrag && (RRTools::AnsiToWide(clickMeasureCard_Item) == pair4UserDeleted.first))
		{
			neuerEintrag = false; //break;
		}


	}

	if(comeback)
	{
		std::pair<std::wstring, int> comeBack(wstrComeBack, 0);    //RRR42
		fieldDefsColumnsUserDBGridAsStr[indexComeBack] = comeBack;

		ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(wstrComeBack), ftString, 12);

		//fieldDefsColumnsUserDBGridAsStr.push_back(newColoumn);
	}



	if( !comeback && ( (neuerEintrag)||(ClientDataSet3->FieldCount == 0) ) )
	{
		ClientDataSet3->FieldDefs->Add(clickMeasureCard_Item, ftString, 12);
//ShowMessage("NewColumnName() : __if( (neuerEintrag)||(ClientDataSet3->FieldCount == 0) )__");
		//Was nun erstellt sich merken
		std::pair<std::wstring, int> newColoumn(RRTools::AnsiToWide(clickMeasureCard_Item), 0);    //RRR42
		fieldDefsColumnsUserDBGridAsStr.push_back(newColoumn);
	}

	if(ClientDataSet3->Active)
	{
		ClientDataSet3->Close();
	}

	if(!ClientDataSet3->Active)
	{
		//ShowMessage("NewColumnName()__ClientDataSet3->CreateDataSet() : direkt davor");
		ClientDataSet3->CreateDataSet();
		ClientDataSet3->Open();
	}

//ClientDataSet3->FieldDefs->Update();
	DBGridUserChoice->Refresh();

	//ShowMessage("NewColumnName() : Ende");
}



void __fastcall TFormXML::NewColumnName_Bisher()
{
	//ShowMessage("NewColumnName() : Start");
	if(cbnLöscheSpalten)
	{
		cbnLöscheSpalten = false;
		ShowMessage("NewColumnName() : Returner");
		return;
	}

	bool columnNameIsDeleted = false;
	bool neuerEintrag = false;

	int dummy = 42;

	//deleted noch abfangen

	for(std::vector<std::pair < std::wstring, int> >::iterator it = fieldDefsColumnsUserDBGridAsStr.begin(); it != fieldDefsColumnsUserDBGridAsStr.end(); ++it)
	{
		const std::pair < std::wstring, int>& pairIfDelete = *it;    //Hier den richtigen Vector nehmen
		const int status = pairIfDelete.second;
		const std::wstring wstrIfDelete = pairIfDelete.first;

		//Schauen ob Neu
		if(!neuerEintrag && (RRTools::AnsiToWide(clickMeasureCard_Item) == pairIfDelete.first))
		{
			neuerEintrag = false; break;
		}
		neuerEintrag = true;
	}


	if( (neuerEintrag)||(ClientDataSet3->FieldCount == 0) )
	{
		ClientDataSet3->FieldDefs->Add(clickMeasureCard_Item, ftString, 12);
	//ShowMessage("NewColumnName() : __if( (neuerEintrag)||(ClientDataSet3->FieldCount == 0) )__");
		//Was nun erstellt sich merken
		std::pair<std::wstring, int> newColoumn(RRTools::AnsiToWide(clickMeasureCard_Item), 0);    //RRR42
		fieldDefsColumnsUserDBGridAsStr.push_back(newColoumn);
	}

	if(ClientDataSet3->Active)
	{
		ClientDataSet3->Close();
	}

	if(!ClientDataSet3->Active)
	{
		//ShowMessage("NewColumnName()__ClientDataSet3->CreateDataSet() : direkt davor");
		ClientDataSet3->CreateDataSet();
		ClientDataSet3->Open();
	}

//ClientDataSet3->FieldDefs->Update();
	DBGridUserChoice->Refresh();

	//ShowMessage("NewColumnName() : Ende");
}


//Frage die sich stellt wo brauche ich dann
void __fastcall TFormXML::UpdateDBGridUserChoice()
{

	bool codePos1 = false;
	bool codePos2 = false;


	//ShowMessage("Anzahl ClientDataSet3->FieldDefs->Count : " + IntToStr(ClientDataSet3->FieldDefs->Count));
	if(xmlProcess)
	{
		//AnsiString clickleft = DBGrid4XMLCard_Item->Columns->Items[0]->Field->AsString;

		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;
		if(xmlData)
		{
			int anzahlSchleifenDurchgang = -1;

			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			//Schleife der Spalten- Überschrift ; Achtung deletes noch abfangen
			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
			{
				++anzahlSchleifenDurchgang;
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;  //bis hier wohl OK

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					if(pair4UserChoice.second == 10)
						ShowMessage("pair4UserChoice.second == 10");
				//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was ngeklickt ist
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first; //OK

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							std::pair<std::wstring, std::wstring> info(pairOfAllXML.second, L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}


				int m = (int)ersteSpalte;
				//ShowMessage("Erste spalte ist Oben: " +IntToStr(m));

				if(!ersteSpalte)
				{

					//ShowMessage("IN SCHLEIFE : ClientDataSet3->FieldDefs->Count NewColumnName() : " + IntToStr(ClientDataSet3->FieldDefs->Count));

					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
					{
						const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

						ClientDataSet3->Append();
		//ShowMessage("__if(!ersteSpalte)__  __ClientDataSet3->FieldByName__ : 6 : " + RRTools::WStrToAnsiString(pairMaxFromColumOne.first));
						ClientDataSet3->FieldByName(pair4UserChoice.first.c_str())->AsString = RRTools::WStrToAnsiString(pairMaxFromColumOne.first);
						codePos1 = true;
		//ShowMessage("NewColumnName() : 7 : ");

						ClientDataSet3->Post();
						ClientDataSet3->Next();
					}
					dataMaxFromColumOne.clear();
					ersteSpalte = true;
					int k = (int)ersteSpalte;
					//ShowMessage("Erste spalte ist nach true setzen: " +IntToStr(k));
					++testcounter;

				}

				else
				{
					//ShowMessage("else UpdateDBGridUserChoice : 8 : ");
					ClientDataSet3->RecNo = 1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
					{
						const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

						ClientDataSet3->Edit();
		//ShowMessage("NewColumnName() : 9 : ");
		//ShowMessage("__else__  __ClientDataSet3->FieldByName__ : 9 : " + RRTools::WStrToAnsiString(pairMaxFromColumOne.first));
						ClientDataSet3->FieldByName(pair4UserChoice.first.c_str())->AsString = RRTools::WStrToAnsiString(pairMaxFromColumOne.first);
						codePos2 = true;
		//ShowMessage("NewColumnName() : 10 : ");
						ClientDataSet3->Post();
						ClientDataSet3->Next();
					}
					dataMaxFromColumOne.clear();
				}
			}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))

			/*if(codePos1){
				ShowMessage("__if(!ersteSpalte)__  __ClientDataSet3->FieldByName__ "); codePos1 = false;}
			if(codePos2){
				ShowMessage("__else__  __ClientDataSet3->FieldByName__ "); codePos2 = false;}
			*/

			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...

		}
	}

}

void __fastcall TFormXML::UpdateDBGridUserChoice_OK()
{
	;
}



void __fastcall TFormXML::UpdateDBGridUserChoice_Bisher()
{
	;
}

//=============================================================================================================================================
//To Do in Process
//=============================================================================================================================================

void __fastcall TFormXML::DBGrid4XMLCard_ItemMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	Screen->Cursor = crDrag;


	/*
	TPoint mousePos = ScreenToClient(Mouse->CursorPos);

	// Zeile und Spalte anhand der Mauskoordinaten erhalten
	int row, col;
	DBGrid4XMLCard_Item->MouseToCell(mousePos.x, mousePos.y, col, row);

	 // Grafische Kennzeichnung der Zelle (Hintergrund blau)
		int cellWidth = DBGrid4XMLCard_Item->ColWidths[col];
		int cellHeight = DBGrid4XMLCard_Item->RowHeights[row];
		TRect cellRect(col, row, col + cellWidth, row + cellHeight);
		//DBGrid1->Canvas->Brush->Color = clBlue;
		//DBGrid1->Canvas->FillRect(cellRect);

	DBGrid4XMLCard_Item->Canvas->Brush->Color = clBlue;
	DBGrid4XMLCard_Item->Canvas->FillRect(cellRect);
	*/
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::DBGrid4XMLCard_ItemMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	Screen->Cursor = crDefault;
}
//--------------------------------------------------------------------------


//fieldDefsColumnsUserDBGridAsStr
void __fastcall TFormXML::DBGrid4XMLCard_ItemOndrawColumnCell(TObject *Sender, const TRect &Rect,
		  int DataCol, TColumn *Column, TGridDrawState State)
{
	int currentRow = DBGrid4XMLCard_Item->DataSource->DataSet->RecNo;   //RecNo fängt bei der ersten Zeile ohne Überschrift an und dann 1 basiert. Überschrift wäre theoretisch 0
	AnsiString clickEreignis = DBGrid4XMLCard_Item->Columns->Items[0]->Field->AsString;
	AnsiString clickDatum = DBGrid4XMLCard_Item->Columns->Items[1]->Field->AsString;
	//Ermitteln ob die aktuelle Zeile und die gespeicherte Zeile in selectedRowsDBGrid4XMLCard_Item gleich sind und als gelöscht nach Löschbutton gedrückt gekennzeichnet
	RRClasses::RRFTorPairs<int,std::wstring> ftor(currentRow, RRTools::AnsiToWide(clickEreignis));
	std::vector<std::pair<int,std::wstring> >::iterator it =
		std::find_if(selectedRowsDBGrid4XMLCard_Item.begin(), selectedRowsDBGrid4XMLCard_Item.end(),ftor);

	//Fall wie oben wenn fieldDefsColumnsUserDBGridAsStr.second == 2; Hier aber Spalten-Namen bezogen
	RRClasses::RRFTorPairs<std::wstring, int> ftor1(RRTools::AnsiToWide(clickEreignis), 2);
	std::vector<std::pair<std::wstring, int> >::iterator it1 =
		std::find_if(fieldDefsColumnsUserDBGridAsStr.begin(), fieldDefsColumnsUserDBGridAsStr.end(),ftor1);

														//Nur clSkyBlue wenn nichts unsichtbar also das bereits blaue nicht wieder rückgängig gemacht wird.
	if ( (it != selectedRowsDBGrid4XMLCard_Item.end()) && (it1 == fieldDefsColumnsUserDBGridAsStr.end()) )
	{
		DBGrid4XMLCard_Item->Canvas->Brush->Color = clSkyBlue; //clBlue;
	}
	else
	{
		DBGrid4XMLCard_Item->Canvas->Brush->Color = clWindow;
	}

	DBGrid4XMLCard_Item->DefaultDrawColumnCell(Rect, DataCol, Column, State);
}

//---------------------------------------------------------------------------

//Hier noch Abfrage wollen Sie das wirklich RRR42
void __fastcall TFormXML::cbnLoescheSpalteOnClick(TObject *Sender)
{
	fieldCounterToDelete = 0;

	for(int i = 0; i < selectedColumnsUserDBGrid4Draw.size(); ++i)
	{
		std::pair<int,int> nunUnsichtbar = selectedColumnsUserDBGrid4Draw[i];
		if( selectedColumnsUserDBGrid4Draw[i].second == 1)
		{
			std::pair<int,int> nunUnsichtbar(selectedColumnsUserDBGrid4Draw[i].first, 1);  //von 2 weg: gehen
			selectedColumnsUserDBGrid4Draw[i] = nunUnsichtbar;
			//int  j = nunUnsichtbar.first;
			DBGridUserChoice->Columns->Items[nunUnsichtbar.first]->Visible = false;

		}
	}


	//Test
	//wegen rot-Verschiebung nach Visible = false
	//Alle nun wieder nicht rot angeglickt darstellen also second = 0 setzen;
	for(int i = 0; i < selectedColumnsUserDBGrid4Draw.size(); ++i)
	{
		selectedColumnsUserDBGrid4Draw[i].second = 0;
	}
    //Test

	//Spaltenbezeichner
	//angeklickte Spalte nun notieren
	//Hier muß auch Grafisch wieder Grid2 frei gegeben werden
	for(int i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{
		std::pair<std::wstring,int> nunUnsichtbar = fieldDefsColumnsUserDBGridAsStr[i];
		if(fieldDefsColumnsUserDBGridAsStr[i].second == 1)
		{
			++fieldCounterToDelete;
			std::pair<std::wstring,int> nunUnsichtbar(fieldDefsColumnsUserDBGridAsStr[i].first, 2);
			std::wstring wstr= nunUnsichtbar.first;
			int j = nunUnsichtbar.second;
			fieldDefsColumnsUserDBGridAsStr[i] = nunUnsichtbar;
			G3ToGrid2Synchron(fieldDefsColumnsUserDBGridAsStr[i].first);
				//ShowMessage("Angeklickte Spalte nun : " + RRTools::WStrToAnsiString(wstr) + " und der Status ist : " + IntToStr(nunUnsichtbar.second));

			//DBGridUserChoice->Columns->FindItemID(i)->Release();

		}
	}

	ersteSpalte = false;
	//ShowMessage("cbnLoescheSpalteOnClick : 1");

	//SonderFall Button gedrückt obwohl nichts rot ist könnte auch in wenn erstemal was rot angeklickt dieswer Button noch gesperrt werden
	if(fieldCounterToDelete == 0)
		return;

	//SonderFall alles nun gelöscht
		//if(SomeFieldsExist() == 0)
		//return;

	if(SomeFieldsExist() >= 1)
		BuildRestOfFieldDefs();
	else
		IniCard_Item();
	//ShowMessage("cbnLoescheSpalteOnClick : 2");

	cbnLöscheSpalten = true;
	//ShowMessage("cbnLoescheSpalteOnClick : 3");
//ShowMessage("ClientDataSet3->FieldCount : " + IntToStr(ClientDataSet3->FieldCount));
	if(ClientDataSet3->FieldCount > 0)
		UpdateDBGridUserChoice();
	DBGrid4XMLCard_Item->Invalidate();
	//Hier muß auch Grafisch wieder Grid2 frei gegeben werden

}
//---------------------------------------------------------------------------

void __fastcall TFormXML::DBGridUserChoiceOnDraw(TObject *Sender, const TRect &Rect,
		  int DataCol, TColumn *Column, TGridDrawState State)
{
	RRClasses::RRFTorPairs<int,int> ftor(Column->Index, 1);
	std::vector<std::pair<int, int> >::iterator it2 =
		std::find_if(selectedColumnsUserDBGrid4Draw.begin(), selectedColumnsUserDBGrid4Draw.end(),ftor);

		//TEST
		if(it2 != selectedColumnsUserDBGrid4Draw.end()){
		std::pair<int,int> nunUnsichtbar = *it2;
		int links = nunUnsichtbar.first;
		int rechts = nunUnsichtbar.second;
		int c = Column->Index;
		int dummy = 0;
		//return;
		//ShowMessage("OnDraw: ___ first is : " + IntToStr(links) + " second is : " + IntToStr(rechts) + " Column->Index is : " + IntToStr(Column->Index));
        }//TEST


	if(it2 != selectedColumnsUserDBGrid4Draw.end())
	{
		Column->Color = clRed;
	}
	else
	{
		DBGridUserChoice->Canvas->Brush->Color = clWindow;
	}

	DBGridUserChoice->DefaultDrawColumnCell(Rect, DataCol, Column, State);


}
//---------------------------------------------------------------------------

void __fastcall TFormXML::cbnLoescheAllesOnClick(TObject *Sender)
{

	IniCard_Item();
    DBGrid4XMLCard_Item->Invalidate();
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::cbOnClick(TObject *Sender)
{
	//ShowMessage("cbOnClick");
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::cbnUebertrageOnClick(TObject *Sender)
{
	AnsiString ausgewaehlterText = ComboBoxDBGridUserChoice->Text;

	if(ausgewaehlterText == "C1")
	{
		;
	}
	else if(ausgewaehlterText == "MA")
	{
		;
	}
	else if(ausgewaehlterText == "TAP")
	{
		;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::Uebertrage()
{
     // Erstellen  eine SQL-Verbindung
    TSQLConnection *SQLConnection = new TSQLConnection(this);
    SQLConnection->DriverName = "MSSQL";
    SQLConnection->GetDriverFunc = "getSQLDriverMSSQL";
    SQLConnection->LibraryName = "dbxmss.dll";
    SQLConnection->VendorLib = "sqlncli10.dll";
    SQLConnection->Params->Values["HostName"] = "localhost";
    SQLConnection->Params->Values["Database"] = "mydatabase";
    SQLConnection->Params->Values["User_Name"] = "username";
    SQLConnection->Params->Values["Password"] = "password";
    SQLConnection->LoginPrompt = false;
    SQLConnection->Connected = true;

    // Erstellen  eine SQL-Abfrage
    TSQLQuery *SQLQuery = new TSQLQuery(this);
    SQLQuery->SQLConnection = SQLConnection;
	SQLQuery->SQL->Add("SELECT * FROM mytable");
	SQLQuery->Open();

	//std::unique_ptr<TDataSetProvider> Provider(new TDataSetProvider(this));
	DataSetProvider1->DataSet = SQLQuery;

	// Laden  die Daten in das ClientDataSet
	ClientDataSet1->Close();
	ClientDataSet1->ProviderName = DataSetProvider1->Name;
		//ClientDataSet1->DataSource->DataSet-> LoadFromDataSet(SQLQuery, 0, lmAppend);
	ClientDataSet1->Open();

	// Bereinigen
	delete SQLQuery;
	delete SQLConnection;
}

//Also schaue wo second == dem vom User gelöschten Spalte ist
//selectedRowsDBGrid4XMLCard_Itemsecond == aktuell vom User gewählte Spalte
void __fastcall TFormXML::G3ToGrid2Synchron(std::wstring wstr)
{
	int dummy = 42;
	int index = -1;

	for(std::vector<std::pair<int, std::wstring> >::iterator it =
			selectedRowsDBGrid4XMLCard_Item.begin(); it != selectedRowsDBGrid4XMLCard_Item.end(); ++it)
	{
	//RRClasses::RRFTor1Condition<int, std::wstring> ftor1(dummy, wstr);
	//std::vector<std::pair<int, std::wstring> >::iterator itAllesBisherVomUserDropped =
	//	std::find_if(selectedRowsDBGrid4XMLCard_Item.begin(), selectedRowsDBGrid4XMLCard_Item.end(),ftor1);
		++index;
		std::pair<int, std::wstring> compare = *it;

		if(compare.second == wstr) //selectedRowsDBGrid4XMLCard_Item.end())
		{
			std::vector<std::pair<int, std::wstring> >::iterator it4erase = selectedRowsDBGrid4XMLCard_Item.begin() + index;
			 //iterator= myvector.begin()+2
		//myvector.erase(iterator);
//Output:    1, 2, 4, 5
			//selectedRowsDBGrid4XMLCard_Item.erase(selectedRowsDBGrid4XMLCard_Item.begin() + index));
			selectedRowsDBGrid4XMLCard_Item.erase(it4erase);     //erase(index);
			break;
		}
	}
}

void __fastcall TFormXML::BuildRestOfFieldDefs()
{

	if(!ClientDataSet3->Active)
	{
		// Das Dataset ist nicht aktiv
		ClientDataSet3->FieldDefs->Clear();

	}
	else
	{
		// Das Dataset ist aktiv, also schließen Sie es zuerst
		ClientDataSet3->Close();
		ClientDataSet3->FieldDefs->Clear();
	}


	// Definieren Sie FieldDefs
	for(int i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{
		std::pair<std::wstring,int> nunUnsichtbar = fieldDefsColumnsUserDBGridAsStr[i];
		if(fieldDefsColumnsUserDBGridAsStr[i].second == 0) //1 bedeutet ja vorbereitet also wird ja bei Button immer zu 2 oder 1 zu 0 durch zurück nehmen
		{
			std::pair<std::wstring,int> nunUnsichtbar(fieldDefsColumnsUserDBGridAsStr[i].first, fieldDefsColumnsUserDBGridAsStr[i].second);
			std::wstring wstr= nunUnsichtbar.first;
			int j = nunUnsichtbar.second;


			ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(fieldDefsColumnsUserDBGridAsStr[i].first), ftString, 12);
		//ShowMessage("BuildRestOfFieldDefs : __FieldDefs->Add__");
		}
	}

	if(fieldCounterToDelete > 0) //1-basiert
		ClientDataSet3->CreateDataSet();
	/*
	else
	{
//ShowMessage("__Else__ Menge an SpaltenDefinitionen : " + IntToStr(ClientDataSet3->FieldCount));
	//--------------------
		Memo1->Clear();
	//ClientDataSet3->Close();
	//ClientDataSet3->FieldDefs->Clear();

	currentRowCard_Item = -1;      //prüfen
	clickMeasureCard_Item = L"";        //prüfen
	clickValueCard_Item = L"";      //prüfen
								//StatusReset();
	DBGridUserChoice->Refresh();
		//--------------------

	}
	*/
	ersteSpalte = false;
//ShowMessage("Menge an SpaltenDefinitionen : " + IntToStr(ClientDataSet3->FieldCount));

}

bool __fastcall TFormXML::AllreadyChosed()
{
	int dummy = 42;
	int index = -1;
	RRClasses::RRFTor1Condition<int, std::wstring> ftor1(dummy, RRTools::AnsiToWide(clickMeasureCard_Item));
	std::vector<std::pair<int, std::wstring> >::iterator itAllesBisherVomUserDropped =
		std::find_if(selectedRowsDBGrid4XMLCard_Item.begin(), selectedRowsDBGrid4XMLCard_Item.end(),ftor1);

	if(itAllesBisherVomUserDropped == selectedRowsDBGrid4XMLCard_Item.end())
	{
		/*
		Lösung wenn nicht doppelt
		Dann schauen wenn aber pair4UserDeleted.second = 2
		wieder auf 0 setzen
		*/
		//-----------------------

		//Fall: War in der Vergangenheit schon einmal gewählt aber gelöscht worden
		//also kein komplettes delete aller Spalten und kein Ereignis Card_item gewechselt in GridXMLin
		for(std::vector<std::pair<std::wstring, int> >::iterator it =
			fieldDefsColumnsUserDBGridAsStr.begin(); it != fieldDefsColumnsUserDBGridAsStr.end(); ++it)
		{
			++index;
			std::pair<std::wstring,int> nunUnsichtbar = *it;
			std::wstring links = nunUnsichtbar.first;
			int rechts = nunUnsichtbar.second;

			if(links == RRTools::AnsiToWide(clickMeasureCard_Item)&&(rechts == 2))
			{
				std::pair<std::wstring,int> nunUnsichtbar2(links, 10); //Besonderheit das feld zu generiren
				fieldDefsColumnsUserDBGridAsStr[index] = nunUnsichtbar2;
					//fieldDefsColumnsUserDBGridAsStr[Column->Index] = nunUnsichtbar2; //weiterhin nicht denke ich !!!
			}

		}


		return  false;
	}
	return true;
 }

void __fastcall TFormXML::IniCard_Item()
{
	ClientDataSet3->Close();
	ClientDataSet3->FieldDefs->Clear();
	selectedRowsDBGrid4XMLCard_Item.clear();
	selectedColumnsUserDBGrid4Draw.clear();
	fieldDefsColumnsUserDBGridAsStr.clear();
	currentRowCard_Item = -1;
	clickMeasureCard_Item = L"";
	clickValueCard_Item = L"";
	fieldCounterToDelete = 0;
}

int __fastcall TFormXML::SomeFieldsExist()
{
	int returner = 0;
	for(int i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{

		if(fieldDefsColumnsUserDBGridAsStr[i].second == 0)
		{
			++returner;
			break;
		}
	}
	return returner;
}


void __fastcall TFormXML::StatusReset()
{
	for(int i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{
		fieldDefsColumnsUserDBGridAsStr[i].second = 0;
	}

    //Dies steuert dann Zeichnen ob rot oder weiß in Draw Grid3
	for(int i = 0; i < selectedColumnsUserDBGrid4Draw.size(); ++i)
	{
		selectedColumnsUserDBGrid4Draw[i].second = 0;
	}

}

void __fastcall TFormXML::DBGridUserChoiceOnCellClick(TColumn *Column)
{
	//ShowMessage("DBGridUserChoiceOnCellClick : Start");
	int dummy = 42;
	AnsiString columnName = Column->FieldName;
	bool neuerEintrag = true;
	int index = -1;



	for(std::vector<std::pair<int, int> >::iterator it =
		selectedColumnsUserDBGrid4Draw.begin(); it != selectedColumnsUserDBGrid4Draw.end(); ++it)
	{
		++index;
		std::pair<int,int> nunUnsichtbar = *it;
		int links = nunUnsichtbar.first;
		int rechts = nunUnsichtbar.second;

		if(links == Column->Index)
		{
			if(rechts == 0)
			{
				std::pair<int,int> nunUnsichtbar2(links, 1);
				selectedColumnsUserDBGrid4Draw[index] = nunUnsichtbar2;
			}

			if(rechts == 1)
			{
				std::pair<int,int> nunUnsichtbar2(links, 0);
				selectedColumnsUserDBGrid4Draw[index] = nunUnsichtbar2;
			}
			neuerEintrag = false;
				//ShowMessage("Index is : " + IntToStr(index) + " Column->Index is : " + IntToStr(Column->Index));
			//ShowMessage("OnCellClick: ___ first is : " + IntToStr(links) + " second is : " + IntToStr(rechts) + " Column->Index is : " + IntToStr(Column->Index)+ " ABER DER INDEX IST index is : " + IntToStr(index));

			break;
		}
	}

	if(neuerEintrag)
	{
		//ShowMessage("Neuer Eintrag");
		std::pair<int,int> angeklickt(Column->Index,1);
		//ShowMessage("Neuer Eintrag in OnCellClick: ___ first is : " + IntToStr(angeklickt.first) + " second is : " + IntToStr(angeklickt.second) + " Column->Index is : " + IntToStr(Column->Index));

		selectedColumnsUserDBGrid4Draw.push_back(angeklickt);
	}

	  //Ist drinn
	index = -1;
	for(std::vector<std::pair<std::wstring, int> >::iterator it =
		fieldDefsColumnsUserDBGridAsStr.begin(); it != fieldDefsColumnsUserDBGridAsStr.end(); ++it)
	{
		++index;
		std::pair<std::wstring,int> nunUnsichtbar = *it;
		std::wstring links = nunUnsichtbar.first;
		int rechts = nunUnsichtbar.second;

		if(RRTools::WStrToAnsiString(links) == Column->FieldName)
		{
			if(rechts == 0)
			{
				std::pair<std::wstring,int> nunUnsichtbar2(links, 1);
				fieldDefsColumnsUserDBGridAsStr[index] = nunUnsichtbar2;
					//fieldDefsColumnsUserDBGridAsStr[Column->Index] = nunUnsichtbar2;
			}

			if(rechts == 1)
			{
				std::pair<std::wstring,int> nunUnsichtbar2(links, 0);
				fieldDefsColumnsUserDBGridAsStr[index] = nunUnsichtbar2;
					//fieldDefsColumnsUserDBGridAsStr[Column->Index] = nunUnsichtbar2;
			}
			//ShowMessage("Index is : " + IntToStr(index) + " Column->Index is : " + IntToStr(Column->Index));
			break;
		}


	}
	 //Ist drinn

	//----ALT
	/*RRClasses::RRFTor1Condition2<std::wstring, int> ftor(RRTools::AnsiToWide(columnName), dummy);
	std::vector<std::pair<std::wstring, int> >::iterator itAllesBisherVomUserDropped = std::find_if(fieldDefsColumnsUserDBGridAsStr.begin(), fieldDefsColumnsUserDBGridAsStr.end(),ftor);

	if(itAllesBisherVomUserDropped != fieldDefsColumnsUserDBGridAsStr.end())
	{
		std::pair<std::wstring,int> nunRotausgewählt = *itAllesBisherVomUserDropped;
		std::wstring wstr= nunRotausgewählt.first;
		int j = nunRotausgewählt.second; nunRotausgewählt.second = 1;
		fieldDefsColumnsUserDBGridAsStr[Column->Index] = nunRotausgewählt;
	//ShowMessage("Angeklickte Spalte nun : " + RRTools::WStrToAnsiString(wstr) + " und der Status ist : " + IntToStr(nunRotausgewählt.second));

	}*/
	//----ALT


	DBGridUserChoice->Invalidate();
	DBGrid4XMLCard_Item->Invalidate();
	//ShowMessage("DBGridUserChoiceOnCellClick : Ende");

}

























/*
//------------------------------4--------------
Später
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	TForm *NewForm = new TForm(this);
	try
	{
		NewForm->Show();
		NewForm->BringToFront();
	}
	catch(...)
	{
		delete NewForm;
		throw;
	}
}
__




__

#include <memory>






void __fastcall TForm1::Button1Click(TObject *Sender)
{
    std::unique_ptr<NewForm> NewFormPtr(new NewForm(this));
    NewFormPtr->Show();
    NewFormPtr->BringToFront();
}



___

#include <SqlExpr.hpp>
#include <DBClient.hpp>

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    // Erstellen  eine SQL-Verbindung
    TSQLConnection *SQLConnection = new TSQLConnection(this);
    SQLConnection->DriverName = "MSSQL";
    SQLConnection->GetDriverFunc = "getSQLDriverMSSQL";
    SQLConnection->LibraryName = "dbxmss.dll";
    SQLConnection->VendorLib = "sqlncli10.dll";
    SQLConnection->Params->Values["HostName"] = "localhost";
    SQLConnection->Params->Values["Database"] = "mydatabase";
    SQLConnection->Params->Values["User_Name"] = "username";
    SQLConnection->Params->Values["Password"] = "password";
    SQLConnection->LoginPrompt = false;
    SQLConnection->Connected = true;

    // Erstellen  eine SQL-Abfrage
    TSQLQuery *SQLQuery = new TSQLQuery(this);
    SQLQuery->SQLConnection = SQLConnection;
    SQLQuery->SQL->Add("SELECT * FROM mytable");
    SQLQuery->Open();

    // Laden  die Daten in das ClientDataSet
    ClientDataSet1->Close();
    ClientDataSet1->LoadFromDataSet(SQLQuery, 0, lmAppend);
    ClientDataSet1->Open();

    // Bereinigen
    delete SQLQuery;
    delete SQLConnection;
}

___

//Besser…

#include <memory>
#include <SqlExpr.hpp>
#include <DBClient.hpp>

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    // Erstellen  eine SQL-Verbindung
    std::unique_ptr<TSQLConnection> SQLConnection(new TSQLConnection(this));
    // ... Setzen  die Verbindungsparameter ...

    // Erstellen  eine SQL-Abfrage
    std::unique_ptr<TSQLQuery> SQLQuery(new TSQLQuery(this));
    SQLQuery->SQLConnection = SQLConnection.get();
    // ... Führen  die Abfrage aus ...

    // Laden  die Daten in das ClientDataSet
    ClientDataSet1->Close();
    ClientDataSet1->LoadFromDataSet(SQLQuery.get(), 0, lmAppend);
    ClientDataSet1->Open();
}

___

*/
