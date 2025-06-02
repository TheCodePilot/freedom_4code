//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Neu_XML_TDBGrid.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Gradient"
#pragma resource "*.dfm"

TFormXML *FormXML;
//---------------------------------------------------------------------------

#include <algorithm>
#include <SqlExpr.hpp>
#include <DBClient.hpp>
#include <vector>
#include "Frame_X10K.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cmath>

namespace RR4XML
{
	static std::wstring ExtractBefore(const std::wstring& input, wchar_t* delimiter)
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


	static std::string MultiplyAndConvertBack(std::string str, int multiply, int precision)
	{
		float value = std::atof(str.c_str());

		value *= multiply;

		//Konvertieren zurück in einen String
		std::ostringstream streamObj;
		//Setze Fixed -Point Notation
		streamObj << std::fixed;

		streamObj << std::setprecision(precision);
		//Addiere float zum stream
		streamObj << value;
		//Bekomme string vom output string stream
		std::string strObj = streamObj.str();

		return strObj;
	}


	static std::string WStringToStr(std::wstring wstring)
	{
		AnsiString ansi(wstring.c_str());
		std::string str = ansi.c_str();
		return str;
	}

	static std::wstring StringToWStr(std::string str)
	{
		std::wstring wstr = std::wstring(str.begin(), str.end());
		return wstr;
	}


    // Funktion zur Konvertierung von std::wstring in int
	static int WstringToInt(const std::wstring& str)
	{
		std::wstringstream wss(str);
		int result;
		wss >> result;
		if (wss.fail()) {
			//std::wcerr << L"Konvertierungsfehler bei int: " << str << std::endl;
			return -1; // oder eine andere geeignete Fehlerbehandlung
		}
		return result;
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

	// Funktion zum Skalieren und Formatieren
	//Sollte bei UpdateDBGridUserChoiceVorAusWahl() und UpdateDBGridUserChoice() AddOtherRowsVorAusWahlDB AddOtherRowsDB je Testart
	//Zur Anwendung kommen
	static std::wstring CustomizedValue(std::wstring original, std::wstring symbol, std::wstring unit, std::wstring precision, std::wstring value)
	{
		int preci;
		double val;

		val = RR4XML::WstringToDouble(value);
		preci = RR4XML::WstringToInt(precision);

		if((val < 0.0)||(preci < 0))
			return original;

		const double factors[] = {1.0, 1e3, 1e6, 1e9, 1e12, 1e15, 1e18, 1e21, 1e24};
		const std::wstring prefixes[] = {L"", L"m", L"µ", L"n", L"p", L"f", L"a", L"z", L"y"};
		const int numFactors = sizeof(factors) / sizeof(factors[0]);
		std::wstring result;

		switch (unit[0])
		{
			case L'%':
			{
				double promilleValue = val * 10; // Umwandlung von Prozent zu Promille
				std::wstringstream ss;
				ss << std::fixed << std::setprecision(std::min(preci, 3)) << promilleValue;     //9
				result = ss.str() + L" ‰"; // Verwende das Promille-Symbol
				break;
			}
			default:
			{
				// Finde den Index des aktuellen Präfixes
				int symbolIndex = 0;
				for (int i = 0; i < numFactors; ++i)
				{
					if (prefixes[i] == symbol)
					{
						symbolIndex = i;
						break;
					}
				}

				// Finde die passende Skala
				int scaleIndex = symbolIndex; //0;
				double scaledValue = val;
				while (scaledValue < 1.0 && scaleIndex < numFactors - 1) {
					scaleIndex++;
					scaledValue *= 1e3;
				}

				// Berechne den neuen Präfix-Index
				int newSymbolIndex = symbolIndex + (scaleIndex - symbolIndex);
				if (newSymbolIndex >= numFactors)
				{
					newSymbolIndex = numFactors - 1; // Begrenze den Index, um nicht über das Array hinauszugehen
				}

				// Formatieren des Wertes
				std::wstringstream ss;
				ss << std::fixed << std::setprecision(std::min(preci, 3)) << scaledValue;      //9
				std::wstring formattedValue = ss.str();

				// Ergebnis zusammenstellen
				result = formattedValue + L" " + prefixes[newSymbolIndex] + unit;
				break;
			}
		}

		return result;
	}

	static std::wstring CustomizedValueNew(std::wstring original, std::wstring symbol, std::wstring unit, std::wstring precision, std::wstring value, bool bUseLookUpTable)
	{
		int preci;
		double val;

		val = RR4XML::WstringToDouble(value);
		preci = RR4XML::WstringToInt(precision);

		if(!bUseLookUpTable)
		{
			if((val < 0.0)||(preci < 0))
				return original;

			const double factors[] = {1.0, 1e3, 1e6, 1e9, 1e12, 1e15, 1e18, 1e21, 1e24};
			const std::wstring prefixes[] = {L"", L"m", L"µ", L"n", L"p", L"f", L"a", L"z", L"y"};
			const int numFactors = sizeof(factors) / sizeof(factors[0]);
			std::wstring result;

			switch (unit[0])
			{
				case L'%':
				{
					double promilleValue = val * 10; // Umwandlung von Prozent zu Promille
					std::wstringstream ss;
					ss << std::fixed << std::setprecision(std::min(preci, 3)) << promilleValue;     //9
					result = ss.str() + L" ‰"; // Verwende das Promille-Symbol
					break;
				}
				default:
				{
					// Finde den Index des aktuellen Präfixes
					int symbolIndex = 0;
					for (int i = 0; i < numFactors; ++i)
					{
						if (prefixes[i] == symbol)
						{
							symbolIndex = i;
							break;
						}
					}

					// Finde die passende Skala
					int scaleIndex = symbolIndex; //0;
					double scaledValue = val;
					while (scaledValue < 1.0 && scaleIndex < numFactors - 1) {
						scaleIndex++;
						scaledValue *= 1e3;
					}

					// Berechne den neuen Präfix-Index //Zunächst nicht ändern
					int newSymbolIndex = symbolIndex + (scaleIndex - symbolIndex);
					if (newSymbolIndex >= numFactors)
					{
						newSymbolIndex = numFactors - 1; // Begrenze den Index, um nicht über das Array hinauszugehen
					}

					// Formatieren des Wertes hier precision abhängig von der lookUpTabelle machen
					int after = 1;
					std::wstringstream ss;

					if(	(original == L"V0" ) || (original == L"Vx" ) )
					{
						after = 0;
					}

					ss << std::fixed << std::setprecision(std::min(preci, after)) << scaledValue;      //9
					std::wstring formattedValue = ss.str();

					// Ergebnis zusammenstellen
					result = formattedValue + L" " + prefixes[newSymbolIndex] + unit;
					break;
				}
			}

			return result;
		}
		else   //Einheit von CP prüfen
		{
			if((val < 0.0)||(preci < 0))
				return original;

			const double factors[] = {1.0, 1e3, 1e6, 1e9, 1e12, 1e15, 1e18, 1e21, 1e24};
			const std::wstring prefixes[] = {L"", L"m", L"µ", L"n", L"p", L"f", L"a", L"z", L"y"};
			const int numFactors = sizeof(factors) / sizeof(factors[0]);
			std::wstring result;

			switch (unit[0])
			{
				case L'%':
				{
					double promilleValue = val * 10; // Umwandlung von Prozent zu Promille
					std::wstringstream ss;
					ss << std::fixed << std::setprecision(std::min(preci, 2)) << promilleValue;     //9
					result = ss.str() + L" ‰"; // Verwende das Promille-Symbol
					break;
				}
				default:
				{
					double scaledValue = val;
					int newSymbolIndex;

						// Finde den Index des aktuellen Präfixes
					int symbolIndex = 0;
					for (int i = 0; i < numFactors; ++i)
					{
						if (prefixes[i] == symbol)
						{
							symbolIndex = i;
							break;
						}
					}

					// Finde die passende Skala
					int scaleIndex = symbolIndex; //0;

					while (scaledValue < 1.0 && scaleIndex < numFactors - 1) {
						scaleIndex++;
						scaledValue *= 1e3;
					}

					// Berechne den neuen Präfix-Index
					newSymbolIndex = symbolIndex + (scaleIndex - symbolIndex);
					if (newSymbolIndex >= numFactors)
					{
						newSymbolIndex = numFactors - 1; // Begrenze den Index, um nicht über das Array hinauszugehen
					}

					// Formatieren des Wertes
					int after = 1;
					std::wstringstream ss;
					if(	(original == L"V0" ) || (original == L"Vx" ) )
					{
						after = 0;
					}

					//Nachtrag wenn Fahrad als Einheit schauen ob manchmal auch in nF anstatt pF
					if(unit == L"F")
					{
						newSymbolIndex = 4; //also p
						if(symbol == L"n")
							scaledValue *= 1000.0; //von nano auf p
					}

					ss << std::fixed << std::setprecision(std::min(preci, after)) << scaledValue;      //9
					std::wstring formattedValue = ss.str();

					// Ergebnis zusammenstellen
					result = formattedValue + L" " + prefixes[newSymbolIndex] + unit;
					break;
				}
			}

			return result;
		}
	}

}

//#include <atomic> //leider von bcc32 nicht unterstützt :(

 __fastcall TFormXML::TFormXML(TComponent* Owner)
	: TForm(Owner), xmlProcess(new RRClasses::XMLData()),
	  OpenTextFileDialog(new TOpenTextFileDialog(this))
{
}

void __fastcall TFormXML::TFormXML_ (TComponent* Owner)
{
	showWindow = false;
	ersteSpalte = false;
	cbnLöscheSpalten = false;
	AnsiString clickEreignisCard_Item = L"";

	clickMeasureCard_Item = L"";
//ShowMessage("Step-5");
	xmlData = dynamic_cast<RRClasses::XMLData*>(xmlProcess.get());
	if(!xmlData){
		ShowMessage("TFormXML RRClasses::XMLData* xmlData = dynamic_cast<RRClasses::XMLData*>(xmlProcess): Problem");
		return;
	}
//ShowMessage("Step-4");
	//Nur bei der Wahl in der Combo-Box Vorauswahl ansttatt keine = true;
	bnUebertrage->Enabled = false;

	ComboBoxDBGridUserChoice->Parent = this;
	ComboBoxDBGridUserChoice->Style = csDropDownList;
	ComboBoxDBGridUserChoice->Items->Add("C1");
	ComboBoxDBGridUserChoice->Items->Add("MA");
	ComboBoxDBGridUserChoice->Items->Add("TAP");
	ComboBoxDBGridUserChoice->ItemIndex = 0;
//ShowMessage("Step-3");
	std::wstring fixeVorAuswahl = L"1|2|3|4|11|12|";
	//Beispiel für 2 Combo-Einträge
	//std::wstring fixeVorAuswahl = L"1|2|8|%3|11|4|";

	InitializeVorAusWahl(fixeVorAuswahl);

	ComboBoxVorauswahl->Parent = this;
	ComboBoxVorauswahl->Style = csDropDownList;
	ComboBoxVorauswahl->Enabled = false;
	ShowMessage("Step-2");
	fromDBGrid4XMLCard_Item = false;
	OpenTextFileDialog->InitialDir = GetCurrentDir();
	OpenTextFileDialog->Filter = "XML Files (*.xml)|*.xml|All Files (*.*)|*.*";
	UnicodeString ustr = U"";
	ustr = pFX10K->ustrXMLFile;
//ShowMessage("Pfad der Datei : " + ustr);

	Memo1->ReadOnly = true;
	Memo1->Clear();

	if(!ustr.IsEmpty())
	{
		this->BorderStyle = bsDialog;

		std::wstring wstr(ustr.w_str());
//ShowMessage("Step1");
		DataSource1->DataSet = ClientDataSet1;
		DBGrid4XMLin->DataSource = DataSource1;
		DBGrid4XMLin->ReadOnly = true;
		DBGrid4XMLin->DefaultDrawing = false;
//ShowMessage("Step2");
		ClientDataSet1->FieldDefs->Clear();
		ClientDataSet1->FieldDefs->Add("Name", ftString, 50);
		ClientDataSet1->FieldDefs->Add("Datum/Uhrzeit", ftString, 255);
		ShowMessage("Step3 Bevor ClientDataSet");
//ClientDataSet1->CreateDataSet();


		if (ClientDataSet1 != NULL) {
			try
			{
				ClientDataSet1->CreateDataSet();
			}
			catch (const Exception& e)
			{
			// Logge die Ausnahme
			ShowMessage("FEHLER: ClientDataSet1");
			}
		}
		else
		{
			ShowMessage("ClientDataSet1 ist NULL");
		}

		//Das zweite
		DataSource2->DataSet = ClientDataSet2;
		DBGrid4XMLCard_Item->DataSource = DataSource2;
		DBGrid4XMLCard_Item->ReadOnly = true; //RR142
		ClientDataSet2->FieldDefs->Clear();
		ClientDataSet2->FieldDefs->Add("Name der Messgroesse", ftString, 50);
		ClientDataSet2->FieldDefs->Add("Wert mit Einheit", ftString, 255);
//ClientDataSet2->CreateDataSet();
		if (ClientDataSet2 != NULL) {
			try
			{
				ClientDataSet2->CreateDataSet();
			}
			catch (const Exception& e)
			{
			// Logge die Ausnahme
			ShowMessage("FEHLER: ClientDataSet2");
			}
		}
		else
		{
			ShowMessage("ClientDataSet2 ist NULL");
		}

		//Das dritte
		DataSource3->DataSet = ClientDataSet3;
		DBGridUserChoice->DataSource = DataSource3;
		DBGridUserChoice->ReadOnly = true;
		ClientDataSet3->FieldDefs->Clear();ShowMessage("Step4");
		xmlProcess->LoadXMLDocument(wstr);
		ShowMessage("Step5");
		try
		{
			XML2Grid(xmlProcess.get());
			showWindow = true;
		}
			catch (const Exception &e)
		{
			ShowMessage("Beim einlesen der XML-Daten. An Position" + e.Message);

		}
	}
	else
	{
		ShowMessage("Sie haben die Aktion abgebrochen. Ohne die Wahl einer XML-Datei werden keine Daten geladen. Fenster bitte schliessen");
		showWindow = false;
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

			AnsiString clickEreignis = DBGrid4XMLin->Columns->Items[0]->Field->AsString;
			AnsiString clickDatum = DBGrid4XMLin->Columns->Items[1]->Field->AsString;

			std::wstring ereignis = RRTools::AnsiToWide(clickEreignis);
			std::wstring datum = RRTools::AnsiToWide(clickDatum);
			int i = DBGrid4XMLin->DataSource->DataSet->RecNo;// GetRecNo(); // Eins - basiert mit 1 beginnend

			if(xmlData->AnalyzeDOM_ProofTyp(i-1) == 0)//	Index der Speicherung bei XMLData 0 - basiert
			{
				//In diesem Frame auf Null belassen //
				xmlData->AnalyzeDOM_PicEventUwish(ereignis, datum, 0); //bei 1: Nur Jede Art von Messwert einmalig

				ClientDataSet2->EmptyDataSet();

				DBGrid4XMLCard_Item->DataSource->DataSet->Next();
				std::vector<std::pair<std::wstring, std::wstring> > data = xmlData->GetXMLInfo();

				//nun genau nur einmalig die verschiedenen dargestellt
				int max = xmlData->GetCountsOfDifferentsMeasu4GUI();
				if(max == -1)
				{
					ShowMessage("XML-Datei beschädigt oder keine Messdaten vorhanden. Vermutlich sollte es nur ein Kommentar werden beim Omnicron Gerät");
                    return;
				}

				int count = 0;
				for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it= data.begin(); it != data.end(); ++it)
				{
					const std::pair < std::wstring, std::wstring>& pair = *it;
					ClientDataSet2->Append();
					ClientDataSet2->FieldByName("Name der Messgroesse")->AsString = pair.first.c_str();
					ClientDataSet2->FieldByName("Wert mit Einheit")->AsString = pair.second.c_str();
					ClientDataSet2->Post();
                    ++count;
					if(count == max)  //alle einmalig in Liste
					{
						break;
                    }

				}

				DBGrid4XMLCard_Item->DataSource->DataSet->RecNo = 1;
				ComboBoxVorauswahl->Enabled = true;

				if(ComboBoxVorauswahl->ItemIndex == 0)
					bnUebertrage->Enabled = false;
				else
					bnUebertrage->Enabled = true;
			}
			else
			{
				std::wstring wcomment = xmlData->GetCommentIfType18();
				AnsiString ansi = RRTools::WStrToAnsiString(wcomment);

				ClientDataSet2->EmptyDataSet();
				Memo1->Clear();
				Memo1->Lines->Add(ansi);
				ComboBoxVorauswahl->Enabled = false;

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

	//User hat das schon angeklickt
	if (AllreadyChosed())
	{
		fromDBGrid4XMLCard_Item = false;
		return;
	}
	else
		fromDBGrid4XMLCard_Item = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::DBGridUserChoiceMouseEnter(TObject *Sender)
{
	if(fromDBGrid4XMLCard_Item == true)
	{
		if(ComboBoxVorauswahl->ItemIndex == 0)
		{
			cbnLoescheSpalte->Enabled = true;
			std::pair<int,std::wstring> data(currentRowCard_Item,RRTools::AnsiToWide(clickMeasureCard_Item));
			selectedRowsDBGrid4XMLCard_Item.push_back(data);
			DBGrid4XMLCard_Item->Invalidate();
			ersteSpalte = false;
			cbnLöscheSpalten = false;
			fromDBGrid4XMLCard_Item = false;

			NewColumnName();
			UpdateDBGridUserChoice();
		}

	}
}

//=============================================================================================================================================
//Hilfsfunktionen
//=============================================================================================================================================
void __fastcall TFormXML::NewColumnName()
{

	if(cbnLöscheSpalten)
	{
		cbnLöscheSpalten = false;
		ShowMessage("NewColumnName() : Returner");
		return;
	}

	bool neuerEintrag = true;
	bool comeback = false;

	int index = -1;
	int indexComeBack = -1;
	std::wstring wstrComeBack =L"";

	for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
	{
		++index;
		const std::pair < std::wstring, int>& pair2Proof = *it1;

		if(pair2Proof.second == 10)
		{
			comeback = true;
			wstrComeBack = pair2Proof.first;
			indexComeBack = index;
		}


		if(neuerEintrag && (RRTools::AnsiToWide(clickMeasureCard_Item) == pair2Proof.first))
		{
			neuerEintrag = false;
			break;
		}
	}

	if(comeback)
	{
		std::pair<std::wstring, int> comeBack(wstrComeBack, 0);
		fieldDefsColumnsUserDBGridAsStr[indexComeBack] = comeBack;

		ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(wstrComeBack), ftString, 12);
	}

	if( !comeback && ( (neuerEintrag)||(ClientDataSet3->FieldCount == 0) ) )
	{

		std::wstring dbHeader = ConvertXMLNameToDBHeader(RRTools::AnsiToWide(clickMeasureCard_Item));
		ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
		std::pair<std::wstring, int> newColoumn(RRTools::AnsiToWide(clickMeasureCard_Item), 0);

		//Was nun neu erstellt sich merken
		fieldDefsColumnsUserDBGridAsStr.push_back(newColoumn);
	}

	if(ClientDataSet3->Active)
	{
		ClientDataSet3->Close();
	}

	if(!ClientDataSet3->Active)
	{

		ClientDataSet3->CreateDataSet();
		ClientDataSet3->Open();
	}

	DBGridUserChoice->Refresh();
}

void __fastcall TFormXML::NewColumnNameVorAusWahl()
{
bool neuerEintrag = true;
	bool comeback = false;

	int index = -1;
	int indexComeBack = -1;
	std::wstring wstrComeBack =L"";

	for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
	{
		++index;
		const std::pair < std::wstring, int>& pair2Proof = *it1;

		if(pair2Proof.second == 10)
		{
			comeback = true;
			wstrComeBack = pair2Proof.first;
			indexComeBack = index;
		}


		if(neuerEintrag && (RRTools::AnsiToWide(clickMeasureCard_Item) == pair2Proof.first))
		{
			neuerEintrag = false;
			break;
		}
	}

	if(comeback)
	{
		std::pair<std::wstring, int> comeBack(wstrComeBack, 0);
		fieldDefsColumnsUserDBGridAsStr[indexComeBack] = comeBack;

		ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(wstrComeBack), ftString, 12);
	}

	if( !comeback && ( (neuerEintrag)||(ClientDataSet3->FieldCount == 0) ) )
	{
		//Hier nun Durchlauf des vektors Vorauswahl
		for(std::vector<std::pair<std::wstring, int> >::iterator it =
			fieldDefsColumnsVorauswahlAsStr.begin(); it != fieldDefsColumnsVorauswahlAsStr.end(); ++it)
		{
			std::pair<std::wstring, int> data = *it;
			std::wstring dbHeader = ConvertXMLNameToDBHeader(data.first);

			ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
			//Was nun neu erstellt sich merken
			std::pair<std::wstring, int> newColoumn(dbHeader, 0);
			fieldDefsColumnsUserDBGridAsStr.push_back(newColoumn);
		}
	}

	if(ClientDataSet3->Active)
	{
		ClientDataSet3->Close();
	}

	if(!ClientDataSet3->Active)
	{

		ClientDataSet3->CreateDataSet();
		ClientDataSet3->Open();
	}

	DBGridUserChoice->DataSource->DataSet->EnableControls();
	DBGridUserChoice->Refresh();
	DBGridUserChoice->DataSource->DataSet->DisableControls();

}


void __fastcall TFormXML::UpdateDBGridUserChoiceVorAusWahl()
{

	if(xmlProcess)
	{
		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;

		if(xmlData)
		{
			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsVorauswahlAsStr.begin(); it1 != fieldDefsColumnsVorauswahlAsStr.end(); ++it1)
			{
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was ngeklickt ist
					int index = -1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						++index;
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first;

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							std::pair<std::wstring, std::wstring> info(ValueOfXMLMeasure(pairOfAllXML.second, index), L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}

					if(!ersteSpalte)
					{
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

							ClientDataSet3->Append();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet3->FieldByName(dbHeader.c_str())->AsString = RRTools::WStrToAnsiString(pairMaxFromColumOne.first);

							ClientDataSet3->Post();
							ClientDataSet3->Next();
						}
						dataMaxFromColumOne.clear();
						ersteSpalte = true;
					}

					else
					{
						ClientDataSet3->RecNo = 1;
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

							ClientDataSet3->Edit();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet3->FieldByName(dbHeader.c_str())->AsString = RRTools::WStrToAnsiString(pairMaxFromColumOne.first);
							ClientDataSet3->Post();
							ClientDataSet3->Next();
						}
						dataMaxFromColumOne.clear();
					}

				}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))
			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...

			DBGridUserChoice->DataSource->DataSet->EnableControls();
			DBGridUserChoice->Refresh();
			DBGridUserChoice->DataSource->DataSet->DisableControls();

		}
	}


}


void __fastcall TFormXML::UpdateDBGridUserChoice()
{
	if(xmlProcess)
	{
		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;

		if(xmlData)
		{
			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			//Schleife der Spalten- Überschrift ; Achtung deletes noch abfangen
			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
			{
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was angeklickt ist
					int index = -1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						++index;
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first;

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							std::pair<std::wstring, std::wstring> info(ValueOfXMLMeasure(pairOfAllXML.second, index), L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}


					if(!ersteSpalte)
					{
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

							ClientDataSet3->Append();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet3->FieldByName(dbHeader.c_str())->AsString = RRTools::WStrToAnsiString(pairMaxFromColumOne.first);
							ClientDataSet3->Post();
							ClientDataSet3->Next();
						}
						dataMaxFromColumOne.clear();
						ersteSpalte = true;
					}

					else
					{
						ClientDataSet3->RecNo = 1;
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

							ClientDataSet3->Edit();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet3->FieldByName(dbHeader.c_str())->AsString = RRTools::WStrToAnsiString(pairMaxFromColumOne.first);
							ClientDataSet3->Post();
							ClientDataSet3->Next();
						}
						dataMaxFromColumOne.clear();
					}
				}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))
			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...

			DBGridUserChoice->DataSource->DataSet->EnableControls();
			DBGridUserChoice->Refresh();
			DBGridUserChoice->DataSource->DataSet->DisableControls();

		}
	}

}

//=============================================================================================================================================
//
//=============================================================================================================================================

void __fastcall TFormXML::DBGrid4XMLCard_ItemMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	Screen->Cursor = crDrag;

}
//---------------------------------------------------------------------------

void __fastcall TFormXML::DBGrid4XMLCard_ItemMouseUp(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	Screen->Cursor = crDefault;
}
//--------------------------------------------------------------------------

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
		DBGrid4XMLCard_Item->Canvas->Brush->Color = clSkyBlue;
	}
	else
	{
		DBGrid4XMLCard_Item->Canvas->Brush->Color = clWindow;
	}

	DBGrid4XMLCard_Item->DefaultDrawColumnCell(Rect, DataCol, Column, State);
}

//---------------------------------------------------------------------------

void __fastcall TFormXML::cbnLoescheSpalteOnClick(TObject *Sender)
{
	if( DBGridUserChoice->DataSource->DataSet->IsEmpty() )
	{
		return;
	}

	fieldCounterToDelete = 0;

	for(size_t i = 0; i < selectedColumnsUserDBGrid4Draw.size(); ++i)
	{
		std::pair<int,int> nunUnsichtbar = selectedColumnsUserDBGrid4Draw[i];
		if( selectedColumnsUserDBGrid4Draw[i].second == 1)
		{
			std::pair<int,int> nunUnsichtbar(selectedColumnsUserDBGrid4Draw[i].first, 1);  //von 2 weg: gehen
			selectedColumnsUserDBGrid4Draw[i] = nunUnsichtbar;
			DBGridUserChoice->Columns->Items[nunUnsichtbar.first]->Visible = false;
		}
	}

	//wegen rot-Verschiebung nach Visible = false
	//Alle nun wieder nicht rot angeglickt darstellen also second = 0 setzen;
	for(size_t i = 0; i < selectedColumnsUserDBGrid4Draw.size(); ++i)
	{
		selectedColumnsUserDBGrid4Draw[i].second = 0;
	}

	//angeklickte Spalte nun notieren
	//Hier muß auch Grafisch wieder Grid2 frei gegeben werden
	for(size_t i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{
		std::pair<std::wstring,int> nunUnsichtbar = fieldDefsColumnsUserDBGridAsStr[i];
		if(fieldDefsColumnsUserDBGridAsStr[i].second == 1)
		{
			++fieldCounterToDelete;
			std::pair<std::wstring,int> nunUnsichtbar(fieldDefsColumnsUserDBGridAsStr[i].first, 2);
			std::wstring wstr= nunUnsichtbar.first;
			fieldDefsColumnsUserDBGridAsStr[i] = nunUnsichtbar;
			G3ToGrid2Synchron(fieldDefsColumnsUserDBGridAsStr[i].first);
		}
	}

	ersteSpalte = false;

	//SonderFall Button gedrückt obwohl nichts rot ist
	if(fieldCounterToDelete == 0)
		return;

	if(SomeFieldsExist() >= 1)
		BuildRestOfFieldDefs();
	else
		IniCard_Item();

	cbnLöscheSpalten = true;
	if(ClientDataSet3->FieldCount > 0)
		UpdateDBGridUserChoice();
	DBGrid4XMLCard_Item->Invalidate();
	
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::DBGridUserChoiceOnDraw(TObject *Sender, const TRect &Rect,
		  int DataCol, TColumn *Column, TGridDrawState State)
{
	RRClasses::RRFTorPairs<int,int> ftor(Column->Index, 1);
	std::vector<std::pair<int, int> >::iterator it2 =
		std::find_if(selectedColumnsUserDBGrid4Draw.begin(), selectedColumnsUserDBGrid4Draw.end(),ftor);

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
	if( DBGridUserChoice->DataSource->DataSet->IsEmpty() )
	{
		return;
	}

	{
		IniCard_Item();
		DBGrid4XMLCard_Item->Invalidate();
	}
}
//---------------------------------------------------------------------------



void __fastcall TFormXML::bnUebertrageOnClick(TObject *Sender)
{

	if( DBGridUserChoice->DataSource->DataSet->IsEmpty() )
	{
		return;
	}

	bool bOK = false;
	AnsiString ausgewaehlterText = ComboBoxDBGridUserChoice->Text;

	if(ausgewaehlterText == "C1")
	{
		bOK = Transfer2DB_C1();
		if(bOK)
			pFX10K->spbToXML->Enabled = false;
	}
	else if(ausgewaehlterText == "MA")
	{
		bOK = Transfer2DB_MA();
        if(bOK)
			pFX10K->spbToXML->Enabled = false;
	}
	else if(ausgewaehlterText == "TAP")
	{
		bOK = Transfer2DB_TAP();
        if(bOK)
			pFX10K->spbToXML->Enabled = false;
	}
	if(bOK)
		FormXML->Close();
}
//---------------------------------------------------------------------------

//Also schaue wo second == dem vom User gelöschten Spalte ist
//selectedRowsDBGrid4XMLCard_Itemsecond == aktuell vom User gewählte Spalte
void __fastcall TFormXML::G3ToGrid2Synchron(std::wstring wstr)
{
	int index = -1;

	for(std::vector<std::pair<int, std::wstring> >::iterator it =
			selectedRowsDBGrid4XMLCard_Item.begin(); it != selectedRowsDBGrid4XMLCard_Item.end(); ++it)
	{
		++index;
		std::pair<int, std::wstring> compare = *it;

		if(compare.second == wstr)
		{
			std::vector<std::pair<int, std::wstring> >::iterator it4erase = selectedRowsDBGrid4XMLCard_Item.begin() + index;
			selectedRowsDBGrid4XMLCard_Item.erase(it4erase);
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
		// Das Dataset ist aktiv, also zuerst schließen
		ClientDataSet3->Close();
		ClientDataSet3->FieldDefs->Clear();
	}

    // Definieren der FieldDefs
	for(size_t i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{
		std::pair<std::wstring,int> nunUnsichtbar = fieldDefsColumnsUserDBGridAsStr[i];
		if(fieldDefsColumnsUserDBGridAsStr[i].second == 0) //1 bedeutet ja vorbereitet also wird ja bei Button immer zu 2 oder 1 zu 0 durch zurück nehmen
		{
			std::pair<std::wstring,int> nunUnsichtbar(fieldDefsColumnsUserDBGridAsStr[i].first, fieldDefsColumnsUserDBGridAsStr[i].second);
			std::wstring wstr= nunUnsichtbar.first;
			std::wstring dbHeader = ConvertXMLNameToDBHeader(fieldDefsColumnsUserDBGridAsStr[i].first);
			ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
		}
	}

	if(fieldCounterToDelete > 0) //1-basiert
		ClientDataSet3->CreateDataSet();

	ersteSpalte = false;
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
				std::pair<std::wstring,int> nunUnsichtbar2(links, 10); //Besonderheit das Feld zu generieren, direkt danach auf 0
				fieldDefsColumnsUserDBGridAsStr[index] = nunUnsichtbar2;
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

	//Für Kundenwunsch
	fieldDefsColumnsVorauswahlAsStr.clear();

	currentRowCard_Item = -1;
	clickMeasureCard_Item = L"";
	fieldCounterToDelete = 0;
}

int __fastcall TFormXML::SomeFieldsExist()
{
	int returner = 0;
	for(size_t i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{

		if(fieldDefsColumnsUserDBGridAsStr[i].second == 0)
		{
			++returner;
			break;
		}
	}
	return returner;
}

void __fastcall TFormXML::DBGridUserChoiceOnCellClick(TColumn *Column)
{
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
			break;
		}
	}

	if(neuerEintrag)
	{
		std::pair<int,int> angeklickt(Column->Index,1);
		selectedColumnsUserDBGrid4Draw.push_back(angeklickt);
	}

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
			}

			if(rechts == 1)
			{
				std::pair<std::wstring,int> nunUnsichtbar2(links, 0);
				fieldDefsColumnsUserDBGridAsStr[index] = nunUnsichtbar2;
			}
			break;
		}


	}

	DBGridUserChoice->Invalidate();
	DBGrid4XMLCard_Item->Invalidate();
}

void __fastcall TFormXML::XMLShow(TObject *Sender)
{
	TFormXML_ (this);
}

//---------------------------------------------------------------------------

bool __fastcall TFormXML::Transfer2DB_C1()
{

	pFX10K->table1->BeginUpdate();
	DataSource4->DataSet = ClientDataSet4;
	pFX10K->table1->DataSource = DataSource4;
	pFX10K->table1->ReadOnly = true;

	ClientDataSet4->FieldDefs->Clear();

	if(ComboBoxVorauswahl->ItemIndex > 0)
	{
		//Spaltenbezeichnung
		for(std::vector<std::pair<std::wstring, int> >::iterator it1 =
			fieldDefsColumnsVorauswahlAsStr.begin(); it1 != fieldDefsColumnsVorauswahlAsStr.end(); ++it1)
		{
			const std::pair < std::wstring, int>& pair4UserChoice = *it1;

			if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
			{
				std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
				ClientDataSet4->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
			}
		}
	}

	//Fall keine Automatisierung über die Combobox
	else
	{
		//Spaltenbezeichnung
		for(std::vector<std::pair<std::wstring, int> >::iterator it1 =
			fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
		{
			const std::pair < std::wstring, int>& pair4UserChoice = *it1;

			if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
			{
				std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
				ClientDataSet4->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
			}
		}

	}


	bool cdOK = InitCDS(4);
	if(!cdOK){ return false; }

	if(ComboBoxVorauswahl->ItemIndex > 0)
	{
		AddOtherRowsVorAusWahlDB_C1();
	}
	else
	{
		AddOtherRowsDB_C1();
	}

	pFX10K->xml4table1GUI = true;
	pFX10K->table1RememberOfXML.clear();
	pFX10K->IniTable1RememberOfXML();
	pFX10K->table1->EndUpdate();

	ClientDataSet4->RecNo = 1;

	return true;

}

bool __fastcall TFormXML::Transfer2DB_MA()
{
	pFX10K->table2->BeginUpdate();
	DataSource5->DataSet = ClientDataSet5;
	pFX10K->table2->DataSource = DataSource5;
	pFX10K->table2->ReadOnly = true;

	ClientDataSet5->FieldDefs->Clear();

	//Spaltenbezeichnung
    if(ComboBoxVorauswahl->ItemIndex > 0)
	{
		//Spaltenbezeichnung
		for(std::vector<std::pair<std::wstring, int> >::iterator it1 =
			fieldDefsColumnsVorauswahlAsStr.begin(); it1 != fieldDefsColumnsVorauswahlAsStr.end(); ++it1)
		{
			const std::pair < std::wstring, int>& pair4UserChoice = *it1;

			if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
			{
				std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
				ClientDataSet5->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
			}
		}
	}

	//Fall keine Automatisierung über die Combobox
	else
	{
		//Spaltenbezeichnung
		for(std::vector<std::pair<std::wstring, int> >::iterator it1 =
			fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
		{
			const std::pair < std::wstring, int>& pair4UserChoice = *it1;

			if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
			{
				std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
				ClientDataSet5->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
			}
		}

	}

	bool cdOK = InitCDS(5);
	if(!cdOK){ return false; }

	if(ComboBoxVorauswahl->ItemIndex > 0)
	{
		AddOtherRowsVorAusWahlDB_MA();
	}
	else
	{
		AddOtherRowsDB_MA();
	}


	pFX10K->xml4table2GUI = true;
	pFX10K->table2RememberOfXML.clear();
	pFX10K->IniTable2RememberOfXML();
	pFX10K->table2->EndUpdate();

	ClientDataSet5->RecNo = 1;

	return true;

}

bool __fastcall TFormXML::Transfer2DB_TAP()
{
	pFX10K->table3->BeginUpdate();
	DataSource6->DataSet = ClientDataSet6;
	pFX10K->table3->DataSource = DataSource6;
	pFX10K->table3->ReadOnly = true;

	ClientDataSet6->FieldDefs->Clear();

	//Spaltenbezeichnung
    if(ComboBoxVorauswahl->ItemIndex > 0)
	{
		//Spaltenbezeichnung
		for(std::vector<std::pair<std::wstring, int> >::iterator it1 =
			fieldDefsColumnsVorauswahlAsStr.begin(); it1 != fieldDefsColumnsVorauswahlAsStr.end(); ++it1)
		{
			const std::pair < std::wstring, int>& pair4UserChoice = *it1;

			if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
			{
				std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
				ClientDataSet6->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
			}
		}
	}

	//Fall keine Automatisierung über die Combobox
	else
	{
		//Spaltenbezeichnung
		for(std::vector<std::pair<std::wstring, int> >::iterator it1 =
			fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
		{
			const std::pair < std::wstring, int>& pair4UserChoice = *it1;

			if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
			{
				std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
				ClientDataSet6->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
			}
		}

	}

	bool cdOK = InitCDS(6);
	if(!cdOK){ return false; }

	if(ComboBoxVorauswahl->ItemIndex > 0)
	{
		AddOtherRowsVorAusWahlDB_TAP();
	}
	else
	{
		AddOtherRowsDB_TAP();
	}

	pFX10K->xml4table3GUI = true;
	pFX10K->table3RememberOfXML.clear();
	pFX10K->IniTable3RememberOfXML();
	pFX10K->table3->EndUpdate();

	ClientDataSet6->RecNo = 1;

	return true;
}

void __fastcall TFormXML::AddOtherRowsDB_C1()
{
	bool ersteSpalte1 = false;

	if(xmlProcess)
	{
		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;

		if(xmlData)
		{
			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			//Schleife der Spalten- Überschrift ; Achtung deletes noch abfangen
			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
			{
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was ngeklickt ist
					int index = -1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						++index;
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first;

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							//std::pair<std::wstring, std::wstring> info(pairOfAllXML.second, L" ");
							std::pair<std::wstring, std::wstring> info(ValueOfXMLMeasure(pairOfAllXML.second, index), L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}

					if(!ersteSpalte1)
					{
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

							ClientDataSet4->Append();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet4->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet4->Post();
							ClientDataSet4->Next();
						}
						dataMaxFromColumOne.clear();
						ersteSpalte1 = true;
					}

					else
					{
						ClientDataSet4->RecNo = 1;
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

							ClientDataSet4->Edit();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet4->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet4->Post();
							ClientDataSet4->Next();
						}
						dataMaxFromColumOne.clear();
					}
				}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))
			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...

		}
	}

}

void __fastcall TFormXML::AddOtherRowsVorAusWahlDB_C1()
{
	bool ersteSpalte1 = false;

	if(xmlProcess)
	{
		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;

		if(xmlData)
		{
			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			//Schleife der Spalten- Überschrift ; Achtung deletes noch abfangen
			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsVorauswahlAsStr.begin(); it1 != fieldDefsColumnsVorauswahlAsStr.end(); ++it1)
			{
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was ngeklickt ist
					int index = -1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						++index;
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first;

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							//std::pair<std::wstring, std::wstring> info(pairOfAllXML.second, L" ");
							std::pair<std::wstring, std::wstring> info(ValueOfXMLMeasure(pairOfAllXML.second, index), L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}

					if(!ersteSpalte1)
					{
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

							ClientDataSet4->Append();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet4->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet4->Post();
							ClientDataSet4->Next();
						}
						dataMaxFromColumOne.clear();
						ersteSpalte1 = true;
					}

					else
					{
						ClientDataSet4->RecNo = 1;
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

							ClientDataSet4->Edit();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet4->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet4->Post();
							ClientDataSet4->Next();
						}
						dataMaxFromColumOne.clear();
					}
				}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))
			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...
		}
	}


}

void __fastcall TFormXML::AddOtherRowsDB_MA()
{
	bool ersteSpalte1 = false;

	if(xmlProcess)
	{
		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;

		if(xmlData)
		{
			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			//Schleife der Spalten- Überschrift ; Achtung deletes noch abfangen
			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
			{
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was ngeklickt ist
					int index = -1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						++index;
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first;

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							//std::pair<std::wstring, std::wstring> info(pairOfAllXML.second, L" ");
							std::pair<std::wstring, std::wstring> info(ValueOfXMLMeasure(pairOfAllXML.second, index), L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}

					if(!ersteSpalte1)
					{
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

							ClientDataSet5->Append();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet5->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet5->Post();
							ClientDataSet5->Next();
						}
						dataMaxFromColumOne.clear();
						ersteSpalte1 = true;
					}

					else
					{
						ClientDataSet5->RecNo = 1;
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

							ClientDataSet5->Edit();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet5->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet5->Post();
							ClientDataSet5->Next();
						}
						dataMaxFromColumOne.clear();
					}
				}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))
			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...

		}
	}

}

void __fastcall TFormXML::AddOtherRowsVorAusWahlDB_MA()
{
	bool ersteSpalte1 = false;

	if(xmlProcess)
	{
		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;

		if(xmlData)
		{
			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			//Schleife der Spalten- Überschrift ; Achtung deletes noch abfangen
			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsVorauswahlAsStr.begin(); it1 != fieldDefsColumnsVorauswahlAsStr.end(); ++it1)
			{
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was ngeklickt ist
                    int index = -1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						++index;
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first;

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							//std::pair<std::wstring, std::wstring> info(pairOfAllXML.second, L" ");
							std::pair<std::wstring, std::wstring> info(ValueOfXMLMeasure(pairOfAllXML.second, index), L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}

					if(!ersteSpalte1)
					{
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

							ClientDataSet5->Append();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet5->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet5->Post();
							ClientDataSet5->Next();
						}
						dataMaxFromColumOne.clear();
						ersteSpalte1 = true;
					}

					else
					{
						ClientDataSet5->RecNo = 1;
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

							ClientDataSet5->Edit();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet5->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet5->Post();
							ClientDataSet5->Next();
						}
						dataMaxFromColumOne.clear();
					}
				}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))
			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...

		}
	}


}

void __fastcall TFormXML::AddOtherRowsDB_TAP()
{

    bool ersteSpalte1 = false;

	if(xmlProcess)
	{
		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;

		if(xmlData)
		{
			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			//Schleife der Spalten- Überschrift ; Achtung deletes noch abfangen
			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin(); it1 != fieldDefsColumnsUserDBGridAsStr.end(); ++it1)
			{
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was ngeklickt ist
					int index = -1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						++index;
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first;

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							//std::pair<std::wstring, std::wstring> info(pairOfAllXML.second, L" ");
							std::pair<std::wstring, std::wstring> info(ValueOfXMLMeasure(pairOfAllXML.second, index), L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}

					if(!ersteSpalte1)
					{
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

							ClientDataSet6->Append();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet6->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet6->Post();
							ClientDataSet6->Next();
						}
						dataMaxFromColumOne.clear();
						ersteSpalte1 = true;
					}

					else
					{
						ClientDataSet6->RecNo = 1;
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

							ClientDataSet6->Edit();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet6->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet6->Post();
							ClientDataSet6->Next();
						}
						dataMaxFromColumOne.clear();
					}
				}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))
			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...

		}
	}

}

void __fastcall TFormXML::AddOtherRowsVorAusWahlDB_TAP()
{
	bool ersteSpalte1 = false;

	if(xmlProcess)
	{
		std::vector<std::pair<std::wstring, std::wstring> > dataMaxFromColumOne;

		if(xmlData)
		{
			//Was vom XML kommt : first ist Name der Messgroesse also zum Beispiel V0; second ist der Wert wie 500 V
			std::vector<std::pair<std::wstring, std::wstring> > dataAllXML = xmlData->GetXMLInfo(); // alle enthalten und erst beim konkreten füllen für dataMaxFromColumOne

			//Schleife der Spalten- Überschrift ; Achtung deletes noch abfangen
			for(std::vector<std::pair<std::wstring, int> >::iterator it1 = fieldDefsColumnsVorauswahlAsStr.begin(); it1 != fieldDefsColumnsVorauswahlAsStr.end(); ++it1)
			{
				const std::pair < std::wstring, int>& pair4UserChoice = *it1;

				if((pair4UserChoice.second == 0) ||(pair4UserChoice.second == 1))
				{
					//Nun schauen wo überall gleiche Userchoice als Einheit weil nun davor geschaut wurde was ngeklickt ist
					int index = -1;
					for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it2 = dataAllXML.begin(); it2 != dataAllXML.end(); ++it2)
					{
						++index;
						const std::pair < std::wstring, std::wstring>& pairOfAllXML = *it2;

						std::wstring w1User = pair4UserChoice.first;
						std::wstring w3XML = pairOfAllXML.first;

						//wenn in großen Pool von XML ein bereits gespeichertes UserEreignis gleich dann weiter schauen
						if((w1User != L"") && (w1User == w3XML))
						{                                         // L" " : zweite hier nicht gebraucht, warte Kundenwunsch ab
							//std::pair<std::wstring, std::wstring> info(pairOfAllXML.second, L" ");
							std::pair<std::wstring, std::wstring> info(ValueOfXMLMeasure(pairOfAllXML.second, index), L" ");
							dataMaxFromColumOne.push_back(info);// temporäre Hilfspair
						}
					}

					if(!ersteSpalte1)
					{
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it3 = dataMaxFromColumOne.begin(); it3 != dataMaxFromColumOne.end(); ++it3)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it3;

							ClientDataSet6->Append();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet6->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet6->Post();
							ClientDataSet6->Next();
						}
						dataMaxFromColumOne.clear();
						ersteSpalte1 = true;
					}

					else
					{
						ClientDataSet6->RecNo = 1;
						for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it4 = dataMaxFromColumOne.begin(); it4 != dataMaxFromColumOne.end(); ++it4)
						{
							const std::pair < std::wstring, std::wstring>& pairMaxFromColumOne = *it4;

							ClientDataSet6->Edit();

							std::wstring dbHeader = ConvertXMLNameToDBHeader(pair4UserChoice.first);
							ClientDataSet6->FieldByName(dbHeader.c_str())->AsString = WStrToAnsiString4FieldName(pairMaxFromColumOne.first);

							ClientDataSet6->Post();
							ClientDataSet6->Next();
						}
						dataMaxFromColumOne.clear();
					}
				}   //if((pair4UserChoice.second == 0)||(pair4UserChoice.second == 1))
			} //for ... ::iterator it1 = fieldDefsColumnsUserDBGridAsStr.begin() ...

		}
	}

}


bool __fastcall TFormXML::InitCDS(int clientNumber)
{
	switch(clientNumber)
	{

		case 4:
			if(ClientDataSet4->Active)
			{
				ClientDataSet4->Close();
				return true;
			}

			if(!ClientDataSet4->Active)
			{
				if(ClientDataSet4->FieldDefList->Count > 0)
				{
					ClientDataSet4->CreateDataSet();
					ClientDataSet4->Open();
					return true;
				}
				else
				{
					return false;
				}
			}
			break;

		case 5:
			if(ClientDataSet5->Active)
			{
				ClientDataSet5->Close();
				return true;
			}

			if(!ClientDataSet5->Active)
			{
				if(ClientDataSet5->FieldDefList->Count > 0)
				{
					ClientDataSet5->CreateDataSet();
					ClientDataSet5->Open();
					return true;
				}
				else
				{
					return false;
				}
			}
			break;

	   case 6:
			if(ClientDataSet6->Active)
			{
				ClientDataSet6->Close();
				return true;
			}

			if(!ClientDataSet6->Active)
			{
					if(ClientDataSet6->FieldDefList->Count > 0)
				{
					ClientDataSet6->CreateDataSet();
					ClientDataSet6->Open();
					return true;
				}
				else
				{
					return false;
				}
			}
			break;
	}
	return false;
}


void __fastcall TFormXML::TFormXMLOnDestroy(TObject *Sender)
{
	//Diese 3 nur für das FormXML
	if(ClientDataSet1->Active)
		ClientDataSet1->Close();
	if(ClientDataSet2->Active)
		ClientDataSet2->Close();
	if(ClientDataSet3->Active)
		ClientDataSet3->Close();

	//Diese 3 sind mit table1,table2 und table3 vom FrameX10K verbunden also hier nichts machen !!!
	//ausser:
	ClientDataSet4->DisableControls();
	ClientDataSet5->DisableControls();
	ClientDataSet6->DisableControls();
}
//---------------------------------------------------------------------------

std::wstring  __fastcall TFormXML::ConvertXMLNameToDBHeader(std::wstring wstr)
{
	//---------------------
		for(std::vector<std::pair<std::wstring, std::wstring> >::iterator it =
			XML_Name2DataBase_Name_.begin(); it != XML_Name2DataBase_Name_.end(); ++it)
		{
			std::pair<std::wstring, std::wstring> data = *it;
			//Hier nun Durchlauf des vektors Vorauswahl
			if(data.first == wstr)
			{
				return data.second;
			}

		}
		return L"";
}

//=============================================================================================================================================
void __fastcall TFormXML::GeneratePairs4Header_()
//=============================================================================================================================================
{
	std::pair<std::wstring , std::wstring> data(L"PROT_ID", L"PROT_ID");
	XML_Name2DataBase_Name_.push_back(data);
	std::pair<std::wstring , std::wstring> data1(L"V0", L"Utest in Volt");  //1
	XML_Name2DataBase_Name_.push_back(data1);
	std::pair<std::wstring , std::wstring> data2(L"Vx", L"Uist in Volt");   //2
	XML_Name2DataBase_Name_.push_back(data2);
	std::pair<std::wstring , std::wstring> data3(L"f", L"Frequenz in Hz");  //3
	XML_Name2DataBase_Name_.push_back(data3);
	std::pair<std::wstring , std::wstring> data4(L"DF", L"tan in ‰");    //4
	XML_Name2DataBase_Name_.push_back(data4);
	std::pair<std::wstring , std::wstring> data5(L"TS", L"TS");
	XML_Name2DataBase_Name_.push_back(data5);
	std::pair<std::wstring , std::wstring> data6(L"Test_Art", L"Test_Art");
	XML_Name2DataBase_Name_.push_back(data6);
	std::pair<std::wstring , std::wstring> data7(L"markedASdestroyed", L"markedASdestroyed");
	XML_Name2DataBase_Name_.push_back(data7);
	std::pair<std::wstring , std::wstring> data8(L"Ix", L"Ix");     // Index 8
	XML_Name2DataBase_Name_.push_back(data8);
	std::pair<std::wstring , std::wstring> data9(L"CompMeas1", L"CompMeas1");
	XML_Name2DataBase_Name_.push_back(data9);
	std::pair<std::wstring , std::wstring> data10(L"CompMeas2", L"CompMeas2");  //10
	XML_Name2DataBase_Name_.push_back(data10);
	std::pair<std::wstring , std::wstring> data11(L"Cp", L"Cp in pF");    //11
	XML_Name2DataBase_Name_.push_back(data11);
	std::pair<std::wstring , std::wstring> data12(L"PF", L"PF in ‰");
	XML_Name2DataBase_Name_.push_back(data12);
	std::pair<std::wstring , std::wstring> data13(L"Pt", L"Pt");
	XML_Name2DataBase_Name_.push_back(data13);
	std::pair<std::wstring , std::wstring> data14(L"P10kV", L"P10KV");   //Wichtig kleines k beim XML
	XML_Name2DataBase_Name_.push_back(data14);
	std::pair<std::wstring , std::wstring> data15(L"Qt", L"Qt");
	XML_Name2DataBase_Name_.push_back(data15);
	std::pair<std::wstring , std::wstring> data16(L"St", L"St");
	XML_Name2DataBase_Name_.push_back(data16);
	std::pair<std::wstring , std::wstring> data17(L"Zabs", L"Zabs");
	XML_Name2DataBase_Name_.push_back(data17);
	std::pair<std::wstring , std::wstring> data18(L"Zphase", L"Zphase");
	XML_Name2DataBase_Name_.push_back(data18);
	std::pair<std::wstring , std::wstring> data19(L"Rp", L"Rp");
	XML_Name2DataBase_Name_.push_back(data19);
	std::pair<std::wstring , std::wstring> data20(L"Rs", L"Rs");
	XML_Name2DataBase_Name_.push_back(data20);
	std::pair<std::wstring , std::wstring> data21(L"Q", L"Q");  // Index 21
	XML_Name2DataBase_Name_.push_back(data21);
	std::pair<std::wstring , std::wstring> data22(L"TempU", L"TempU");
	XML_Name2DataBase_Name_.push_back(data22);
	std::pair<std::wstring , std::wstring> data23(L"TempDfg", L"TempDfg");
	XML_Name2DataBase_Name_.push_back(data23);
	std::pair<std::wstring , std::wstring> data24(L"Feuchte", L"Feuchte");
	XML_Name2DataBase_Name_.push_back(data24);
	std::pair<std::wstring , std::wstring> data25(L"Kommentar", L"Kommentar");
	XML_Name2DataBase_Name_.push_back(data25);

}

AnsiString __fastcall TFormXML::WStrToAnsiString4FieldName(std::wstring wstrin)
{
	AnsiString ansi = RRTools::WStrToAnsiString(wstrin);
	return ansi;
}

//wenn fehlschlägt gibt ed das original zurück
std::wstring __fastcall TFormXML::ValueOfXMLMeasure(std::wstring wstrOriginal, int indexOfXML)
{
	if(xmlData)
	{
		//Werte aus dem XML zusammentragen
		std::vector<std::pair<std::wstring, int> > dataMulti = xmlData->GetXMLInfoMulti();
		const std::pair<std::wstring, int>& pairMulti = dataMulti[indexOfXML];
		std::wstring symbol = pairMulti.first;

		std::vector<std::pair<std::wstring, int> > dataUnit = xmlData->GetXMLInfoUnit();
		const std::pair<std::wstring, int>& pairUnit = dataUnit[indexOfXML];
		std::wstring unit = pairUnit.first;

		std::vector<std::pair<std::wstring, int> > dataPrecision = xmlData->GetXMLInfoPrecision();
		const std::pair<std::wstring, int>& pairPrecision = dataPrecision[indexOfXML];
		std::wstring precision = pairPrecision.first;

		std::vector<std::pair<std::wstring, int> > dataValue = xmlData->GetXMLInfoValue();
		const std::pair<std::wstring, int>& pairValue = dataValue[indexOfXML];
		std::wstring value = pairValue.first;

		return RR4XML::CustomizedValueNew(wstrOriginal, symbol, unit, precision, value, true);
	}
	return wstrOriginal;
}

//Bei der ComboBox kann ich die prakmatischen Werte als View geben
void __fastcall TFormXML::InitializeVorAusWahl(std::wstring vorauswahl)
{
	GeneratePairs4Header_();
	vorauswahl4ComboBox.clear();
	indexBereiche.clear(); //Für ComboBox

	std::vector<int> vIndex;
	std::vector<std::wstring> CaptionCombo;

	std::size_t start = 0;
	std::size_t end = vorauswahl.find(L'%');

	//Immer als Index 0 ist
	ComboBoxVorauswahl->Items->Add(RRTools::WStrToAnsiString(L"Keine"));

	while (end != std::wstring::npos) {
		indexBereiche.push_back(vorauswahl.substr(start, end - start));
		start = end + 1;
		end = vorauswahl.find(L'%', start);
	}
	indexBereiche.push_back(vorauswahl.substr(start));

	std::wstring teilString;
	std::wstring oneCaptionComboBox = L"";
	int indexXML_Name2DataBase_Name;
	for (size_t i = 0; i < indexBereiche.size(); ++i)
	{
		teilString = indexBereiche[i];

		start = 0;
		oneCaptionComboBox = L"";
		end = teilString.find(L'|');
		while (end != std::wstring::npos) {
			std::wstring tempString = teilString.substr(start, end - start);
			indexXML_Name2DataBase_Name = RR4XML::WstringToInt(teilString.substr(start, end - start));

			std::pair < std::wstring, std::wstring > pairXML2DB = XML_Name2DataBase_Name_[indexXML_Name2DataBase_Name];
			oneCaptionComboBox += RR4XML::ExtractBefore(pairXML2DB.second,L" ");
			oneCaptionComboBox += L" ";

			//Test nun
			vIndex.push_back(	RR4XML::WstringToInt(teilString.substr(start, end - start))	);

			start = end + 1;
			end = teilString.find(L'|', start);
		}
		std::wstring tempString = teilString.substr(start);

		if( (tempString != L"") && (tempString != L"|") && (tempString != L" ") ){
			vIndex.push_back(	RR4XML::WstringToInt(teilString.substr(start)) );
		}
		ComboBoxVorauswahl->Items->Add(RRTools::WStrToAnsiString(oneCaptionComboBox));

	}

	//Für den letzten Eintrag
	indexXML_Name2DataBase_Name = RR4XML::WstringToInt(teilString.substr(start));

	if(indexXML_Name2DataBase_Name > -1)
	{
		vIndex.push_back(	RR4XML::WstringToInt(teilString.substr(start)) );

		std::pair < std::wstring, std::wstring > pairXML2DB = XML_Name2DataBase_Name_[indexXML_Name2DataBase_Name];
		oneCaptionComboBox += pairXML2DB.second;
	}
	ComboBoxVorauswahl->ItemIndex = 0;
}

//Zur Zeit bei MouseEnter von UserGrid genutzt
void __fastcall TFormXML::GenerateVorausWahl()
{
	if(ComboBoxVorauswahl->ItemIndex == 0)
		return;

	std::size_t start = 0;
	std::size_t end;

	std::wstring teilString;
	int indexXML_Name2DataBase_Name;

	teilString = indexBereiche[ComboBoxVorauswahl->ItemIndex-1]; //-1 da 0 mit "Keine" vorbsetzt ist  würde der User dann mindestens 1 wählen

	start = 0;
	end = teilString.find(L'|');

	while (end != std::wstring::npos)
	{
		indexXML_Name2DataBase_Name = RR4XML::WstringToInt(teilString.substr(start, end - start));

		std::pair < std::wstring, std::wstring > pairXML2DB = XML_Name2DataBase_Name_[indexXML_Name2DataBase_Name];
			//-------------------
			std::pair<std::wstring, int> data;
			data.first = pairXML2DB.first; data.second = 0;
			if(data.first != L"")
				fieldDefsColumnsVorauswahlAsStr.push_back(data);
			//-------------------
		start = end + 1;
		end = teilString.find(L'|', start);
	}

	//Für den letzten Eintrag
	indexXML_Name2DataBase_Name = RR4XML::WstringToInt(teilString.substr(start));
	if(indexXML_Name2DataBase_Name > -1)
	{
		std::pair < std::wstring, std::wstring > pairXML2DB = XML_Name2DataBase_Name_[indexXML_Name2DataBase_Name];
		std::pair<std::wstring, int> data;
		data.first = pairXML2DB.first; data.second = 0;
		if(data.first != L"")
			fieldDefsColumnsVorauswahlAsStr.push_back(data);
	}
}

void __fastcall TFormXML::ComboBoxVorAuswahlOnClick(TObject *Sender)
{
	//Immer vorher erneut bereinigen
	IniCard_Item();
	DBGrid4XMLCard_Item->Invalidate();

	//So auto auf Combo-Box-Click
	if(ComboBoxVorauswahl->ItemIndex > 0)
	{
		bnUebertrage->Enabled = true;

		cbnLoescheSpalte->Enabled = false;
		GenerateVorausWahl();
		DBGrid4XMLCard_Item->Invalidate();

		ersteSpalte = false;
		cbnLöscheSpalten = false;
		fromDBGrid4XMLCard_Item = false;

		NewColumnNameVorAusWahl();
		UpdateDBGridUserChoiceVorAusWahl();
	}
	else
		bnUebertrage->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::TFormXMLOnPaint(TObject *Sender)
{
	if(!showWindow)
	{
		DBGrid4XMLin->Enabled = false;
		DBGrid4XMLCard_Item->Enabled = false;
		Memo1->Enabled = false;
		DBGridUserChoice->Enabled = false;
		cbnLoescheAlles->Enabled = false;
		cbnLoescheSpalte->Enabled = false;
		bnUebertrage->Enabled = false;
		ComboBoxDBGridUserChoice->Enabled = false;
		ComboBoxVorauswahl->Enabled = false;
		FormXML->Color = clRed;
		FormXML->Caption = "Keine XML-Datei gewählt. Fenster bitte schliessen";
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormXML::DBGrid4XMLCard_ItemOnMouseEnter(TObject *Sender)
{
	//nötig für Drag und Drop Simulation: Sonderfall nur 1 Eintrag
	if( DBGrid4XMLCard_Item->DataSource->DataSet->IsEmpty() )
	{
		return;
	}
}
//---------------------------------------------------------------------------
