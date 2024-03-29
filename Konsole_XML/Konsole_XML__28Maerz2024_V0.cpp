#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include <stdio.h>
#include "rr_system.h"
#include <iostream>

//---Neu
//#include "RRTools.h"

//#include <iostream>  // Für std::wcout
#include <map>  // Für std::map
#include <string>  // Für std::wstring
//#include <comdef.h>  // Für _bstr_t
#include <msxml6.h>  // Für IXMLDOMNode
#pragma comment(lib, "msxml6.lib")

#include <locale>
//---Neu

//OK bis Results

//Verssuch mit DOMNodeType nodeType;
//sei gesucht Werte wo:
// Wert von <bstrName> == C1 (f)
//Wert von <Date> == 02 / 26 / 2019 10:57 : 29
//main_bevorobigesKriterium

//HR noch abfangen

int _tmain(int argc, _TCHAR* argv[])
{

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

	BSTR dataINodeName;
	BSTR dataINodeValue;

    VARIANT_BOOL status;
    VARIANT xmlSource;
    xmlSource.vt = VT_BSTR;
	xmlSource.bstrVal = SysAllocString (L"AAA.xml"); //	(L"Beispiel_10KV.xml");
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
											// Beispiel Wert "21" ist, überprüfe andere Bedingungen
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
																	// Verarbeite hier jeden Geschwisterknoten...
																	// Beispiel Wert von <Date> == 02/26/2019 10:57:29
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
																			IXMLDOMNode* pChildWithInterestingData;
																			IXMLDOMNodeList* pChildWithInterestingDatas;

																			hr = pNodeConditionOK->get_childNodes(&pChildWithInterestingDatas);

																			//while (pChildWithInterestingDatas->nextNode(&pChildWithInterestingData) == S_OK)
																			while (pSiblings->nextNode(&pChildWithInterestingData) == S_OK) {

																					//hr = pChildWithInterestingData->get_text(&dataINodeName); //wäre Wert
																				hr = pChildWithInterestingData->get_nodeName(&dataINodeName); //wäre Wert

																				if (SUCCEEDED(hr)  && wcscmp(dataINodeName, L"Results") == 0) //hier auch Results enthalten
																				{
																					std::wcout << "Der richtige Datensatz gefunden wenn es Results ist. __ Knotenname: " << " Wert: " << dataINodeName << std::endl;
																					//Hier nun Results_Item über die Kinder
																					IXMLDOMNode* pChildWithInterestingData_Results_Item = NULL;
																					IXMLDOMNodeList* pChildWithInterestingDatas_Results_Item = NULL;
																					hr = pChildWithInterestingData->get_childNodes(&pChildWithInterestingDatas_Results_Item);

																						while (pChildWithInterestingDatas_Results_Item->nextNode(&pChildWithInterestingData_Results_Item) == S_OK) {
																							//Erstmal alle Kinder
																							//std::wcout << "Der richtige Datensatz gefunden  __ Knotenname: " << " Wert: " << dataINodeName << std::endl;
																							hr = pChildWithInterestingData_Results_Item->get_nodeName(&dataINodeName);
																							std::wcout << "Alle Kinder von Results  __ Knotenname: " << " Wert: " << dataINodeName << std::endl;


																							//Hier nun Results_Item gefunden und nun über die Kinder von Results_Item
																							IXMLDOMNode* pChildWithInterestingData_Results_Item_Childs = NULL;
																							IXMLDOMNodeList* pChildWithInterestingDatas_Results_Item_Childs = NULL;
																							//hr = pChildWithInterestingData->get_childNodes(&pChildWithInterestingDatas_Results_Item_Childs);
																							hr = pChildWithInterestingData_Results_Item->get_childNodes(&pChildWithInterestingDatas_Results_Item_Childs);

																							//Hier endlich die Daten pro Messwert welcher aus mehreren Attributen besteht
																							while (pChildWithInterestingDatas_Results_Item_Childs->nextNode(&pChildWithInterestingData_Results_Item_Childs) == S_OK) {
																								//Erstmal alle Kinder
																								hr = pChildWithInterestingData_Results_Item_Childs->get_nodeName(&dataINodeName);
																								hr = pChildWithInterestingData_Results_Item_Childs->get_text(&dataINodeValue);

																								//std::wcout << "Alle Kinder von Results_Item  __ Knotenname: " << " Name: " << dataINodeName << "der Wert " << std::endl;
																								std::wcout << "Alle Kinder von Results_Item  __ Knotenname: " << " Name: " << dataINodeName << std::endl;

																								//Hier nun die Datenbeschreiber
																								IXMLDOMNode* pChildWithInterestingData_Results_Item_Childs_Describer = NULL;
																								IXMLDOMNodeList* pChildWithInterestingDatas_Results_Item_Childs_Describer = NULL;
																								hr = pChildWithInterestingData_Results_Item_Childs->get_childNodes(&pChildWithInterestingDatas_Results_Item_Childs_Describer);

																								while (pChildWithInterestingDatas_Results_Item_Childs_Describer->nextNode(&pChildWithInterestingData_Results_Item_Childs_Describer) == S_OK){
																									hr = pChildWithInterestingData_Results_Item_Childs_Describer->get_nodeName(&dataINodeName);
																									hr = pChildWithInterestingData_Results_Item_Childs_Describer->get_text(&dataINodeValue);

																									std::wcout << "Alle Kinder von Results_Item  __ Knotenname: " << " Name: " << dataINodeName << std::endl;

																									pChildWithInterestingData_Results_Item_Childs_Describer->Release();
																								}
																								pChildWithInterestingDatas_Results_Item_Childs_Describer->Release();
																								pChildWithInterestingData_Results_Item_Childs->Release();
																							}
																							pChildWithInterestingDatas_Results_Item_Childs->Release();
																							pChildWithInterestingData_Results_Item->Release();      //Prüfen

																						}
																						//break;
                                                                                }
                                                                                pChildWithInterestingData->Release();
																			}  //bis hier OK

                                                                            pChildWithInterestingDatas->Release();
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

			SysFreeString(dataINodeName);
			SysFreeString(dataINodeValue);
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




