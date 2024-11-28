#ifndef RRTOOLS_H
#define RRTOOLS_H

#include <iostream>  // Für std::wcout
#include <map>  // Für std::map
#include <string>  // Für std::wstring
#include <comdef.h>  // Für _bstr_t
#include <msxml6.h>  // Für IXMLDOMNode
#pragma comment(lib, "msxml6.lib")

namespace RRTools {

    const int WCEQUALOP = 0;
    const int WCSCMP = 1;

    static void TestVergleiche(IXMLDOMNode* pNode1, IXMLDOMNode* pNode2, int option = WCEQUALOP) {

        BSTR nodeName1, nodeName2;
        pNode1->get_nodeName(&nodeName1);
        pNode2->get_nodeName(&nodeName2);

        if (option == WCEQUALOP)
        {
            std::wcout << "Mit dem == Operator von std::wstring : " << L"Vergleiche Knoten: " << nodeName1 << L" und " << nodeName2 << std::endl;

            std::wstring wnodeName1(nodeName1);
            std::wstring wnodeName2(nodeName2);


            if (wnodeName1 == wnodeName2) {
                std::wcout << L"Die Knoten sind gleich." << std::endl;
            }
            else {
                std::wcout << L"Die Knoten sind nicht gleich." << std::endl;
            }

        }
        else if (option == WCSCMP)
        {
            std::wcout << "Mit std::wcscmp(nodeName1, nodeName2) : " << L"Vergleiche Knoten: " << nodeName1 << L" und " << nodeName2 << std::endl;

            if (wcscmp(nodeName1, nodeName2) == 0) {
                std::wcout << L"Die Knoten sind gleich." << std::endl;
            }
            else {
                std::wcout << L"Die Knoten sind nicht gleich." << std::endl;
            }
        }
        else
        {
            std::cout << "Parameterfehler bei RRTools::TestFirst2Nots. Bitte als zweiten Parameter die Option :  WCEQUALOP = 0 oder WCSCMP = 1 waehlen." << std::endl;
        }

        SysFreeString(nodeName1);
        SysFreeString(nodeName2);
    }

    static bool TestFirst2Nots(IXMLDOMNode* pRootNode, int option = WCEQUALOP)
    {
        bool returner = false;
        if (pRootNode) {

            IXMLDOMNode* pNode1 = NULL;
            IXMLDOMNode* pNode2 = NULL;

            // Erster und zweiter Knoten des Wurzelknotens holen
            pRootNode->get_firstChild(&pNode1);
            if (pNode1) {
                pNode1->get_nextSibling(&pNode2);
                if (pNode2) {

                    RRTools::TestVergleiche(pNode1, pNode2, option);
                }
                pNode1->Release();
            }
            if (pNode2) {
                pNode2->Release();
                returner = true;
            }
        }
        
        return returner;
        
    }

    static void printNode(IXMLDOMNode* pNode, int depth = 0) {
        BSTR nodeName, nodeValue;
        pNode->get_nodeName(&nodeName);
        pNode->get_text(&nodeValue);
        if (nodeName && wcscmp(nodeName, L"#text") == 0) {
            IXMLDOMNode* pParentNode;
            pNode->get_parentNode(&pParentNode);
            if (pParentNode) {
                BSTR parentName;
                pParentNode->get_nodeName(&parentName);
                for (int i = 0; i < depth; ++i) std::wcout << L"  ";
                std::wcout << L"Der Knoten hat den Namen \"" << parentName << L"\" und den Wert \"" << nodeValue << L"\"." << std::endl;
                SysFreeString(parentName);
                pParentNode->Release();
            }
        }
        SysFreeString(nodeName);
        SysFreeString(nodeValue);

        IXMLDOMNode* pChildNode;
        pNode->get_firstChild(&pChildNode);
        while (pChildNode) {
            printNode(pChildNode, depth + 1);
            IXMLDOMNode* pNextNode;
            pChildNode->get_nextSibling(&pNextNode);
            pChildNode->Release();
            pChildNode = pNextNode;
        }
    }


    static std::pair<std::wstring, bool> getNodeInfo(IXMLDOMNode* pNode) {
        std::wstring nodeType;
        bool hasChild = false;

        if (pNode == NULL) {
            return { nodeType, hasChild };
        }

        // Extrahieren des Knotentyps
        DOMNodeType type;
        pNode->get_nodeType(&type);
        switch (type) {
        case NODE_ELEMENT:
            nodeType = L"Element"; // Beispiel: <Tag>...</Tag>
            break;
        case NODE_ATTRIBUTE:
            nodeType = L"Attribut"; // Beispiel: name="value" in <Tag name="value">
            break;
        case NODE_TEXT:
            nodeType = L"Text"; // Beispiel: Text innerhalb eines Tags wie <Tag>Text</Tag>
            break;
        case NODE_COMMENT:
            nodeType = L"Kommentar"; // Beispiel: <!-- Kommentar -->
            break;
        case NODE_DOCUMENT:
            nodeType = L"Dokument"; // Beispiel: Das gesamte XML-Dokument
            break;
        case NODE_DOCUMENT_TYPE:
            nodeType = L"Dokumenttyp"; // Beispiel: <!DOCTYPE ...>
            break;
        case NODE_DOCUMENT_FRAGMENT:
            nodeType = L"Dokumentfragment"; // Beispiel: Eine Gruppe von Knoten, die keinem Wurzelknoten zugeordnet sind
            break;
        default:
            nodeType = L"Unbekannter Typ";
            break;
        }

        // Überprüfen, ob der Knoten Kinder hat
        IXMLDOMNodeList* pChildren;
        pNode->get_childNodes(&pChildren);
        long length;
        pChildren->get_length(&length);
        hasChild = (length > 0);

        return { nodeType, hasChild };
    }

    static void processNode(IXMLDOMNode* pNode) {
        if (pNode == NULL) {
            return;
        }

        auto nodeInfo = RRTools::getNodeInfo(pNode);
        std::wcout << L"Knotentyp: " << nodeInfo.first << L", Hat Kinder: " << (nodeInfo.second ? L"Ja" : L"Nein") << std::endl;

        IXMLDOMNodeList* pChildren;
        pNode->get_childNodes(&pChildren);
        long length;
        pChildren->get_length(&length);

        for (long i = 0; i < length; i++) {
            IXMLDOMNode* pChild;
            pChildren->get_item(i, &pChild);
            RRTools::processNode(pChild);
        }
    }

    static void processNodePlus(IXMLDOMNode* pNode) {
        if (pNode == NULL) {
            return;
        }

        auto nodeInfo = RRTools::getNodeInfo(pNode);

        // Extrahieren des Knotennamens und des Knotenwerts
        BSTR nodeName, nodeValue;
        pNode->get_nodeName(&nodeName);
        pNode->get_text(&nodeValue);

        std::wcout << L"Knotentyp: " << nodeInfo.first << L", Hat Kinder: " << (nodeInfo.second ? L"Ja" : L"Nein");
        std::wcout << L", Name: " << nodeName << L", Wert: " << nodeValue << std::endl;

        // Freigeben der Speicherressourcen
        SysFreeString(nodeName);
        SysFreeString(nodeValue);

        IXMLDOMNodeList* pChildren;
        pNode->get_childNodes(&pChildren);
        long length;
        pChildren->get_length(&length);

        for (long i = 0; i < length; i++) {
            IXMLDOMNode* pChild;
            pChildren->get_item(i, &pChild);
            RRTools::processNodePlus(pChild);
        }
    }

    static void getNodeInfoByTagName(IXMLDOMNode* pNode, const std::wstring& tagName) {
        BSTR nodeName;
        pNode->get_nodeName(&nodeName);
        std::wstring wnodeName(nodeName);
        SysFreeString(nodeName);

        if (wnodeName == tagName) {
            // Die Bezeichnung des aktuellen Knotens entspricht der Ziel-Zeichenkette
            // Führen Sie hier die gewünschten Aktionen aus, z. B. den aktuellen Knoten verarbeiten
            // oder speichern
            RRTools::processNodePlus(pNode); //achtung sonst hier
        }

        // Rekursiv die Kinderknoten durchlaufen
        IXMLDOMNode* pChildNode;
        pNode->get_firstChild(&pChildNode);
        while (pChildNode) {
            RRTools::getNodeInfoByTagName(pChildNode, tagName);
            IXMLDOMNode* pNextNode;
            pChildNode->get_nextSibling(&pNextNode);
            pChildNode->Release();
            pChildNode = pNextNode;
        }
    }

    //In Entwicklung
    void processNodesBetweenMarkers(IXMLDOMNode* pRootNode, const std::wstring& startMarker, int startOccurrence, const std::wstring& endMarker, int endOccurrence) {
        std::map<std::wstring, int> markerCounts4Start;
        std::map<std::wstring, int> markerCounts4End;

        bool insideInterval = false;
        int abteilungCount = 0;
        int emailCount = 0;
        int zwei = 2;

        IXMLDOMNode* pNode = nullptr;
        pRootNode->get_firstChild(&pNode);
        while (pNode != nullptr) {
            std::cout << "EINS" << std::endl;
            BSTR nodeName;
            pNode->get_nodeName(&nodeName);
            std::wstring wnodeName(nodeName);
            //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            auto nodeInfo = RRTools::getNodeInfo(pNode);
            std::wcout << L"Knotentyp: " << nodeInfo.first << L", Hat Kinder: " << (nodeInfo.second ? L"Ja" : L"Nein") << std::endl;
            //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            if (wnodeName == startMarker) {
                std::cout << "ZWEI" << std::endl;
                markerCounts4Start[wnodeName]++;
                abteilungCount++;
                std::cout << "IntervallStartCount ist nun" << abteilungCount << std::endl;

                if (markerCounts4Start[wnodeName] == startOccurrence) {
                    std::cout << "DREI" << std::endl;
                    if (!insideInterval) {  //nun hier schauen
                        std::cout << "VIER" << std::endl;
                        if (abteilungCount == zwei) {   //hier geht er nicht rein
                            std::cout << "FÜNF" << std::endl;
                            insideInterval = true;
                        }
                    }
                }
            }

            /*
            if (insideInterval == true) {
                processNode(pNode);
                std::cout << "SECHS" << std::endl;
            }
            */

            if (insideInterval == true) {
                //if (wnodeName == L"Email") {
                //    processNode(pNode);
                                                            //processEmailNodes(pNode);
                std::cout << "SECHS" << std::endl;
                //}
            }

            // Rest des Codes bleibt unverändert


            //std::wstring wnodeName(nodeName);
            std::wcout << L"markerCounts4End?[" << wnodeName << L"]++;" << std::endl;

            //§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
            // Ausgabe der Knotenbezeichnung
            std::wcout << L"__VOR__ SIEBEN Knotenbezeichnung : " << wnodeName << std::endl;

            // Ausgabe des Startmarkers
            std::wcout << L"__VOR__ SIEBEN Endmarker : " << endMarker << std::endl;

            std::wcout << L"__VOR__ SIEBEN markerCounts4End[wnodeName] : " << markerCounts4End[wnodeName] << std::endl;

            std::cout << "__VOR__ SIEBEN insideInterval : " << insideInterval << std::endl;


            //§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§


            if (wnodeName == endMarker && insideInterval) {
                markerCounts4End[wnodeName]++;
                std::cout << "SIEBEN" << std::endl;
                if (markerCounts4End[wnodeName] == endOccurrence) {
                    insideInterval = false;
                    std::cout << "ACHT" << std::endl;
                    break;
                }
            }

            IXMLDOMNode* pNextNode = nullptr;
            pNode->get_nextSibling(&pNextNode);
            pNode->Release();
            pNode = pNextNode;
            std::cout << "NEUN" << std::endl;
        }
    }
}  // namespace RRTools

#endif  // RRTOOLS_H

// if(!RRTools::TestFirst2Nots(pRootNode, 0)) std::cout << "Problem" << std::endl;

/*
int main() {

    HRESULT hr_com = CoInitialize(NULL);
    if (FAILED(hr_com)) {
        std::cout << "Fehler bei der Initialisierung der Com-Schnittstelle" << std::endl;
        // Fehlerbehandlung
        return hr_com;
    }

    IXMLDOMDocument* pXMLDom;
    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDom));

    if (SUCCEEDED(hr)) {
        VARIANT_BOOL status;
        VARIANT xmlSource;
        xmlSource.vt = VT_BSTR;
    xmlSource.bstrVal = SysAllocString(L"BeispielG.xml");
            //xmlSource.bstrVal = SysAllocString(L"omicron.xml");
        hr = pXMLDom->load(xmlSource, &status);
        SysFreeString(xmlSource.bstrVal);

        if (status == VARIANT_TRUE) {
            IXMLDOMElement* pRootNode;
            hr = pXMLDom->get_documentElement(&pRootNode);
            if (SUCCEEDED(hr) && pRootNode) {


                //++++++++++++++++++

                RRTools::getNodeInfoByTagName(pRootNode, L"Unit");
             //processNodesBetweenMarkers(pRootNode, L"Abteilung", 2, L"Email", 1); //4,5,6,7 ist Schrott 3 und 11 Ausbau-fähig
                            //processNodesBetweenMarkers(pRootNode, L"Measurement", 2, L"Value", 1); //4,5,6,7 ist Schrott 3 und 11 Ausbau-fähig
                //~~~~~~~~~~~~~~~~~~
                        //printNode1(pRootNode, L"Abteilung",L"Entwicklung", L"Email");

                processEmailNodes(pRootNode);
                pRootNode->Release();
            }
        }
        else {
            std::cout << "Fehler beim Laden der XML-Datei." << std::endl;
        }
    }
    else {
        std::cout << "Fehler beim Erstellen des XML-DOM-Objekts." << std::endl;
    }

    if (pXMLDom) {
        pXMLDom->Release();
    }

    CoUninitialize();

    std::cout << "Drücken Sie eine beliebige Taste, um fortzufahren...";
    std::cin.get();
    return 0;
}*/



//==========================================INFO==================================
Guten Morgen! Hier sind die zehn wichtigsten Vorteile modernerer C+±Versionen im Vergleich zu C++03, jeweils mit der entsprechenden Version und einer kurzen Erklärung:

Lambda-Ausdrücke (C++11): Lambda-Ausdrücke ermöglichen es, Funktionen lokal zu definieren, was den Code kompakter und lesbarer macht. Sie eliminieren die Notwendigkeit für Funktionsobjekte und vereinfachen die Verwendung von Algorithmen1.
Automatische Typableitung (auto) (C++11): Mit dem auto-Schlüsselwort kann der Compiler den Typ einer Variablen automatisch ableiten. Dies reduziert den Schreibaufwand und macht den Code flexibler und weniger fehleranfällig1.
Rvalue-Referenzen und Move-Semantik (C++11): Rvalue-Referenzen ermöglichen es, Ressourcen effizienter zu verwalten, indem sie unnötige Kopien vermeiden. Dies führt zu einer besseren Performance, insbesondere bei der Arbeit mit temporären Objekten1.
Smart Pointer (C++11): Neue Smart Pointer wie std::unique_ptr und std::shared_ptr helfen, Speicherlecks zu vermeiden und die Speicherverwaltung zu vereinfachen. Sie bieten eine automatische Speicherfreigabe, wenn der Pointer nicht mehr benötigt wird1.
Multithreading-Unterstützung (C++11): Die Einführung der <thread>-Bibliothek ermöglicht die einfache Erstellung und Verwaltung von Threads. Dies erleichtert die Entwicklung von parallelen und nebenläufigen Anwendungen1.
Uniforme Initialisierung (C++11): Die uniforme Initialisierung mit geschweiften Klammern {} vereinfacht die Initialisierung von Variablen und Objekten. Sie beseitigt viele der Inkonsistenzen und Fallstricke der alten Initialisierungsmethoden2.
Range-based for Schleife (C++11): Diese Schleife ermöglicht eine einfachere und lesbarere Iteration über Container. Sie reduziert den Boilerplate-Code und minimiert das Risiko von Off-by-one-Fehlern1.
constexpr (C++11): Mit constexpr können Funktionen und Variablen zur Kompilierzeit ausgewertet werden. Dies führt zu effizienterem Code und ermöglicht die Definition von Konstanten, die zur Kompilierzeit berechnet werden1.
Typ-Traits (C++11): Die Typ-Traits-Bibliothek bietet Werkzeuge zur Metaprogrammierung, die es ermöglichen, Typinformationen zur Kompilierzeit zu ermitteln und zu manipulieren. Dies verbessert die Flexibilität und die Möglichkeiten der Template-Programmierung1.
std::any, std::optional und std::variant (C++17): Diese neuen Typen bieten flexible Möglichkeiten zur Handhabung von Werten, die zur Laufzeit ihren Typ ändern können (std::any), optional vorhanden sind (std::optional) oder mehrere mögliche Typen haben (std::variant). Sie verbessern die Sicherheit und Lesbarkeit des Codes1.
-----
Zu Uniforme Initialisierung
Einheitliche Initialisierung
In C++03 war die Initialisierung von Variablen für verschiedene Arten von Variablen unterschiedlich, und manchmal war sie nicht einmal möglich. Mit C++11 haben wir die sogenannte *einheitliche Initialisierung* bekommen, die versucht, den Entwicklern das ganze Thema etwas zu erleichtern.

Die einheitliche Initialisierung ist ziemlich einfach: Sie können praktisch alles mit Argumenten in geschweiften Klammern initialisieren. Der Compiler wird dann genau das Richtige tun.

Betrachten Sie diesen C++03-Code:

struct POD { int i; float f; };

class C {
  POD p;
  int iarr[3];
  double d;
public:
  C() : d(3.14) {
    p.i=2; p.f=22.34;
    for (unsigned i = 0; i < 3; ++i) iarr[i] = i;
  }
};

class D {
public:
  D(C const&, POD const&) {}
};

int main() {
  C c; 
  D d(C(), POD()); 
  int i = int();
  POD p = {i, 6.5};
  POD* pp = new POD();
  pp->i = 4;
  pp->f = 22.1;
  float pf* = new float[2];
  pf[0] = 1.2f;
  pf[1] = 2.3f;
}
Es gibt mehrere Probleme und einen eindeutigen Fehler, die mit der Initialisierung in diesem Code verbunden sind und die durch eine einheitliche Initialisierung in C++11 behoben werden können. Schauen wir uns alle Initialisierungen nacheinander an.

Initialisieren von POD-Klassenmembern
Im Konstruktor von C wird nur die Member-Variable 'd' initialisiert, was nicht sehr sauber ist. Wir sollten uns bemühen, jede Membervariable in der Initialisierungsliste zu initialisieren, denn wenn wir dies nicht tun, bleiben alte Datentypen uninitialisiert und enthalten fehlerhafte Werte, nicht einmal Nullen.

Das Problem ist, dass in C++03 Aggregate nicht in der Initialisierungsliste initialisiert werden können, sondern ihre Werte erst später zugewiesen bekommen müssen. Mit einer einheitlichen Initialisierung können sie:

class C {
  //...
  C()
    : p{2, 44.11}
    , iarr{0, 1, 2}
    , d{22.34}
  {}
};
 

Wie Sie sehen können, kann das 'double'-Element, das kein Aggregat ist, auch mit geschweiften Klammern initialisiert werden.

Aufrufen von Konstruktoren und die lästigste Analyse
In C++03 gibt es ein Problem, das als "ärgerlichste Analyse" bezeichnet wird und von der Tatsache herrührt, dass Klammern auch für Funktionsdeklarationen, Funktionsaufrufe und Initialisierungen verwendet werden.

Das einfachste Beispiel ist der "explizite Aufruf eines Standardkonstruktors", der in C++03 nicht möglich ist. Wenn ein Aufruf mit einem Argument wie 'C c(22);' aussieht, dann sollte er mit null Argumenten wie 'C c();' aussehen. Der Compiler analysiert dies jedoch als Deklaration einer Funktion 'c', gibt ein 'C' zurück und akzeptiert keine Argumente. Wir mussten uns daher auf einen impliziten Aufruf des Standardkonstruktors verlassen, der "C c;" schrieb.

Zeile 21 ist nicht sehr anders – wir *denken*, dass wir ein 'D' mit dem Namen 'd' konstruieren, mit zwei Konstruktorargumenten, die ein standardkonstruiertes 'C' und ein standardkonstruiertes 'POD' sind. Der Compiler liest dies jedoch als Deklaration einer Funktion namens 'd', die ein 'D' zurückgibt, wobei als erstes Argument eine Funktion verwendet wird, die ein 'C' ohne Parameter zurückgibt, und als zweites eine Funktion, die ein 'POD' ohne Argumente zurückgibt.

Bei einer einheitlichen Initialisierung sind die geschweiften Klammern nicht mehrdeutig, und wir können jede Initialisierung verwenden, die andernfalls dem ärgerlichsten Parsen zum Opfer fallen würde:

C c{};
D d{C{}, POD{}};
Natürlich können wir jeden Konstrukteur mit geschweiften Klammern aufrufen.

Initialisieren von einfachen alten Daten und Aggregaten
Angesichts dessen, was ich bisher über die einheitliche Initialisierung geschrieben habe, wird die Initialisierung von einfachen alten Daten und Aggregaten keine Überraschung sein, daher werde ich der Vollständigkeit halber einfach die restlichen Zeilen von 'main' mit einheitlicher Initialisierung auswerfen:

int i{};
POD p{i, 6.5};
POD* pp = new POD{4, 22.1};
float* pf = new float[2]{1.2, 2.3};
Wie Sie sehen können, ist es nicht mehr notwendig, Werte nach der Initialisierung eines Aggregats "anzupassen", da es durch eine einheitliche Initialisierung alle benötigten Werte erhalten kann.

Aufbau von Funktionsparametern
Sie können Funktionsparameter sogar im laufenden Betrieb erstellen, ohne ihren Typ anzugeben, solange die Parametertypen eindeutig bleiben. Beispielsweise gibt es nur einen Konstruktor von 'D', sodass die folgende Zeile nicht mehrdeutig ist:

D d{{}, {}};
Es heißt einfach "default construct the first and second argument" – und der Compiler kann herausfinden, welchen Typ diese Argumente haben sollten.

std::initializer_list
Im Standardese werden die geschweiften Klammern mit der Liste der Elemente als braced-init-list bezeichnet, und die Liste der Elemente selbst wird als initializer-list bezeichnet. In einigen Fällen wäre es praktisch, wenn wir einfach ein Objekt vom Typ Klasse mit einer solchen Liste konstruieren könnten, z.B. wenn wir einen Container mit einer Liste seiner Elemente initialisieren könnten, genau wie ein Array:

std::vector<std::string> names{ "Braum", "Janna", "Leona", "Sona" };
Und tatsächlich wurde ein Weg, dies zu erreichen, in den C++11-Standard aufgenommen, der die obige Zeile zu gültigem C++ macht.

C++11 führt einen neuen Typ 'std::initializer_list<T>' ein. Objekte dieser Klasse sind Containerproxys mit Forward-Iteratoren und einer Größe für ein temporäres Array.

Sie sind in erster Linie für solche Listeninitialisierungen gedacht, können aber auch für Funktionsaufrufe verwendet werden, z.B. ruft 'f({1, 2, 5});' eine Funktion 'f' auf, die einen 'initializer_list<int>' Parameter hat.

std::initializer_list und einheitliche Initialisierung
Meistens spielen die beiden Funktionen, die ich vorgestellt habe, sehr gut zusammen, z.B. wenn man eine Map initialisieren möchte, kann man eine Initializer-Liste von braced-init-lists der Schlüssel-Wert-Paare verwenden:

std::map<std::string, int> scores{ 
  {"Alex", 522}, {"Pumu", 423}, {"Kitten", 956} 
};
Hier ist der Typ der Paare klar und der Compiler wird daraus schließen, dass '{"Alex", 522}' tatsächlich 'std::p air<std::string const, int>{"Alex", 522}' bedeutet.

Aber manchmal können "initializer_list"-Konstrukteure mit den anderen Konstrukteuren konkurrieren. Das prominenteste Beispiel ist der Konstruktor von 'std::vector<int>' für ein Größen- und ein Einzelwertargument, mit dem der Vektor gefüllt werden soll:

std::vector<int> aDozenOfFives{12, 5};
Die Absicht wird aus der Namensgebung deutlich, der Programmierer hätte erwartet, einen Vektor zu erhalten, der zwölf Elemente hat, die jeweils den Wert 5 haben. Der Compiler sieht dies jedoch als einen Aufruf des Konstruktors 'initializer_list' von 'vector', der einen Vektor mit zwei Elementen mit den Werten 12 und 5 erzeugt.
    
Um in solchen Fällen das gewünschte Verhalten zu erzielen, muss die Old-School-Syntax mit Klammern verwendet werden: 'std::vector<int> aDozenOfFoves(12, 5);'

//====================================================
Natürlich! Hier sind praktische Szenarien für jeden der genannten Vorteile:

Lambda-Ausdrücke (C++11): Szenario: Du schreibst eine Funktion, die eine Liste von Zahlen filtert. Mit Lambda-Ausdrücken kannst du die Filterlogik direkt in der std::filter-Funktion definieren, ohne eine separate Funktionsdefinition zu benötigen.
Automatische Typableitung (auto) (C++11): Szenario: Du iterierst über eine komplexe Datenstruktur wie std::map<std::string, std::vector<int>>. Mit auto kannst du den Typ der Iteratoren automatisch ableiten, was den Code kürzer und lesbarer macht.
Rvalue-Referenzen und Move-Semantik (C++11): Szenario: Du schreibst eine Funktion, die große Datenmengen verarbeitet und zurückgibt. Mit Move-Semantik kannst du die Daten effizienter zurückgeben, ohne unnötige Kopien zu erzeugen, was die Performance verbessert.
Smart Pointer (C++11): Szenario: Du entwickelst eine Anwendung, die dynamisch Speicher allokiert. Mit std::unique_ptr kannst du sicherstellen, dass der Speicher automatisch freigegeben wird, wenn der Pointer außer Gebrauch kommt, wodurch Speicherlecks vermieden werden.
Multithreading-Unterstützung (C++11): Szenario: Du implementierst eine Anwendung, die mehrere Aufgaben parallel ausführt, wie z.B. das Herunterladen von Dateien. Mit der <thread>-Bibliothek kannst du einfach Threads erstellen und verwalten, um die Aufgaben gleichzeitig auszuführen.
Uniforme Initialisierung (C++11): Szenario: Du initialisierst ein Objekt einer Klasse mit mehreren Konstruktoren. Mit der uniformen Initialisierung kannst du sicherstellen, dass alle Member-Variablen korrekt und konsistent initialisiert werden.
Range-based for Schleife (C++11): Szenario: Du iterierst über ein std::vector von Objekten. Mit der range-based for Schleife kannst du den Code kürzer und lesbarer gestalten, indem du direkt über die Elemente iterierst.
constexpr (C++11): Szenario: Du definierst eine mathematische Funktion, die zur Kompilierzeit ausgewertet werden soll, wie z.B. die Berechnung einer Fakultät. Mit constexpr kannst du sicherstellen, dass die Berechnung zur Kompilierzeit erfolgt, was die Laufzeitperformance verbessert.
Typ-Traits (C++11): Szenario: Du schreibst eine Template-Funktion, die nur für bestimmte Typen funktionieren soll. Mit Typ-Traits kannst du zur Kompilierzeit überprüfen, ob ein Typ bestimmte Eigenschaften erfüllt, und den Code entsprechend anpassen.
std::any, std::optional und std::variant (C++17): Szenario: Du entwickelst eine API, die verschiedene Rückgabewerte haben kann. Mit std::variant kannst du mehrere mögliche Rückgabetypen in einer einzigen Schnittstelle kapseln, was die Handhabung und Sicherheit des Codes verbessert.
    
    




    




























