#include "rr_system.h"
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UmgangmitTDBGridUndXML.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm5 *Form5;
//---------------------------------------------------------------------------
__fastcall TForm5::TForm5(TComponent* Owner)
	: TForm(Owner)
{

	DataSource1->DataSet = ClientDataSet1;
	DBGrid4XMLin->DataSource = DataSource1;
	// Felder im ClientDataSet definieren

	ClientDataSet1->FieldDefs->Clear();
	ClientDataSet1->FieldDefs->Add("NodeName", ftString, 50);
	ClientDataSet1->FieldDefs->Add("NodeValue", ftString, 255);

	ClientDataSet1->CreateDataSet();
	// Prozessieren des XML-Dokuments und Hinzufügen der Daten zum ClientDataSet
	XMLProcess* xmlProcess = new XMLProcess();
	xmlProcess->LoadXMLDocument(L"M.xml");
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
		std::vector<std::pair<std::wstring, std::wstring> > data = xmlProcess->getXMLInfo();
		for(size_t i = 1; i < data.size()-1; ++i)
		{
			const std::pair < std::wstring, std::wstring>& pair = (data)[i];
			ClientDataSet1->Append();
			ClientDataSet1->FieldByName("NodeName")->AsString = pair.first.c_str();
			ClientDataSet1->FieldByName("NodeValue")->AsString = pair.second.c_str();
			ClientDataSet1->Post();
		}
}

//=============================================================================================================================================
void __fastcall TForm5::XML2Grid(XMLProcess* xmlProcess)
//=============================================================================================================================================
{
    IXMLDOMElement* pRootNode;
		xmlProcess->pXMLDom->get_documentElement(&pRootNode); //.pXMLDom->get_documentElement(&pRootNode);
		//Zentrale Routine hier auch Filterung
		xmlProcess->ProcessNodePlus(pRootNode);
		//Zentrale Routine hier auch Filterung

		if(pRootNode) {
			pRootNode->Release(); }
			FillTDBGridFromXML(xmlProcess);

}
