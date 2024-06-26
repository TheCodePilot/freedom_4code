#include "rr_system.h"
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UmgangmitTDBGridUndXML.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//#include <fmx.h>
//#pragma hdrstop

//#include "fmMain.h"
#include <memory>
//---------------------------------------------------------------------------
//#pragma package(smart_init)
//#pragma resource "*.fmx"
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)  //!!!Achtung Grid dmManual !!!
	: TForm(Owner)
{
	this->BorderStyle = bsDialog;

OpenTextFileDialog1 = new TOpenTextFileDialog(this);
	OpenTextFileDialog1->InitialDir = GetCurrentDir();
	if (OpenTextFileDialog1->Execute())
	{
		ShowMessage("Datei ausgewählt: " + OpenTextFileDialog1->FileName);
	}
	UnicodeString ustr = OpenTextFileDialog1->FileName;
	delete OpenTextFileDialog1;          //Ok wenn
	std::wstring wstr(ustr.w_str());

	DataSource1->DataSet = ClientDataSet1;
	DBGrid4XMLin->DataSource = DataSource1;
	DBGrid4XMLin->ReadOnly = true;
	// Felder im ClientDataSet definieren

	ClientDataSet1->FieldDefs->Clear();
	ClientDataSet1->FieldDefs->Add("Name", ftString, 50);
	ClientDataSet1->FieldDefs->Add("Datum/Uhrzeit", ftString, 255);

	ClientDataSet1->CreateDataSet();
	// Prozessieren des XML-Dokuments und Hinzufügen der Daten zum ClientDataSet
	XMLProcess* xmlProcess = new XMLProcess();
	xmlProcess->LoadXMLDocument(wstr);
	//hier noch Kompakter gestalten


	try
	{
		XML2Grid(xmlProcess);
	}
	catch (const Exception &e)
	{
		ShowMessage("Beim einlesen der XML-Daten. An Position" + e.Message);
		if(xmlProcess) {
			delete xmlProcess;
		}   xmlProcess = NULL;
	}
	if(xmlProcess){
		delete xmlProcess;
		xmlProcess = NULL;
	}
}
//---------------------------------------------------------------------------

//=============================================================================================================================================
void __fastcall TForm5::FillTDBGridFromXML(XMLProcess* xmlProcess)
//=============================================================================================================================================
{
		DBGrid4XMLin->DataSource->DataSet->Next();
		std::vector<std::pair<std::wstring, std::wstring> > data = xmlProcess->GetXMLInfo();
		/*if(*data==NULL)
		{   Noch was überlegen
			ShowMessage("(data)[0]";
		}*/
		for(size_t i = 0; i < data.size(); ++i)
		{
			const std::pair < std::wstring, std::wstring>& pair = (data)[i];
			ClientDataSet1->Append();
			ClientDataSet1->FieldByName("Name")->AsString = pair.first.c_str();
			ClientDataSet1->FieldByName("Datum/Uhrzeit")->AsString = pair.second.c_str();
			ClientDataSet1->Post();
		}
}

//=============================================================================================================================================
void __fastcall TForm5::XML2Grid(XMLProcess* xmlProcess)
//=============================================================================================================================================
{
    IXMLDOMElement* pRootNode;
	//xmlProcess->pXMLDom->get_documentElement(&pRootNode); //.pXMLDom->get_documentElement(&pRootNode);
	HRESULT hr = xmlProcess->pXMLDom->get_documentElement(&pRootNode);
	if (FAILED(hr) || !pRootNode) {
		throw std::runtime_error("Fehler beim Abrufen des Wurzelknotens.");
	}

	//Zentrale Routine hier auch Filterung
	xmlProcess->ProcessNodePlus(pRootNode);
	//Zentrale Routine hier auch Filterung

	if(pRootNode)
	{
		pRootNode->Release();
	}
	FillTDBGridFromXML(xmlProcess);

}


//3 Wichtig MouseDown, StartDrag, EndDrag  weil Quelle: dkDrag und dmAutomatic
void __fastcall TForm5::DBGrid4XMLinMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{

	TGridCoord coord = DBGrid4XMLin->MouseCoord(X,Y);
	if ((Button == mbLeft)&&(coord.X != 1))
	{
		TGridCoord coord = DBGrid4XMLin->MouseCoord(X, Y);
		if (coord.Y > 0) // Nicht auf den Spaltenüberschriften klicken
		{
			 //DBGrid4XMLin->DragMode = dmAutomatic;
				//AnsiString text = DBGrid4XMLin->Columns->Items[coord.X - 1]->Field->AsString;
			AnsiString text = DBGrid4XMLin->Columns->Items[DBGrid4XMLin->SelectedIndex]->Field->AsString;
			DBGrid4XMLin->BeginDrag(false);
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TForm5::DBGrid4XMLinEndDrag(TObject *Sender, TObject *Target, int X,
          int Y)
{
   return;
    TControl *control = dynamic_cast<TControl*>(Target);
    if (control != NULL)
	{
		AnsiString text = DBGrid4XMLin->Columns->Items[DBGrid4XMLin->SelectedIndex]->Field->AsString;
		if (control->ClassNameIs("TEdit"))
        {
			dynamic_cast<TEdit*>(control)->Text = text;
        }

	}

}
//---------------------------------------------------------------------------

void __fastcall TForm5::Edit1DragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
	//return;
	if(Source == DBGrid4XMLin)
	{
		AnsiString text = DBGrid4XMLin->Columns->Items[DBGrid4XMLin->SelectedIndex]->Field->AsString;
        dynamic_cast<TEdit*>(Sender)->Text = text;
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm5::DBGrid4XMLinStartDrag(TObject *Sender, TDragObject *&DragObject)   //Nun return
{
	return;


			//DBGrid4XMLin->DragMode = dmManual;
				//AnsiString text = DBGrid4XMLin->Columns->Items[coord.X - 1]->Field->AsString;
			AnsiString text = DBGrid4XMLin->Columns->Items[DBGrid4XMLin->SelectedIndex]->Field->AsString;
			DBGrid4XMLin->BeginDrag(false);


}
//---------------------------------------------------------------------------
//vermutlich erst wenn DragDrop gesetzt ist und DragKind dkDock DragMode dmManual
void __fastcall TForm5::Edit1DragOver(TObject *Sender, TObject *Source, int X, int Y,
          TDragState State, bool &Accept)
{

    Accept = (Source == DBGrid4XMLin);
}
//---------------------------------------------------------------------------

void __fastcall TForm5::DBGrid4XMLinOnCellClick(TColumn *Column)
{
		if(Column->Index !=1)
		{

			AnsiString text = DBGrid4XMLin->Columns->Items[DBGrid4XMLin->SelectedIndex]->Field->AsString;
			DBGrid4XMLin->BeginDrag(false);

		}


}
//---------------------------------------------------------------------------
void __fastcall TForm5::DBGrid4XMLinDblclick(TObject *Sender)
{
	;
}
//---------------------------------------------------------------------------

