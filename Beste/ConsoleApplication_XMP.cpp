#pragma hdrstop
//#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
typedef char _TCHAR;
#define _tmain main
#endif

#include "rr_system.h"
#pragma comment(lib, "msxml6.lib")


//sei gesucht Werte wo:
// Wert von <bstrName> == C1 (f)
//Wert von <Date> == 02 / 26 / 2019 10:57 : 29
//main_bevorobigesKriterium

//Stand 29.03.2024 22:23 Uhr

//int _tmain(int argc, _TCHAR* argv[]) embarcadero
int main()
{
	XMLData* xmld = NULL;
	xmld = new XMLData();

	VARIANT_BOOL status;
	status = xmld->LoadXMLDocument(L"Beispiel_10KV.xml");
	if (status == VARIANT_TRUE)
	{
		std::wstring ereignis = L"";
		std::wstring datum = L"";


		std::cout << "Bitte Ereignis eingeben:";
		//std::wcin >> ereignis;
		std::getline(std::wcin, ereignis);
		std::cout << "Bitte Datum eingeben";
		std::getline(std::wcin, datum);

		std::cout << "@@@@@ CollectCards_Items @@@@@";
		int iferror1 = xmld->CollectCards_Items();
		std::cout << "@@@@@ AnalyzeDOM_PicEventUwish @@@@@";
		int iferror2 = xmld->AnalyzeDOM_PicEventUwish(ereignis,datum, 0);
		if ((iferror1 > 0) || (iferror2 > 0)) {
				
			std::cout << "Fehler bei der Analyse. Programm beendet sich";
			std::cin.get();
			if (xmld)
				delete xmld;
			return -1;
		}
		xmld->WriteInfos();
	}

	std::cout << "Anzahl : Verschiedener Messwerte : " << xmld->countMaxDifferM << std::endl;

	//Damit Konsole bleibt
	std::cout << "Drücken Sie eine beliebige Taste, um fortzufahren...";
	std::cin.get();
	//Damit Konsole bleibt

	delete xmld;
	return 0;
}

/*
Ideen:
1. Analyse bis erte Tabellen-Situation bleibt
-> CollectCards_Items(); Intern: addNode(pChildNodeOfCards); 
2.
->AnalyzeDOM_PicEventUwish(std::wstring ereignis, std::wstring datum, int option = 0); Intern: IXMLDOMNode* pChildNodeOfCards = NULL;	pChildNodeOfCards = GetNode((size_t)IndexOfVector);
->FindTheMeasurementData hier kommt die Option zum Tragen Intern: xmlInfo.push_back(info);

3. In Verbindung mit Counter verschiedene Messdaten-Arten wird eine Liste Zeilen weise gefüllt 
-> dann mit :WriteInfos() bzw... die aufgezeigte Möglichkeit Werte Abgreifbar

Grafische Situation:

Datei wird eingelesen.

und Automatisch die möglichen Ereignisse [Intern Cards_Item mit Bezeichner und Datum] in der Tabelle dargestellt.

User klickt eins an
Hier muß sich die Software den richtigen nehmen
Hier fehlt der Zusammnhang nodes zu klick
Entweder bereits alte Funktion nutzen und dann vom Grid den wert geben lassen
und in AnalyzeDOM_PicEventUwish hinein oder

Strucktur gößer machen (aber möchte ich eigentlic nicht)

Dann wenn aber gegebne in weitren Tabelle
welche Werte will ich

dann von dieser Aus den "Einkaufszettel fertig machen"
mit Hilfe von Drag und Drop

Und dort dann mit dem übrig gebliebenen
ein Grid aufbauen, welches dann per Freigabe sogar eventuell Daten zur DB senden kann.





*/