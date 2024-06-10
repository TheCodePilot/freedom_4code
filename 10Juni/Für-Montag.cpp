Problem für Montag:


void __fastcall TFormXML::cbnLoescheSpalteOnClick(TObject *Sender)
{
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

	//SonderFall Button gedrückt obwohl nichts rot ist könnte auch in wenn erstemal was rot angeklickt dieswer Button noch gesperrt werden
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


//--------------------------

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


	/*
	// Definieren der FieldDefs
	for(size_t i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{
		std::pair<std::wstring,int> nunUnsichtbar = fieldDefsColumnsUserDBGridAsStr[i];
		if(fieldDefsColumnsUserDBGridAsStr[i].second == 0) //1 bedeutet ja vorbereitet also wird ja bei Button immer zu 2 oder 1 zu 0 durch zurück nehmen
		{
			std::pair<std::wstring,int> nunUnsichtbar(fieldDefsColumnsUserDBGridAsStr[i].first, fieldDefsColumnsUserDBGridAsStr[i].second);
			std::wstring wstr= nunUnsichtbar.first;
			ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(fieldDefsColumnsUserDBGridAsStr[i].first), ftString, 12);
		}
	} */

    // Definieren der FieldDefs
	for(size_t i = 0; i < fieldDefsColumnsUserDBGridAsStr.size(); ++i)
	{
		std::pair<std::wstring,int> nunUnsichtbar = fieldDefsColumnsUserDBGridAsStr[i];
		if(fieldDefsColumnsUserDBGridAsStr[i].second == 0) //1 bedeutet ja vorbereitet also wird ja bei Button immer zu 2 oder 1 zu 0 durch zurück nehmen
		{
			std::pair<std::wstring,int> nunUnsichtbar(fieldDefsColumnsUserDBGridAsStr[i].first, fieldDefsColumnsUserDBGridAsStr[i].second);
			std::wstring wstr= nunUnsichtbar.first;
			//RRR999
			//------------
//!!!!!!!!!			std::wstring dbHeader = ConvertXMLNameToDBHeader(fieldDefsColumnsUserDBGridAsStr[i].first);
			//std::wstring dbHeader = ConvertXMLNameToDBHeader(RRTools::AnsiToWide(clickMeasureCard_Item));
			//fieldDefsColumnsUserDBGridAsStr[i].first
//!!!!!!!!!			ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(dbHeader), ftString, 12);
			//-----------
				//Vorher
				//ClientDataSet3->FieldDefs->Add(RRTools::WStrToAnsiString(fieldDefsColumnsUserDBGridAsStr[i].first), ftString, 12);
		}
	}

	if(fieldCounterToDelete > 0) //1-basiert
		ClientDataSet3->CreateDataSet();

	ersteSpalte = false;
}

//Hinweis das Feld ist leer

