N6259507

1978425 

Guter Test:
1443402

TEST:
00243201

002568

Neues Protokoll OK


Nochmal genauen Verlauf
von wo an Probleme beim benutzen
Übertägt nicht


//-------------------------
Im Code die Befehlsabfolge


Alle 3 auch Auto OK von FormXML aus
Position 0: Startphase

0;1: Oben
2:
	


void __fastcall TFrameX10K::table1OnBeforeSelect(TObject *Sender, int ACol, int ARow,
		  bool &Accept)
{
	return;
	if(!ReadOnly)
		table1->Columns->Item[ACol]->Options << coEditing;

}
	
	
if Closed()...
beim Übertragen

	if(!ClientDataSet4->Active)
		ClientDataSet4->Open();	
		
		
//Test 1 bei dem kleonene Xml

TestUtest
Uist
Ix

alle 42,01   ... ,02 ... ,03 machen
danach als gegenwert:

Frequenz so ändern

ZUr Zeit 12 Einträge in Tab1

Tab2: Keiner mehr

//--------------------
Verändert dazu:

if(tableNr == 2)
	{
		for (int i = 0; i < table1->Columns->Count ; ++i)
		{
			//Hier wenn der Name des feldes lautet:  markedAsDestroy oder ID nicht sichtbar machen
			Field = table1->Columns->Item[i]->Field;
			//Feldname wie im DataSet !!!
			unistr = Field->FieldName;

//if( (unistr == U"Test_Art")||(unistr == U"markedASdestroyed")||(unistr == U"TempU")||(unistr == U"TempDfg")||(unistr == U"Feuchte")||(unistr == U"Kommentar") )
//				{	table1->Columns->Item[i]->Visible = false;	}

			if(!ReadOnly)
			{
				table1->Columns->Item[i]->Options << coEditing;   //RRR42 schauen ob wieder rein
				table1->Columns->Item[i]->Enabled = true;
			}

		}
	}
	
//-------------------------
Thema: Editieren
//-------------------------

//Bei XML
//Bemerkung: ERst schwarz wenn ja mit Esitiertaste bestätigt
Das Problem wenn dann einmal editiert nichts mehr machen	
	
//Also weitere Verwaltung nötig

//Was denn bei normal editieren

//Erstmal bein neuen starten


//Bei Normal

//Nein

record ist falsch nähmlich
auf oberste wie es der SQL-Manager in der Reihenfolge
darstellt.

Select ist nicht verantwortlich
Kein Button sondern Nachricht des Editierens

table1OnAfterEdit
Nun modifiziert

Fehler war statt ID Protokoll-Nummer zu wählen im SQL->Text






//-------------------------
Thema: löschen 
//-------------------------

//Bei XML





//Bei Normal
Test 518
OK




//TEST-Bilanz
Editieren von Normal OK
--> Bemerkung es wird nicht Select sondern die Row Von der Nachricht gewählt

Delete funktioniert OK 
table1OnCellClick
gesteuert über ColTable1, RowTable1


//Nun: XML
---> Bemerkung HandleIfDelete( ); hier entscheidend je table

Einmal Delete mit 4 TEST 503 und 504 löschen wir ==> Mit 4 OK
Einmal mit 14 TEST OK 
DELETE OK

Editieren:
nimmt keine Komma-Zahlen mehr
//Später 


















//===========================================================
//Nochmal nachdenken
//===========================================================

void __fastcall TFrameX10K::table1RecZurDBOnClick(TObject *Sender)
{
	//Beim test erstmal raus sollte besser im großen Projekt getesttet werden
	if(!CheckAllFieldsSet())
		return;

	if(xml4table1GUI)
	{
		CompleteXMLAndSend(1);
		if(table1->SelectedRow >= 0)     //Problem selected und Clickrow 2 paar Schuhe !!!
		{
			int index = table1->SelectedRow;
			std::pair<int, int> pairSelected = table1RememberOfXML[index];
			pairSelected.first = 0;
			pairSelected.second = 0;
			table1RememberOfXML[index] = pairSelected;
			table1->RefreshRow(table1->SelectedRow);
			
			


Problem: double nicht in der Tabelle
			
OK bei Normal:			
Im Gegensatz dazu:
	String uniValue = L"";
	std::wstring wuniValue = L"";

	header = table1->Columns->Item[ACol]->Header;
	headerCaption = L"";
	headerCaption = header->Caption;

	cellIDValue = table1->GetCellValue(0, ARow);

	uniValue = table1->Columns->Item[ACol]->Field->AsString;
	double dconvert = StrToFloat(uniValue);			
	
	QC1->FieldByName(headerCaption)->AsFloat = dconvert;
	
//Dazu 1786 bis 1788 geändert	





//Problem:
Verwalten:
Wenn editiert nun doch löschen 


Mehrere editieren auch wenn schon in die DB gesendet
Also Spaltenwert in Rec ändern 
zur DB
Also Spaltenwert in Rec ändern
erneut wird zur Zeit nicht unterstützt 

Und kein nachträgliches löschen möglich

//---
TEST 1 WNR ein File vorallem wenn erneut
DB: OK
Explorer: OK	


//Die Lösung:

Ja, die vorgeschlagenen Ansätze funktionieren auch mit C++03. Hier sind die entsprechenden Versionen:

Verwendung von std::stod() (C++03-kompatibel): Die std::stod()-Funktion ist in C++03 nicht verfügbar. Stattdessen kannst du die strtod()-Funktion aus der C-Bibliothek verwenden:
#include <iostream>
#include <cstdlib>

int main() {
    std::wstring wuniValue = L"123,456";
    wchar_t* endptr;
    double result = std::wcstod(wuniValue.c_str(), &endptr);
    if (endptr == wuniValue.c_str()) {
        std::wcerr << L"Fehler bei der Konvertierung." << std::endl;
    } else {
        std::wcout << L"Konvertiert zu double: " << result << std::endl;
    }
    return 0;
}
Beachte, dass std::wcstod() ebenfalls eine Ausnahme wirft, wenn die Konvertierung fehlschlägt.
Verwendung von wcstod() (für breite Zeichen, C++03): Wenn du speziell mit breiten Zeichen arbeiten möchtest, kannst du wcstod() verwenden, wie im obigen Beispiel gezeigt.
Dein ursprünglicher Ansatz (C++03): Deine ursprüngliche Funktion WstringToDouble sollte auch in C++03 funktionieren, solange die Kommazahlen im Format "123.456" (Punkt als Dezimaltrennzeichen) vorliegen. Wenn du jedoch Kommas als Dezimaltrennzeichen verwendest, musst du diese vor der Konvertierung durch Punkte ersetzen.
Ich hoffe, das hilft dir weiter! Falls du weitere Fragen hast, stehe ich gerne zur Verfügung. 😊

Sie
Gesendet von sie:
Geht Deine Funktion auch mit " std::wstring wuniValue = L"123.456";  " ?
Copilot
das web
wird verwendet
Gesendet von Copilot:
Ja, meine Funktion sollte auch mit "std::wstring wuniValue = L"123.456";" funktionieren. In diesem Fall wird die Zeichenkette bereits im richtigen Format vorliegen (mit einem Punkt als Dezimaltrennzeichen). Du kannst die ursprüngliche Funktion WstringToDouble verwenden oder alternativ die wcstod()-Funktion wie folgt:

#include <iostream>
#include <cwchar>

int main() {
    std::wstring wuniValue = L"123.456";
    wchar_t* endptr;
    double result = wcstod(wuniValue.c_str(), &endptr);
    if (endptr == wuniValue.c_str()) {
        std::wcerr << L"Fehler bei der Konvertierung." << std::endl;
    } else {
        std::wcout << L"Konvertiert zu double: " << result << std::endl;
    }
    return 0;
}	

