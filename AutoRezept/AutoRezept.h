#ifndef AutoRezept_H
#define AutoRezept_H

#include <list>

#include "HSP_Trench.h"
#include "Penta.h"

//Der Vorgang:
//1: System einmalig die möglichen Entscheidungspfade und Bedingungen geben
	//Intern erstellt sich AutoRezept einige Zeichenketten.
	//Eine davon dient der Beschreibung der Angaben wo bei von MDE kommend oder anderen Programmen Daten bereit gestellt werden
	//zum anderen eine lange Zeichenkette welche Pro Penta-Knoten mit einem % - Zeichen beginnt
	//Kontroll-Werte wie Anzahl der Penta-Knoten und anderes
	//Je nach Bedarf der permanenten Speicherung könnten auch bei Bedarf weitere generiert werden.
//2:
	//Syntax für den Parser. Ist Intern und braucht kein User zu wissen :)
	/*Also hier die Syntax
	% Penta-Knoten beginnt;
	! ID einer Bedingungsgröße; In unserem Beispiel: GH Volumen gesamt sei 42 und Kessel 33/34 sei 142
	? Bedingung beginnt; In unserem Beispiel: GH Volumen <= 200         GH Volumen 201-700 oder K33/34          GH Volumen > 700
	* Beschreiber um über den Switch die richtige Funktion zu nutzen. KG also <=  usw...
    # tatsächliche mathematisch verwertbare Zahl. Wird in einer Funktion in ein Float gewandelt zwischen gespeichert und im Switch je nach * genutzt
	§ Wenn Bedingung erfüllt. Hier vorallem Absprache mit Achim und Alex von Nöten
    | Trenner zwischen den einzelnen Bedingungen eines Penta-Knoten

	Weitere Details wie das Parsen genau funktioniert in einer gesonderten Besprechung. Wichtig ist das es es stets funktionieren wird.
    Testphase bei mehrschachtelungen und dann verwendung von ORC und ANC Hier gibt es weitere Zeichen aber sind für den jetzigen Stand der Forderung nicht nötig. Später dazu mehr.

	Beispiel Zeichenkette eines einzelnen Penta-Knoten: Leerstellen nur für die Veranschaulichung
	%10420	!42!142 ? !42 *KG #200 §VP888            |? !42 *VON #200 *BIS #700 *OR !142 *TRUE $VP890           |!42 *GA #700 §VP924 %
	*/

//3: Verarbeitung der beim geparrsten heraus extrahierten Informationen

//4: User Ergebnis bzw. Rückmeldung per GUI oder/und Datenbank sowie oder/und MDE geben.


//Die langen Zeichenketten könnte in einer separaten Tabelle gut Verwaltete werden und brauchen pro neue Initialisierung nur einen
//Record. über den X_TYP AURT (gibt es noch nicht) könnten auch spezielle Daten gespeichert werden in Protokoll. Zählerstände zum Beispiel
class AutoRezept : public  HSP_Trench {
public:
    AutoRezept() {}
	virtual ~AutoRezept() {}

	//Setzen der ID's jede wird inkrementiert

	//Extrahieren der beschreibenden Zeichenkette eines jeweiligen Penta-Knoten

	//Auffinden Zuordnen von Zahlen/String-Werten in der Zeichenkette
	//und setzen der Parameter-Reihenfolge für die KG() GA() GG() KA() OR() AN() ORC() ANC() FALSE() TRUE()

	//Wenn # vor dem Zeichen in Float umwandeln

	//switch entsprechend geparsten Bedingung mit KG GA GG KA OR AN ORC ANC FALSE TRUE

	//Die Möglichen verschiedenen Funktionen welche auftreten können.
	//int KG(){...}; sowie
	//GA GG KA OR AN ORC ANC FALSE TRUE

	//Hilfsfunktionen im Falle Kombi von Teil-Termen in der Logik
    //vorallem bei ANC und ORC


private:
	std::list<Penta> pentaList;

	//Beschreibung siehe Klasse Penta
	std::pair<int, int> eineVon_N_MöglichenBedingungenGrößen;
	std::pair<std::wstring, int> artDerBedingungenGröße;

	std::pair<std::wstring, int> eineVon_N_MöglichenBedingungen;
	std::pair<int, int> wennErfüllt;
	std::pair<int, int> wennNichtErfüllt;

	std::pair<int, int> posZiel_Rücksprung;
	std::pair<int, int> posStart_Rücksprung;

	std::pair<int, int> AdresseFürMöglichenRückSprung;
	std::pair<std::wstring, int> binRezept;
    
};



















#endif

/*
std::list<Penta> pentaList;

    // Penta-Objekte zur Liste hinzufügen
    pentaList.push_back(Penta()); // Fügt ein Penta-Objekt am Ende hinzu
    pentaList.push_back(Penta());
    pentaList.push_back(Penta());

    // Ein Penta-Objekt aus der Liste entfernen (z.B. das erste)
    std::list<Penta>::iterator it = pentaList.begin(); // Iterator auf das erste Element
    pentaList.erase(it);

    // Ein Penta-Objekt mittendrin in die Liste einfügen (nach dem zweiten Element)
    it = pentaList.begin();
    std::advance(it, 2); // Iterator auf das dritte Element setzen
    pentaList.insert(it, Penta()); // Fügt ein neues Penta-Objekt ein

    // Auf das erste und letzte Element der Liste zugreifen
    Penta& erstesElement = pentaList.front();
	Penta& letztesElement = pentaList.back();

	*/