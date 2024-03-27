//Temp_ins_suabere Projekt

//Das Beste !!!
#include <iostream>  // Für std::wcout
#include <map>  // Für std::map
#include <string>  // Für std::wstring
#include <comdef.h>  // Für _bstr_t
#include <msxml6.h>  // Für IXMLDOMNode
#pragma comment(lib, "msxml6.lib")

void TestVergleiche0(IXMLDOMNode* pNode1, IXMLDOMNode* pNode2) {
    BSTR nodeName1, nodeName2;
    pNode1->get_nodeName(&nodeName1);
    pNode2->get_nodeName(&nodeName2);

    std::wcout << L"Vergleiche Knoten: " << nodeName1 << L" und " << nodeName2 << std::endl;

    if (wcscmp(nodeName1, nodeName2) == 0) {
        std::wcout << L"Die Knoten sind gleich." << std::endl;
    }
    else {
        std::wcout << L"Die Knoten sind nicht gleich." << std::endl;
    }

    SysFreeString(nodeName1);
    SysFreeString(nodeName2);
}

void TestVergleiche(IXMLDOMNode* pNode1, IXMLDOMNode* pNode2) {
    BSTR nodeName1, nodeName2;
    pNode1->get_nodeName(&nodeName1);
    pNode2->get_nodeName(&nodeName2);

    std::wstring wnodeName1(nodeName1);
    std::wstring wnodeName2(nodeName2);

    std::wcout << L"Vergleiche Knoten: " << wnodeName1 << L" und " << wnodeName2 << std::endl;

    if (wnodeName1 == wnodeName2) {
        std::wcout << L"Die Knoten sind gleich." << std::endl;
    }
    else {
        std::wcout << L"Die Knoten sind nicht gleich." << std::endl;
    }

    SysFreeString(nodeName1);
    SysFreeString(nodeName2);
}

//############################################Ende Testroutinen##########################################

void printNode(IXMLDOMNode* pNode, int depth = 0) {
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

void processNode(IXMLDOMNode* pNode) {
    BSTR nodeName, nodeValue;
    pNode->get_nodeName(&nodeName);
    pNode->get_text(&nodeValue);

    // Verarbeiten Sie den Knoten oder speichern Sie ihn für weitere Verwendungszwecke
    std::wcout << "Process-Result" << L"Node Name: " << nodeName << L", Value: " << nodeValue << std::endl;

    SysFreeString(nodeName);
    SysFreeString(nodeValue);
}

void ProcessNodesByTagName(IXMLDOMNode* pNode, const std::wstring& tagName) {
    BSTR nodeName;
    pNode->get_nodeName(&nodeName);
    std::wstring wnodeName(nodeName);
    SysFreeString(nodeName);

    if (wnodeName == tagName) {
        // Die Bezeichnung des aktuellen Knotens entspricht der Ziel-Zeichenkette
        // Führen Sie hier die gewünschten Aktionen aus, z. B. den aktuellen Knoten verarbeiten
        // oder speichern
        processNode(pNode);
    }

    // Rekursiv die Kinderknoten durchlaufen
    IXMLDOMNode* pChildNode;
    pNode->get_firstChild(&pChildNode);
    while (pChildNode) {
        ProcessNodesByTagName(pChildNode, tagName);
        IXMLDOMNode* pNextNode;
        pChildNode->get_nextSibling(&pNextNode);
        pChildNode->Release();
        pChildNode = pNextNode;
    }
}

void processEmailNodes(IXMLDOMNode* pNode) {
    if (pNode == NULL) {
        return;
    }

    BSTR nodeName;
    pNode->get_nodeName(&nodeName);

    IXMLDOMNodeList* pChildren;
    pNode->get_childNodes(&pChildren);

    long length;
    pChildren->get_length(&length);

    for (long i = 0; i < length; i++) {
        IXMLDOMNode* pChild;
        pChildren->get_item(i, &pChild);

        BSTR childNodeName;
        pChild->get_nodeName(&childNodeName);

        if (wcscmp(childNodeName, L"Email") == 0) {
            processNode(pChild);    //hier schauen was sinnvoll
        }

        processEmailNodes(pChild);
    }
}

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

        if (wnodeName == startMarker) {
            std::cout << "ZWEI" << std::endl;
            markerCounts4Start[wnodeName]++;
            abteilungCount++;
            std::cout << "abteilungCount ist nun" << abteilungCount << std::endl;

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
            processEmailNodes(pNode);
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
        hr = pXMLDom->load(xmlSource, &status);
        SysFreeString(xmlSource.bstrVal);

        if (status == VARIANT_TRUE) {
            IXMLDOMElement* pRootNode;
            hr = pXMLDom->get_documentElement(&pRootNode);
            if (SUCCEEDED(hr) && pRootNode) {
                //++++++++++++++++++
                IXMLDOMNode* pNode1 = NULL;
                IXMLDOMNode* pNode2 = NULL;

                // Erster und zweiter Knoten des Wurzelknotens holen
                pRootNode->get_firstChild(&pNode1);
                if (pNode1) {
                    pNode1->get_nextSibling(&pNode2);
                    if (pNode2) {
                        // TestVergleiche() aufrufen
                        TestVergleiche0(pNode1, pNode2);
                    }
                    pNode1->Release();
                }
                if (pNode2) {
                    pNode2->Release();
                }
                //++++++++++++++++++
                //ProcessNodesByTagName(pRootNode, L"Email");
                processNodesBetweenMarkers(pRootNode, L"Abteilung", 2, L"Email", 1); //4,5,6,7 ist Schrott 3 und 11 Ausbau-fähig

                //~~~~~~~~~~~~~~~~~~
                //printNode(pRootNode);
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
}