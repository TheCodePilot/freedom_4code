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

/*
//====================================================
//V1
//====================================================

	
#include <iostream>
#include <vector>
#include <string>

// Funktionsdefinitionen
bool compareDoubles(double a, double b) {
    return a < b; // Beispiel: Gibt true zurück, wenn a kleiner als b ist
}

bool compareStrings(const std::string& a, const std::string& b) {
    return a.length() < b.length(); // Beispiel: Vergleich basierend auf Länge
}

// Wrapperfunktion
typedef bool (*CompareFunc)(void*, void*);

bool compareDoubleWrapper(void* a, void* b) {
    return compareDoubles(*static_cast<double*>(a), *static_cast<double*>(b));
}

bool compareStringWrapper(void* a, void* b) {
    return compareStrings(*static_cast<std::string*>(a), *static_cast<std::string*>(b));
}

// Hauptprogramm
int main() {
    // Vector mit Funktionszeigern
    std::vector<CompareFunc> comparators;

    // Hinzufügen der Wrapperfunktionen
    comparators.push_back(&compareDoubleWrapper);
    comparators.push_back(&compareStringWrapper);

    // Beispielwerte
    double x = 3.14, y = 2.71;
    std::string str1 = "Hello", str2 = "World";

    // Funktionsaufruf basierend auf Index
    int myIndex = 0; // Vergleiche Doubles
    bool result = comparators[myIndex](&x, &y);
    std::cout << "Vergleich Doubles: " << (result ? "true" : "false") << std::endl;

    myIndex = 1; // Vergleiche Strings
    result = comparators[myIndex](&str1, &str2);
    std::cout << "Vergleich Strings: " << (result ? "true" : "false") << std::endl;

    return 0;
}

//====================================================
//V2
//====================================================

#include <iostream>
#include <vector>
#include <cstdarg>
#include <string>

// Funktionsdefinitionen
bool compareDoubles(double a, double b, ...) {
    va_list args;
    va_start(args, b);
    double tolerance = va_arg(args, double); // Optional: Zusätzlicher Parameter für Toleranz
    va_end(args);
    return std::abs(a - b) < tolerance; // Vergleich mit Toleranz
}

bool compareStrings(const std::string& a, const std::string& b, ...) {
    va_list args;
    va_start(args, b);
    size_t minLength = va_arg(args, size_t); // Optional: Mindestlänge für Vergleich
    va_end(args);
    return a.length() >= minLength && b.length() >= minLength; // Vergleich basierend auf Länge
}

// Wrapperfunktion
typedef bool (*CompareFunc)(void*, ...);

// Hauptprogramm
int main() {
    // Vector mit Funktionszeigern
    std::vector<CompareFunc> comparators;

    // Hinzufügen der Funktionen
    comparators.push_back(reinterpret_cast<CompareFunc>(&compareDoubles));
    comparators.push_back(reinterpret_cast<CompareFunc>(&compareStrings));

    // Beispielwerte
    double x = 3.14, y = 3.15, tolerance = 0.1;
    std::string str1 = "Hello", str2 = "World";
    size_t minLength = 3;

    // Funktionsaufruf basierend auf Index
    int myIndex = 0; // Vergleiche Doubles
    bool result = comparators[myIndex](&x, &y, tolerance);
    std::cout << "Vergleich Doubles: " << (result ? "true" : "false") << std::endl;

    myIndex = 1; // Vergleiche Strings
    result = comparators[myIndex](&str1, &str2, minLength);
    std::cout << "Vergleich Strings: " << (result ? "true" : "false") << std::endl;

    return 0;
}

//====================================================
//V3
//====================================================

#include <iostream>
#include <vector>
#include <string>

// Funktionsdefinitionen
bool compareDoubles(double a, double b, double p1, double p2, double p3, double p4, 
                    double p5, double p6, double p7, double p8) {
    // Beispiel: Vergleich mit zusätzlichem Toleranzparameter (p1)
    return std::abs(a - b) < p1;
}

bool compareStrings(const std::string& a, const std::string& b, int p1, int p2, int p3, 
                    int p4, int p5, int p6, int p7, int p8) {
    // Beispiel: Vergleich der Längen mit einer Mindestlänge (p1)
    return a.length() >= p1 && b.length() >= p1;
}

// Funktionszeigertyp
typedef bool (*CompareFunc)(void*, void*, double, double, double, double, double, double, double, double);

// Wrapper für Doubles
bool compareDoubleWrapper(void* a, void* b, double p1, double p2, double p3, double p4, 
                          double p5, double p6, double p7, double p8) {
    return compareDoubles(*static_cast<double*>(a), *static_cast<double*>(b), p1, p2, p3, p4, p5, p6, p7, p8);
}

// Wrapper für Strings
bool compareStringWrapper(void* a, void* b, double p1, double p2, double p3, double p4, 
                          double p5, double p6, double p7, double p8) {
    return compareStrings(*static_cast<std::string*>(a), *static_cast<std::string*>(b), 
                          static_cast<int>(p1), static_cast<int>(p2), static_cast<int>(p3), 
                          static_cast<int>(p4), static_cast<int>(p5), static_cast<int>(p6), 
                          static_cast<int>(p7), static_cast<int>(p8));
}

// Hauptprogramm
int main() {
    // Vector mit Funktionszeigern
    std::vector<CompareFunc> comparators;

    // Hinzufügen der Wrapperfunktionen
    comparators.push_back(&compareDoubleWrapper);
    comparators.push_back(&compareStringWrapper);

    // Beispielwerte
    double x = 3.14, y = 3.15, tolerance = 0.1; // Toleranz als zusätzlicher Parameter
    std::string str1 = "Hello", str2 = "World";
    int minLength = 3; // Mindestlänge als zusätzlicher Parameter

    // Funktionsaufruf basierend auf Index
    int myIndex = 0; // Vergleiche Doubles
    bool result = comparators[myIndex](&x, &y, tolerance, 0, 0, 0, 0, 0, 0, 0, 0);
    std::cout << "Vergleich Doubles: " << (result ? "true" : "false") << std::endl;

    myIndex = 1; // Vergleiche Strings
    result = comparators[myIndex](&str1, &str2, minLength, 0, 0, 0, 0, 0, 0, 0, 0);
    std::cout << "Vergleich Strings: " << (result ? "true" : "false") << std::endl;

    return 0;
}



