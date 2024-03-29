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
//Verssuch mit DOMNodeType nodeType;
//sei gesucht Werte wo:
// Wert von <bstrName> == C1 (f)
//Wert von <Date> == 02 / 26 / 2019 10:57 : 29
//main_bevorobigesKriterium

//pRoot bitte sauber verwalten  -> Über Klasse
//breaks nutzen

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
	BSTR dataHelp;


	VARIANT_BOOL status;
	VARIANT xmlSource;
	xmlSource.vt = VT_BSTR;
	xmlSource.bstrVal = SysAllocString (L"AAA.xml"); //	(L"Beispiel_10KV.xml");
	hr = pXMLDom->load(xmlSource, &status);
	if (FAILED(hr)) {
		std::cout << "Fehler beim Laden der XML-Datei." << std::endl;
		return hr;
	}
	SysFreeString(xmlSource.bstrVal);

	if (status == VARIANT_TRUE) {
		IXMLDOMElement* pRootNode = NULL;
		hr = pXMLDom->get_documentElement(&pRootNode);
		if (SUCCEEDED(hr) && pRootNode) {
			IXMLDOMNodeList* pObjects = NULL;  //DEKLARATION: Alle Kinder von pRootNode
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

			IXMLDOMNode* pCardsNode = NULL;
			IXMLDOMNode* pNodeRoot = NULL;

            HRESULT hr_loop = S_OK; // Variable zur Überwachung des Schleifenstatus
			while (pObjects->nextNode(&pNodeRoot) == S_OK)  //SCHLEIFE: Solange Root Kinder hat
			{
                if (hr_loop != S_OK) {
                    // Wenn nextNode kein weiteres Element zurückgibt, brechen Sie die Schleife ab
                    break;
                }

				hr = pNodeRoot->get_nodeName(&dataINodeName);
				if (SUCCEEDED(hr) && dataINodeName && wcscmp(dataINodeName, L"Cards") == 0) {
					// Wenn der Knoten "Cards" ist, speichern Sie ihn und brechen Sie die Schleife ab
					pCardsNode = pNodeRoot;
					break;
				}

            }

			pObjects->Release(); // Freigeben des Speichers für die Knotenliste

			if (pCardsNode) {
                // Hier haben Sie den Knoten "Cards", von dem aus Sie die Kinder abrufen können
                // Fügen Sie hier Ihren Code hinzu, um die Kinder des "Cards"-Knotens zu verarbeiten
				IXMLDOMNodeList* pChildrenOfCards = NULL;
				hr = pCardsNode->get_childNodes(&pChildrenOfCards); //hier nun Cards_Item erhalten
				if (SUCCEEDED(hr)) {
					IXMLDOMNode* pChildNodeOfCards = NULL;
                    while (pChildrenOfCards->nextNode(&pChildNodeOfCards) == S_OK) {
                        // Verarbeiten Sie hier die Kinder des "Cards"-Knotens
						hr = pChildNodeOfCards->get_nodeName(&dataINodeName);
						if (SUCCEEDED(hr)) {
								std::wcout << "Kinder Von Cards  __ Knotenname: " << dataINodeName << std::endl; //", Wert: " << value << std::endl;
						}
							//Nun Da hier alle Kindsknoten nun hier erscheinen
							IXMLDOMNodeList* pChildrenChildrenOfCards = NULL; //Für die Kinder von Card_Item
							IXMLDOMNode* pChildChildNodeOfCards = NULL;
							hr = pChildNodeOfCards->get_childNodes(&pChildrenChildrenOfCards);
							if (SUCCEEDED(hr)) {
								while (pChildrenChildrenOfCards->nextNode(&pChildChildNodeOfCards) == S_OK) {

									hr = pChildChildNodeOfCards->get_nodeName(&dataINodeName);
									if (SUCCEEDED(hr) && wcscmp(dataINodeName, L"nType") == 0) {                                                //hier neu | && wcscmp(childChildNodeName, L"nType") == 0)
										hr = pChildChildNodeOfCards->get_text(&dataINodeValue);
										if (SUCCEEDED(hr) && wcscmp(dataINodeValue, L"21") == 0) {
											// Beispiel Wert "21" ist, überprüfe andere Bedingungen
											IXMLDOMNode* pSiblingNode = NULL;
											hr = pChildChildNodeOfCards->get_nextSibling(&pSiblingNode);
											if (SUCCEEDED(hr))
											{
												// Wert von <bstrName> == C1 (f)
												hr = pSiblingNode->get_nodeName(&dataINodeName);
												if (SUCCEEDED(hr) && wcscmp(dataINodeName, L"bstrName") == 0)
												{
													hr = pSiblingNode->get_text(&dataINodeValue);
													if (SUCCEEDED(hr) && wcscmp(dataINodeValue, L"C1 (f)") == 0)
                                                    {
														//erneut Schleife aller siblings
														IXMLDOMNode* pParentNode;
														hr = pSiblingNode->get_parentNode(&pParentNode);    //dort wo Bedingung erfüllt schauen wer ist Parent
														if (SUCCEEDED(hr) && pParentNode) {                 // RRR3: Noch tzesten wenn gleicher Name //RRR4: was wenn genau gleich sehr theoretisch da sekundengenau?
                                                            IXMLDOMNodeList* pSiblings;
															hr = pParentNode->get_childNodes(&pSiblings);
                                                            if (SUCCEEDED(hr) && pSiblings) {
                                                                IXMLDOMNode* pSibling;
																while (pSiblings->nextNode(&pSibling) == S_OK) {
																	// Verarbeite hier jeden Geschwisterknoten...
																	// Beispiel Wert von <Date> == 02/26/2019 10:57:29
																	hr = pSibling->get_nodeName(&dataINodeName);
																	if (SUCCEEDED(hr) && wcscmp(dataINodeName, L"Date") == 0)
																	{
																		hr = pSibling->get_text(&dataINodeValue);
																		if (SUCCEEDED(hr) && wcscmp(dataINodeValue, L"02/26/2019 10:57:29") == 0)
																		{
																			std::wcout << "Der richtige Cards_Item gefunden  __ Knotenname: " <<  " Wert: " << dataINodeValue << std::endl;
																			//Nun mit den richtigen Cards_Item sind dier Werte in Greifbarer Nähe .
																			IXMLDOMNode* pChildWithInterestingData;
																			
																			while (pSiblings->nextNode(&pChildWithInterestingData) == S_OK) {

																				hr = pChildWithInterestingData->get_nodeName(&dataINodeName); //wäre Wert
																				if (SUCCEEDED(hr)  && wcscmp(dataINodeName, L"Results") == 0) //hier auch Results enthalten
																				{
																					std::wcout << "Der richtige Datensatz gefunden wenn es Results ist. __ Knotenname: " << " Wert: " << dataINodeName << std::endl;
																					//Hier nun Results_Item über die Kinder
																					IXMLDOMNode* pChildWithInterestingData_Results_Item = NULL;
																					IXMLDOMNodeList* pChildWithInterestingDatas_Results_Item = NULL;
																					hr = pChildWithInterestingData->get_childNodes(&pChildWithInterestingDatas_Results_Item);
																					if (FAILED(hr)) {
																						std::cout << "Fehler beim Bekommen der Kinder" << std::endl;
																						return hr;
																						}
																						while (pChildWithInterestingDatas_Results_Item->nextNode(&pChildWithInterestingData_Results_Item) == S_OK) {
																							//Erstmal alle Kinder
																							hr = pChildWithInterestingData_Results_Item->get_nodeName(&dataINodeName);
																							if (FAILED(hr)) {
																								std::cout << "Fehler beim Bekommen des Knotennamens" << std::endl;
																								return hr;
																							}
																							std::wcout << "Alle Kinder von Results  __ Knotenname: " << " Wert: " << dataINodeName << std::endl;

																							//Hier nun Results_Item gefunden und nun über die Kinder von Results_Item
																							IXMLDOMNode* pChildWithInterestingData_Results_Item_Childs = NULL;
																							IXMLDOMNodeList* pChildWithInterestingDatas_Results_Item_Childs = NULL;
																							hr = pChildWithInterestingData_Results_Item->get_childNodes(&pChildWithInterestingDatas_Results_Item_Childs);
																							if (FAILED(hr)) {
																								std::cout << "Fehler beim Bekommen der Kinder" << std::endl;
																								return hr;
																							}
																							//Hier endlich die Daten pro Messwert welcher aus mehreren Attributen besteht
																							while (pChildWithInterestingDatas_Results_Item_Childs->nextNode(&pChildWithInterestingData_Results_Item_Childs) == S_OK) {
																								//Erstmal alle Kinder
																								hr = pChildWithInterestingData_Results_Item_Childs->get_nodeName(&dataINodeName);
																								if (FAILED(hr)) {
																									std::cout << "Fehler beim Bekommen des Knotennamens" << std::endl;
																									return hr;
																								}
																								hr = pChildWithInterestingData_Results_Item_Childs->get_text(&dataINodeValue);
																								if (FAILED(hr)) {
																									std::cout << "Fehler beim Bekommen des Knotenwerts" << std::endl;
																									return hr;
																								}
																								std::wcout << "Alle Kinder von Results_Item  __ Knotenname: " << " Name: " << dataINodeName << std::endl;

																								//Hier nun die Datenbeschreiber
																								IXMLDOMNode* pChildWithInterestingData_Results_Item_Childs_Describer = NULL;
																								IXMLDOMNodeList* pChildWithInterestingDatas_Results_Item_Childs_Describer = NULL;
																								hr = pChildWithInterestingData_Results_Item_Childs->get_childNodes(&pChildWithInterestingDatas_Results_Item_Childs_Describer);

																								while (pChildWithInterestingDatas_Results_Item_Childs_Describer->nextNode(&pChildWithInterestingData_Results_Item_Childs_Describer) == S_OK){
																									hr = pChildWithInterestingData_Results_Item_Childs_Describer->get_nodeName(&dataINodeName);
                                                                                                    if (FAILED(hr)) {
																									std::cout << "Fehler beim Bekommen des Knotennamens" << std::endl;
																									return hr;
																									}
																									hr = pChildWithInterestingData_Results_Item_Childs_Describer->get_text(&dataINodeValue);
																									if (FAILED(hr)) {
																									std::cout << "Fehler beim Bekommen des Knotenwerts" << std::endl;
																									return hr;
																									}

																									//hier die Probe gemacht .
																									//hr = pChildWithInterestingData->get_nodeName(&dataINodeName); //wäre Wert
																									hr = pChildWithInterestingData_Results_Item_Childs_Describer->get_nodeName(&dataINodeName); //wäre Wert
																									pChildWithInterestingData_Results_Item_Childs->get_nodeName(&dataHelp); //Vom Parent
																									if ( SUCCEEDED(hr)&&(wcscmp(dataINodeName, L"dValue")) == 0)	//&&(wcscmp(dataHelp, L"Vx") == 0) )       //RRR6
																									{
																										std::wcout << "__________Alle Kinder von dem jeweiligen Daten  __ Knotenname: " << " Name: " << dataINodeName << std::endl;
																											pChildWithInterestingData_Results_Item_Childs_Describer->Release();  //geprueft //RRR6
																											break;     //RRR6
																									}
																									pChildWithInterestingData_Results_Item_Childs_Describer->Release();
																									//danach erst Release
																								}  //ENDE while (pChildWithInterestingDatas_Results_Item_Childs_Describer->nextNode(&pChildWithInterestingData_Results_Item_Childs_Describer)
																								pChildWithInterestingDatas_Results_Item_Childs_Describer->Release();  //geprueft
																								pChildWithInterestingData_Results_Item_Childs->Release();   //geprueft
																							}  //ENDE while (pChildWithInterestingDatas_Results_Item_Childs->nextNode(&pChildWithInterestingData_Results_Item_Childs)
																							pChildWithInterestingDatas_Results_Item_Childs->Release();
																							pChildWithInterestingData_Results_Item->Release();  //geprueft

																						}  //ENDE while (pChildWithInterestingDatas_Results_Item->nextNode(&pChildWithInterestingData_Results_Item)
																						//break;
                                                                                }
																				pChildWithInterestingData->Release();   //geprueft
																			}  //ENDE while (pSiblings->nextNode(&pChildWithInterestingData)

																		}

                                                                    }

																	pSibling->Release(); // Freigeben des Speichers  //geprueft
																} //ENDE while (pSiblings->nextNode(&pSibling)
																pSiblings->Release(); // Freigeben des Speichers  //geprueft
															}
															pParentNode->Release(); // Freigeben des Speichers  //geprueft
                                                        }
														pSiblingNode->Release();    //RRR5  //geprueft
                                                    }

												}

                                            }
											break;  // Speicher freigeben             //RRR2
										}

									}
									//SysFreeString(childChildNodeName);
									pChildChildNodeOfCards->Release(); // Freigeben des Speichers für den Kindknoten  //geprueft
								} //ENDE while (pChildrenChildrenOfCards->nextNode(&pChildChildNodeOfCards
								pChildrenChildrenOfCards->Release(); // Freigeben des Speichers für die Liste der Kinder //geprueft
							}

						pChildNodeOfCards->Release(); // Freigeben des Speichers für den Kindknoten  //geprueft
						} //ENDE while (pChildrenOfCards->nextNode(&pChildNodeOfCards) == S_OK)
					pChildrenOfCards->Release(); // Freigeben des Speichers für die Liste der Kinder //geprueft
				}
				pCardsNode->Release(); // Freigeben des Speichers für den "Cards"-Knoten //geprueft
			}
            else {
                std::cerr << "Der Knoten 'Cards' wurde nicht gefunden." << std::endl;
            }

			SysFreeString(dataINodeName);
			SysFreeString(dataINodeValue);
			SysFreeString(dataHelp);

			pRootNode->Release();
			pXMLDom->Release(); // Freigeben des Speichers
			CoUninitialize();

			std::cout << "Drücken Sie eine beliebige Taste, um fortzufahren...";
			std::cin.get();
			return 0;
		}
    }
}




