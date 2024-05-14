#ifndef AutoRezept_H
#define AutoRezept_H

#include <list>

#include "HSP_Trench.h"
#include "Penta.h"

//Der Vorgang:
//1: System einmalig die m�glichen Entscheidungspfade und Bedingungen geben
	//Intern erstellt sich AutoRezept einige Zeichenketten.
	//Eine davon dient der Beschreibung der Angaben wo bei von MDE kommend oder anderen Programmen Daten bereit gestellt werden
	//zum anderen eine lange Zeichenkette welche Pro Penta-Knoten mit einem % - Zeichen beginnt
	//Kontroll-Werte wie Anzahl der Penta-Knoten und anderes
	//Je nach Bedarf der permanenten Speicherung k�nnten auch bei Bedarf weitere generiert werden.
//2:
	//Syntax f�r den Parser. Ist Intern und braucht kein User zu wissen :)
	/*Also hier die Syntax
	% Penta-Knoten beginnt;
	! ID einer Bedingungsgr��e; In unserem Beispiel: GH Volumen gesamt sei 42 und Kessel 33/34 sei 142
	? Bedingung beginnt; In unserem Beispiel: GH Volumen <= 200         GH Volumen 201-700 oder K33/34          GH Volumen > 700
	* Beschreiber um �ber den Switch die richtige Funktion zu nutzen. KG also <=  usw...
    # tats�chliche mathematisch verwertbare Zahl. Wird in einer Funktion in ein Float gewandelt zwischen gespeichert und im Switch je nach * genutzt
	� Wenn Bedingung erf�llt. Hier vorallem Absprache mit Achim und Alex von N�ten
    | Trenner zwischen den einzelnen Bedingungen eines Penta-Knoten

	Weitere Details wie das Parsen genau funktioniert in einer gesonderten Besprechung. Wichtig ist das es es stets funktionieren wird.
    Testphase bei mehrschachtelungen und dann verwendung von ORC und ANC Hier gibt es weitere Zeichen aber sind f�r den jetzigen Stand der Forderung nicht n�tig. Sp�ter dazu mehr.

	Beispiel Zeichenkette eines einzelnen Penta-Knoten: Leerstellen nur f�r die Veranschaulichung
	%10420	!42!142 ? !42 *KG #200 �VP888            |? !42 *VON #200 *BIS #700 *OR !142 *TRUE $VP890           |!42 *GA #700 �VP924 %
	*/

//3: Verarbeitung der beim geparrsten heraus extrahierten Informationen

//4: User Ergebnis bzw. R�ckmeldung per GUI oder/und Datenbank sowie oder/und MDE geben.


//Die langen Zeichenketten k�nnte in einer separaten Tabelle gut Verwaltete werden und brauchen pro neue Initialisierung nur einen
//Record. �ber den X_TYP AURT (gibt es noch nicht) k�nnten auch spezielle Daten gespeichert werden in Protokoll. Z�hlerst�nde zum Beispiel
class AutoRezept : public  HSP_Trench {
public:
    AutoRezept() {}
	virtual ~AutoRezept() {}

	//Setzen der ID's jede wird inkrementiert

	//Extrahieren der beschreibenden Zeichenkette eines jeweiligen Penta-Knoten

	//Auffinden Zuordnen von Zahlen/String-Werten in der Zeichenkette
	//und setzen der Parameter-Reihenfolge f�r die KG() GA() GG() KA() OR() AN() ORC() ANC() FALSE() TRUE()

	//Wenn # vor dem Zeichen in Float umwandeln

	//switch entsprechend geparsten Bedingung mit KG GA GG KA OR AN ORC ANC FALSE TRUE

	//Die M�glichen verschiedenen Funktionen welche auftreten k�nnen.
	//int KG(){...}; sowie
	//GA GG KA OR AN ORC ANC FALSE TRUE

	//Hilfsfunktionen im Falle Kombi von Teil-Termen in der Logik
    //vorallem bei ANC und ORC


private:
	std::list<Penta> pentaList;

	//Beschreibung siehe Klasse Penta
	std::pair<int, int> eineVon_N_M�glichenBedingungenGr��en;
	std::pair<std::wstring, int> artDerBedingungenGr��e;

	std::pair<std::wstring, int> eineVon_N_M�glichenBedingungen;
	std::pair<int, int> wennErf�llt;
	std::pair<int, int> wennNichtErf�llt;

	std::pair<int, int> posZiel_R�cksprung;
	std::pair<int, int> posStart_R�cksprung;

	std::pair<int, int> AdresseF�rM�glichenR�ckSprung;
	std::pair<std::wstring, int> binRezept;
    
};



















#endif

/*
std::list<Penta> pentaList;

    // Penta-Objekte zur Liste hinzuf�gen
    pentaList.push_back(Penta()); // F�gt ein Penta-Objekt am Ende hinzu
    pentaList.push_back(Penta());
    pentaList.push_back(Penta());

    // Ein Penta-Objekt aus der Liste entfernen (z.B. das erste)
    std::list<Penta>::iterator it = pentaList.begin(); // Iterator auf das erste Element
    pentaList.erase(it);

    // Ein Penta-Objekt mittendrin in die Liste einf�gen (nach dem zweiten Element)
    it = pentaList.begin();
    std::advance(it, 2); // Iterator auf das dritte Element setzen
    pentaList.insert(it, Penta()); // F�gt ein neues Penta-Objekt ein

    // Auf das erste und letzte Element der Liste zugreifen
    Penta& erstesElement = pentaList.front();
	Penta& letztesElement = pentaList.back();

	*/