#ifndef Penta_H
#define Penta_H

#include <map>
#include <string>
#include <vector>

//Ein Penta entspricht einer dreistöckigen Schicht:
//Penta ist ein Knotenpunkt eines nicht!!! üblichen binären Entscheidungssystem und der Gehe zu Fähigkeit
//Bedingungsgrößen (1 bis n-viele); Bedingungen (1 bis n-viele); Wenn Bedingung erfüllt nicht erfüllt (1 einzige je Bedingung );
class Penta {
public:
	Penta() {}
	~Penta() {}

//Getter/Setter
//der Vektoren
//Setzen würde ausschliesslich Auto_Rezept bei der GUI-Initialisierung EntscheidungsWege
//Das Lesen ist für das Finden in der Liste aller Penta's notwendig

private:

//Um alles Generisch zu halten gibt es für einen absoluten dynamischen Penta
//ID's Penta ist es egal wie etwas genannt wird . Es sortiert nach den Sorten
//welche Penta braucht: 1. Parameter immer der Wert, zweiter mögliche option
	std::vector<std::pair<int, int> > EineVon_N_MöglichenBedingungenGrößen;
		//ob einfacher Wert oder Adresse der Datenbank mit Tabelle und Zelle bzw. Spaltenbezeichner wo der Wert hinterlegt ist
		//Beispiele Summe Aller Aktivteile oder Kesselbelegung in Echtzeit. Zahlenwerte werden aus String konvertiert wenn es der entsprechende Typ ist
		std::vector<std::pair<std::wstring, int> > ArtDerBedingungenGröße;

	std::vector<std::pair<std::wstring, int> > EineVon_N_MöglichenBedingungen;
		std::vector<std::pair<int, int> > WennErfüllt;
		std::vector<std::pair<int, int> > WennNichtErfüllt;

	//Neue Position im Entscheiedungsweg falls nötig

	std::vector<std::pair<int, int> > PosZiel_Rücksprung;
		//Aus Kontrollgründen
		std::vector<std::pair<int, int> > PosStart_Rücksprung;

	//Hier bekommt jeder Entscheidungsverlauf-Pfeil eine zusätzliche Eigenschaft: Seine Adresse
	std::vector<std::pair<int, int> > AdresseFürMöglichenRückSprung;

	//Wenn gewünscht ist hier das Rezept, dies kann aber falls weitere Prozeßschritte plötzlich die Entscheidung noch
	//benötigt jederzeit änderbar
	std::vector<std::pair<std::wstring, int> > BinRezept;

};

#endif



