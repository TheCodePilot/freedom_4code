#include "RRTools.h"

#include <iostream>  // Für std::wcout
#include <map>  // Für std::map
#include <string>  // Für std::wstring
#include <comdef.h>  // Für _bstr_t
#include <msxml6.h>  // Für IXMLDOMNode
#pragma comment(lib, "msxml6.lib")

#include <locale>

    //std::locale::global(std::locale(""));
//if (wnodeName == targetName && wnodeValue == targetValue)

void printNode1(IXMLDOMNode* pNode, const std::wstring& targetName, const std::wstring& targetValue, const std::wstring& stopName, int depth = 0) {
    BSTR nodeName, nodeValue;
    pNode->get_nodeName(&nodeName);
    pNode->get_text(&nodeValue);
    std::wstring wnodeName(nodeName);
    std::wstring wnodeValue(nodeValue);
    if (nodeName && wcscmp(nodeName, L"#text") == 0) {
        IXMLDOMNode* pParentNode;
        pNode->get_parentNode(&pParentNode);
        if (pParentNode) {
            BSTR parentName;
            pParentNode->get_nodeName(&parentName);
            for (int i = 0; i < depth; ++i) std::wcout << L"  ";
            if (wnodeName == targetName && wnodeValue == targetValue) {
                std::wcout << L"Der Knoten hat den Namen \"" << parentName << L"\" und den Wert \"" << nodeValue << L"\"." << std::endl;
            }
            SysFreeString(parentName);
            pParentNode->Release();
        }
    }
    SysFreeString(nodeName);
    SysFreeString(nodeValue);

    IXMLDOMNode* pChildNode;
    pNode->get_firstChild(&pChildNode);
    while (pChildNode) {
        printNode1(pChildNode, targetName, targetValue, stopName, depth + 1);
        IXMLDOMNode* pNextNode;
        pChildNode->get_nextSibling(&pNextNode);
        pChildNode->Release();
        pChildNode = pNextNode;
    }
}




void processNode_sonst(IXMLDOMNode* pNode) {
    BSTR nodeName, nodeValue;
    pNode->get_nodeName(&nodeName);
    pNode->get_text(&nodeValue);

    // Verarbeiten Sie den Knoten oder speichern Sie ihn für weitere Verwendungszwecke
    std::wcout << "Process-Result" << L"Node Name: " << nodeName << L", Value: " << nodeValue << std::endl;

    SysFreeString(nodeName);
    SysFreeString(nodeValue);
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
            RRTools::processNodePlus(pChild);    //hier schauen was sinnvoll
        }

        processEmailNodes(pChild);
    }
}

//Verssuch mit DOMNodeType nodeType;
//sei gesucht Werte wo:
// Wert von <bstrName> == C1 (f)
//Wert von <Date> == 02 / 26 / 2019 10:57 : 29
//main_bevorobigesKriterium

int main() {

    HRESULT hr_com = CoInitialize(NULL);
    if (FAILED(hr_com)) {
        std::cout << "Fehler bei der Initialisierung der COM-Schnittstelle" << std::endl;
        return hr_com;
    }

    IXMLDOMDocument3* pXMLDom;
    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDom));
    if (FAILED(hr)) {
        std::cout << "Fehler beim Erstellen des XML-DOM-Objekts." << std::endl;
        CoUninitialize();
        return hr;
    }

    VARIANT_BOOL status;
    VARIANT xmlSource;
    xmlSource.vt = VT_BSTR;
    xmlSource.bstrVal = SysAllocString(L"Beispiel_10KV.xml");
    hr = pXMLDom->load(xmlSource, &status);
    SysFreeString(xmlSource.bstrVal);

    if (status == VARIANT_TRUE) {
        IXMLDOMElement* pRootNode;
        hr = pXMLDom->get_documentElement(&pRootNode);
        if (SUCCEEDED(hr) && pRootNode) {
            IXMLDOMNodeList* pObjects;  //DEKLARATION: Alle Kinder von pRootNode
            hr = pRootNode->get_childNodes(&pObjects); //BEKOMME: Alle Kinder von pRootNode
            if (FAILED(hr)) {
                std::cerr << "Fehler beim Abrufen der Objektknoten." << std::endl;
                pXMLDom->Release();
                CoUninitialize();
                return hr;
            }

            long nodeListLength = 0;
            hr = pObjects->get_length(&nodeListLength);
            if (nodeListLength == 0) {
                std::cout << "Die Objektknotenliste ist leer." << std::endl;
                pObjects->Release();
                pXMLDom->Release();
                CoUninitialize();
                return 0;
            }

            IXMLDOMNode* pLastChildNode;
            hr = pRootNode->get_lastChild(&pLastChildNode);
            if (FAILED(hr)) {
                std::cerr << "Fehler beim Abrufen des letzten Kindknotens vom Wurzelelement." << std::endl;
                pRootNode->Release();
                pXMLDom->Release();
                CoUninitialize();
                return hr;
            }

            IXMLDOMNode* pCardsNode = NULL;
            IXMLDOMNode* pNodeConditionOK = NULL;


            IXMLDOMNode* pNodeRoot;
            HRESULT hr_loop = S_OK; // Variable zur Überwachung des Schleifenstatus
            while (pObjects->nextNode(&pNodeRoot) == S_OK)  //SCHLEIFE: Solange Root Kinder hat
            {
                if (hr_loop != S_OK) {
                    // Wenn nextNode kein weiteres Element zurückgibt, brechen Sie die Schleife ab
                    break;
                }

                BSTR childNodeName;
                hr = pNodeRoot->get_nodeName(&childNodeName);
                if (SUCCEEDED(hr) && childNodeName && wcscmp(childNodeName, L"Cards") == 0) {
                    // Wenn der Knoten "Cards" ist, speichern Sie ihn und brechen Sie die Schleife ab
                    pCardsNode = pNodeRoot;
                    SysFreeString(childNodeName);
                    break;
                }
                SysFreeString(childNodeName);
            }

            pObjects->Release(); // Freigeben des Speichers für die Knotenliste

            if (pCardsNode) {
                // Hier haben Sie den Knoten "Cards", von dem aus Sie die Kinder abrufen können
                // Fügen Sie hier Ihren Code hinzu, um die Kinder des "Cards"-Knotens zu verarbeiten
                IXMLDOMNodeList* pChildrenOfCards;
                hr = pCardsNode->get_childNodes(&pChildrenOfCards); //hier nun Cards_Item erhalten
                if (SUCCEEDED(hr)) {
                    IXMLDOMNode* pChildNodeOfCards;
                    while (pChildrenOfCards->nextNode(&pChildNodeOfCards) == S_OK) {
                        // Verarbeiten Sie hier die Kinder des "Cards"-Knotens
                        BSTR childNodeName;
                        hr = pChildNodeOfCards->get_nodeName(&childNodeName);
                                           //sollte rein |+| && wcscmp(childNodeName, L"Cards_Item") == 0
                           
                            if (SUCCEEDED(hr)) {
                                std::wcout << "Kinder Von Cards  __ Knotenname: " << childNodeName << std::endl; //", Wert: " << value << std::endl;
                                //SysFreeString(value);
                            }
                            SysFreeString(childNodeName);

                            //Nun Da hier alle Kindsknoten nun hier erscheinen
                            IXMLDOMNodeList* pChildrenChildrenOfCards; //Für die Kinder von Card_Item
                            hr = pChildNodeOfCards->get_childNodes(&pChildrenChildrenOfCards);
                            if (SUCCEEDED(hr)) {

                                IXMLDOMNode* pChildChildNodeOfCards = NULL;

                                while (pChildrenChildrenOfCards->nextNode(&pChildChildNodeOfCards) == S_OK) {

                                    BSTR childChildNodeName;
                                    hr = pChildChildNodeOfCards->get_nodeName(&childChildNodeName);
                                    if (SUCCEEDED(hr) && wcscmp(childChildNodeName, L"nType") == 0) {                                                //hier neu | && wcscmp(childChildNodeName, L"nType") == 0)
                                        BSTR value;
                                        hr = pChildChildNodeOfCards->get_text(&value);
                                        if (SUCCEEDED(hr) && wcscmp(value, L"21") == 0) {
                                            // Wenn der Wert "21" ist, überprüfen Sie die anderen Bedingungen
                                            // Hier können Sie weitere Bedingungen hinzufügen, z. B. für <bstrName> und <Date>
                                            IXMLDOMNode* pSiblingNode = NULL;
                                            hr = pChildChildNodeOfCards->get_nextSibling(&pSiblingNode);
                                            if (SUCCEEDED(hr))
                                            {
                                                // Wert von <bstrName> == C1 (f)
                                                BSTR siblingNodeName;
                                                hr = pSiblingNode->get_nodeName(&siblingNodeName);
                                                if (SUCCEEDED(hr) && wcscmp(siblingNodeName, L"bstrName") == 0)
                                                {
                                                    BSTR siblingNodeValue;
                                                    hr = pSiblingNode->get_text(&siblingNodeValue);
                                                    if (SUCCEEDED(hr) && wcscmp(siblingNodeValue, L"C1 (f)") == 0)
                                                    {
                                                        SysFreeString(siblingNodeValue);
                                                        //erneut Schleife aller siblings
                                                        //----------------------------
                                                        IXMLDOMNode* pParentNode;
                                                        hr = pSiblingNode->get_parentNode(&pParentNode);
                                                        if (SUCCEEDED(hr) && pParentNode) {
                                                            IXMLDOMNodeList* pSiblings;
                                                            hr = pParentNode->get_childNodes(&pSiblings);
                                                            if (SUCCEEDED(hr) && pSiblings) {
                                                                IXMLDOMNode* pSibling;
                                                                while (pSiblings->nextNode(&pSibling) == S_OK) {
                                                                    // Verarbeiten Sie hier jeden Geschwisterknoten...
                                                                    // Wert von <Date> == 02/26/2019 10:57:29
                                                                    BSTR siblingNodeName;
                                                                    hr = pSibling->get_nodeName(&siblingNodeName);
                                                                    if (SUCCEEDED(hr) && wcscmp(siblingNodeName, L"Date") == 0)
                                                                    {
                                                                        BSTR siblingNodeValue;
                                                                        hr = pSibling->get_text(&siblingNodeValue);
                                                                        if (SUCCEEDED(hr) && wcscmp(siblingNodeValue, L"02/26/2019 10:57:29") == 0)
                                                                        {
                                                                            std::wcout << "Der richtige Cards_Item gefunden  __ Knotenname: " <<  " Wert: " << siblingNodeValue << std::endl;
                                                                            //+++++++++++Wenn bedingung erfüllt+++++++++++++++++++++++
                                                                            pNodeConditionOK = pChildChildNodeOfCards;  // Wenn alle Bedingungen erfüllt sind, speichern Sie den Knoten und brechen Sie die Schleife ab
                                                                            //+++++++++++Wenn bedingung erfüllt+++++++++++++++++++++++
                                                                            //Nun mit den richtigen Cards_Item sind dier Werte in Greifbarer Nähe 
                                                                            
                                                                        }

                                                                    }

                                                                    pSibling->Release(); // Freigeben des Speichers
                                                                }
                                                                SysFreeString(siblingNodeValue);
                                                                SysFreeString(siblingNodeName);
                                                                pSiblings->Release(); // Freigeben des Speichers
                                                            }
                                                            pParentNode->Release(); // Freigeben des Speichers
                                                        }


                                                        //----------------------------

                                                    }

                                                }

                                            }

                                            SysFreeString(value);
                                            SysFreeString(childChildNodeName);
                                            //pChildChildNodeOfCards->Release();
                                            break;  // Speicher freigeben
                                        }
                                        SysFreeString(value);

                                    }
                                    SysFreeString(childChildNodeName);
                                    //pChildChildNodeOfCards->Release(); // Freigeben des Speichers für den Kindknoten
                                }
                                pChildrenChildrenOfCards->Release(); // Freigeben des Speichers für die Liste der Kinder
                            }
                        
                        pChildNodeOfCards->Release(); // Freigeben des Speichers für den Kindknoten
                        SysFreeString(childNodeName);
                    } //ENDE while (pChildrenOfCards->nextNode(&pChildNodeOfCards) == S_OK)

                    pChildrenOfCards->Release(); // Freigeben des Speichers für die Liste der Kinder
                }
                pCardsNode->Release(); // Freigeben des Speichers für den "Cards"-Knoten
            }
            else {
                std::cerr << "Der Knoten 'Cards' wurde nicht gefunden." << std::endl;
            }

            pNodeConditionOK->Release(); // Freigeben des Speichers für den Kindknoten

            pRootNode->Release();
            pXMLDom->Release(); // Freigeben des Speichers
            CoUninitialize();

            std::cout << "Drücken Sie eine beliebige Taste, um fortzufahren...";
            std::cin.get();
            return 0;
        }
    }
}


//Verssuch mit DOMNodeType nodeType;
//sei gesucht Werte wo:
// Wert von <bstrName> == C1 (f)
//Wert von <Date> == 02 / 26 / 2019 10:57 : 29
int main_nix() {

    HRESULT hr_com = CoInitialize(NULL);
    if (FAILED(hr_com)) {
        std::cout << "Fehler bei der Initialisierung der COM-Schnittstelle" << std::endl;
        return hr_com;
    }

    IXMLDOMDocument3* pXMLDom;
    HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDom));
    if (FAILED(hr)) {
        std::cout << "Fehler beim Erstellen des XML-DOM-Objekts." << std::endl;
        CoUninitialize();
        return hr;
    }

    VARIANT_BOOL status;
    VARIANT xmlSource;
    xmlSource.vt = VT_BSTR;
    xmlSource.bstrVal = SysAllocString(L"Beispiel_10KV.xml");
    hr = pXMLDom->load(xmlSource, &status);
    SysFreeString(xmlSource.bstrVal);

    if (status == VARIANT_TRUE) {
        IXMLDOMElement* pRootNode;
        hr = pXMLDom->get_documentElement(&pRootNode);
        if (SUCCEEDED(hr) && pRootNode) {
            IXMLDOMNodeList* pObjects;  //DEKLARATION: Alle Kinder von pRootNode
            hr = pRootNode->get_childNodes(&pObjects); //BEKOMME: Alle Kinder von pRootNode
            if (FAILED(hr)) {
                std::cerr << "Fehler beim Abrufen der Objektknoten." << std::endl;
                pXMLDom->Release();
                CoUninitialize();
                return hr;
            }

            long nodeListLength = 0;
            hr = pObjects->get_length(&nodeListLength);
            if (nodeListLength == 0) {
                std::cout << "Die Objektknotenliste ist leer." << std::endl;
                pObjects->Release();
                pXMLDom->Release();
                CoUninitialize();
                return 0;
            }

            IXMLDOMNode* pLastChildNode;
            hr = pRootNode->get_lastChild(&pLastChildNode);
            if (FAILED(hr)) {
                std::cerr << "Fehler beim Abrufen des letzten Kindknotens vom Wurzelelement." << std::endl;
                pRootNode->Release();
                pXMLDom->Release();
                CoUninitialize();
                return hr;
            }

            IXMLDOMNode* pCardsNode = NULL;
            IXMLDOMNode* pCards_ItemNode = NULL;

            IXMLDOMNode* pNodeRoot;
            HRESULT hr_loop = S_OK; // Variable zur Überwachung des Schleifenstatus
            while (pObjects->nextNode(&pNodeRoot) == S_OK)  //SCHLEIFE: Solange Root Kinder hat
            {
                if (hr_loop != S_OK) {
                    // Wenn nextNode kein weiteres Element zurückgibt, brechen Sie die Schleife ab
                    break;
                }

                BSTR childNodeName;
                hr = pNodeRoot->get_nodeName(&childNodeName);
                if (SUCCEEDED(hr) && childNodeName && wcscmp(childNodeName, L"Cards") == 0) {
                    // Wenn der Knoten "Cards" ist, speichern Sie ihn und brechen Sie die Schleife ab
                    pCardsNode = pNodeRoot;
                    SysFreeString(childNodeName);
                    break;
                }
                SysFreeString(childNodeName);
            }

            pObjects->Release(); // Freigeben des Speichers für die Knotenliste

            if (pCardsNode) {
                // Hier haben Sie den Knoten "Cards", von dem aus Sie die Kinder abrufen können
                // Fügen Sie hier Ihren Code hinzu, um die Kinder des "Cards"-Knotens zu verarbeiten
                IXMLDOMNodeList* pChildrenOfCards;
                hr = pCardsNode->get_childNodes(&pChildrenOfCards);
                if (SUCCEEDED(hr)) {
                    IXMLDOMNode* pChildNodeOfCards;
                    while (pChildrenOfCards->nextNode(&pChildNodeOfCards) == S_OK) {
                        // Verarbeiten Sie hier die Kinder des "Cards"-Knotens
                        BSTR childNodeName;
                        hr = pChildNodeOfCards->get_nodeName(&childNodeName);
                        if (SUCCEEDED(hr)) {                                            //sollte rein |+| && wcscmp(childNodeName, L"Cards_Item") == 0
                            // Hier haben Sie den Namen des Kindknotens
                            // Fügen Sie hier Ihren weiteren Verarbeitungscode hinzu
                            BSTR value;
                            hr = pNodeRoot->get_text(&value);
                            if (SUCCEEDED(hr)) {
                                std::wcout << "Kinder Von Cards  __ Knotenname: " << childNodeName << std::endl; //", Wert: " << value << std::endl;
                                SysFreeString(value);
                            }
                            SysFreeString(childNodeName);

                            //Nun Da hier alle Kindsknoten nun hier erscheinen
                            IXMLDOMNodeList* pChildrenChildrenOfCards;
                            hr = pChildNodeOfCards->get_childNodes(&pChildrenChildrenOfCards);
                            if (SUCCEEDED(hr)) {

                                IXMLDOMNode* pChildChildNodeOfCards = NULL;

                                while (pChildrenChildrenOfCards->nextNode(&pChildChildNodeOfCards) == S_OK) {

                                    BSTR childChildNodeName;
                                    hr = pChildChildNodeOfCards->get_nodeName(&childChildNodeName);
                                    
                                    /*if (SUCCEEDED(hr) && wcscmp(childChildNodeName, L"nType") == 0) {                                                //hier neu | && wcscmp(childChildNodeName, L"nType") == 0)
                                        BSTR value;

                                        if (SUCCEEDED(hr) && wcscmp(value, L"21") == 0)                       //hier neu | && wcscmp(value, L"21") raus || && childChildNodeName && wcscmp(childChildNodeName, L"nType") == 0
                                        {
                                            //TEST-Ausgabe
                                            std::wcout << "Kinder Von Cards SOLLTE DER NAME Cards_Item sein und der  __ Knotenname__ lautet: " << childChildNodeName << ", Wert: " << value << std::endl;
                                            //TEST-Ausgabe

                                            pCards_ItemNode = pChildChildNodeOfCards;
                                            SysFreeString(childChildNodeName);
                                            pChildChildNodeOfCards->Release();
                                            break;  //Speicherfreigeben im Falle von break !!!
                                        }
                                        SysFreeString(value);

                                    }*/
                                    if (SUCCEEDED(hr) && wcscmp(childChildNodeName, L"nType") == 0)
                                    {
                                        BSTR value;
                                        hr = pChildChildNodeOfCards->get_text(&value);
                                        if (SUCCEEDED(hr) && wcscmp(value, L"21") == 0)
                                        {
                                            SysFreeString(value);

                                            // Überprüfen der Geschwisterknoten
                                            IXMLDOMNode* pSiblingNode = NULL;
                                            hr = pChildChildNodeOfCards->get_nextSibling(&pSiblingNode);
                                            if (SUCCEEDED(hr))
                                            {
                                                // Wert von <bstrName> == C1 (f)
                                                BSTR siblingNodeName;
                                                hr = pSiblingNode->get_nodeName(&siblingNodeName);
                                                if (SUCCEEDED(hr) && wcscmp(siblingNodeName, L"bstrName") == 0)
                                                {
                                                    BSTR siblingNodeValue;
                                                    hr = pSiblingNode->get_text(&siblingNodeValue);
                                                    if (SUCCEEDED(hr) && wcscmp(siblingNodeValue, L"C1 (f)") == 0)
                                                    {
                                                        SysFreeString(siblingNodeValue);
                                                        pSiblingNode->Release();

                                                        // Überprüfen des nächsten Geschwisterknotens
                                                        hr = pSiblingNode->get_nextSibling(&pSiblingNode);
                                                        if (SUCCEEDED(hr))
                                                        {
                                                            // Wert von <Date> == 02/26/2019 10:57:29
                                                            BSTR siblingNodeName;
                                                            hr = pSiblingNode->get_nodeName(&siblingNodeName);
                                                            if (SUCCEEDED(hr) && wcscmp(siblingNodeName, L"Date") == 0)
                                                            {
                                                                BSTR siblingNodeValue;
                                                                hr = pSiblingNode->get_text(&siblingNodeValue);
                                                                if (SUCCEEDED(hr) && wcscmp(siblingNodeValue, L"02/26/2019 10:57:29") == 0)
                                                                {
                                                                    SysFreeString(siblingNodeValue);
                                                                    pSiblingNode->Release();

                                                                    // Jetzt haben wir den gewünschten Knoten "Cards_Item"
                                                                    pCards_ItemNode = pChildChildNodeOfCards;
                                                                    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                                                                    BSTR childNodeName42;
                                                                    hr = pCards_ItemNode->get_nodeName(&childNodeName42);
                                                                    if (SUCCEEDED(hr)) {                                            //sollte rein |+| && wcscmp(childNodeName, L"Cards_Item") == 0
                                                                        // Hier haben Sie den Namen des Kindknotens
                                                                        // Fügen Sie hier Ihren weiteren Verarbeitungscode hinzu
                                                                        BSTR value42;
                                                                        hr = pCards_ItemNode->get_text(&value42);
                                                                        if (SUCCEEDED(hr)) {
                                                                            std::wcout << "gewuenschten Knoten Cards_Item: " << childNodeName42 << std::endl; //", Wert: " << value42 << std::endl;
                                                                            SysFreeString(value42);
                                                                        }
                                                                        SysFreeString(childNodeName42);
                                                                    }
                                                                    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
                                                                    SysFreeString(childChildNodeName);
                                                                    pChildChildNodeOfCards->Release();
                                                                    break;
                                                                }
                                                                SysFreeString(siblingNodeValue);
                                                            }
                                                            pSiblingNode->Release();
                                                        }
                                                    }
                                                    SysFreeString(siblingNodeName);
                                                }
                                                pSiblingNode->Release();
                                            }
                                        }
                                        SysFreeString(value);
                                    }




                                    SysFreeString(childChildNodeName);
                                    pChildChildNodeOfCards->Release(); // Freigeben des Speichers für den Kindknoten
                                }
                                pChildrenChildrenOfCards->Release(); // Freigeben des Speichers für die Liste der Kinder
                            }
                        }
                        pChildNodeOfCards->Release(); // Freigeben des Speichers für den Kindknoten
                        SysFreeString(childNodeName);
                    }
                    pChildrenOfCards->Release(); // Freigeben des Speichers für die Liste der Kinder
                }
                pCardsNode->Release(); // Freigeben des Speichers für den "Cards"-Knoten
            }
            else {
                std::cerr << "Der Knoten 'Cards' wurde nicht gefunden." << std::endl;
            }

            pRootNode->Release();
            pXMLDom->Release(); // Freigeben des Speichers
            CoUninitialize();

            std::cout << "Drücken Sie eine beliebige Taste, um fortzufahren...";
            std::cin.get();
            return 0;
        }
    }
}















