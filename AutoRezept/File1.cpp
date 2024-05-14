#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include "Root.h"
#include "BranchVonRootLinks.h"
#include "BranchVonRootRechts.h"
#include "ChildVonBranchLinks.h"
#include "ChildVonBranchRechts.h"
#include "AutoRezept.h"

#include <stdio.h>
#include <iostream>
#include <string>


//int _tmain(int argc, _TCHAR* argv[])

int main() {
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

    // Weitere Operationen mit der Liste durchführen...

    return 0;
}



//Ok
int main2() {
    // Erstellen der Objekte
    ChildVonBranchLinks leftChild;
    ChildVonBranchRechts rightChild;

    // Setzen der Zeiger
	leftChild.setRightChildPtr(&rightChild);
    rightChild.setLeftChildPtr(&leftChild);

    // Datenaustausch
    leftChild.sendDataToRight(42);
	std::cout << "Data in rightChild: " << rightChild.getData() << std::endl;

	rightChild.sendDataToLeft(24);
	std::cout << "Data in leftChild: " << leftChild.getData() << std::endl;

    return 0;
}

//Aber noch nicht mit Pointern um Vorwärtsdeklaration zu erreichen
////using namespace std;

/*
int main1()
{
	ChildVonBranchLinks leftChild;
	ChildVonBranchRechts rightChild;

	// Datenaustausch
	leftChild.sendDataToRight(rightChild, 42);
	cout << "Data in rightChild: " << rightChild.getData() << endl;

	rightChild.sendDataToLeft(leftChild, 24);
	cout << "Data in leftChild: " << leftChild.getData() << endl;

	return 0;
}
*/

/*
int main0()
{
	std::string name;
	std::cout << "Enter the name: ";

	// Get the input from std::cin and store into name
	std::getline(std::cin, name);
	std::cout << "Hello" << name << "!\n";
		std::getline(std::cin, name);

	//std:: wcout << std::endl;
	return 0;

}
*/
