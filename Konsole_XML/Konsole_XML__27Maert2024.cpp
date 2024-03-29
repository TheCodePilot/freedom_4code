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

//problem: Kind von Name des Knotens ist der wertr !!!
//Bitte beachten es braucht den Nodetyp
//Ausserdem leider eigene Methode lastChildren implementieren
int _tmain(int argc, _TCHAR* argv[])
{
	IXMLDOMDocument* pXMLDom;
	HRESULT hr_com = CoInitialize(NULL);

	if (FAILED(hr_com)) {
			throw std::runtime_error("Fehler bei der Initialisierung der COM-Schnittstelle");
	}

	HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDom));
	if (FAILED(hr)) {
			CoUninitialize();
			throw std::runtime_error("Fehler beim Erstellen des XML-DOM-Objekts.");
	}

	VARIANT_BOOL status;
	VARIANT xmlSource;
	xmlSource.vt = VT_BSTR;
	xmlSource.bstrVal = SysAllocString(L"BeispielG.xml");   //1. Datei-Namen
	hr = pXMLDom->load(xmlSource, &status);
	SysFreeString(xmlSource.bstrVal);

	if (FAILED(hr) || status != VARIANT_TRUE) {
			pXMLDom = NULL;
			CoUninitialize();
			throw std::runtime_error("Fehler beim Laden der XML-Datei.");
	}

	IXMLDOMElement* pRootNode;

	HRESULT hrp = pXMLDom->get_documentElement(&pRootNode);
	if (FAILED(hrp) || !pRootNode) {
		throw std::runtime_error("Fehler beim Abrufen des Wurzelknotens.");
	}

	IXMLDOMNode* pDeptNode;
	IXMLDOMNode* pDeptInnerNode;
	IXMLDOMNode* pEmailNode;

	IXMLDOMNodeList *pDeptNodes;

	const wchar_t* xmlDataMitarbeiter = L"Mitarbeiter"; //2. eltern von darunter das ist sicher vielleicht geht auch darüber
	BSTR bstrXMLDataMitarbeiter = SysAllocString(xmlDataMitarbeiter);

	hr = pRootNode->getElementsByTagName(bstrXMLDataMitarbeiter,&pDeptNodes);
	if (FAILED(hr)) {
		throw std::runtime_error("Fehler beim Abrufen des TagNames.");
	}

	const wchar_t* xmlData = L"Name"; //ist OK Knotenname der helfen soll im Sibling-Bereich
	BSTR bstrXMLData = SysAllocString(xmlData);

	const wchar_t* xmlDataEmail = L"Email"; //ist OK  3. Was ich suche
	BSTR bstrXMLDataEmail = SysAllocString(xmlDataEmail);

	//In pDeptNodes ist die Liste aller Mitarbeiter-Knoten, also welche um die Daten der Arbeiter sich jeweils befindet
	while (pDeptNodes->nextNode(&pDeptNode) == S_OK)
	{
		pDeptNode->selectSingleNode(bstrXMLData, &pDeptInnerNode);
		//Nun schauen ob es der richtige Name ist, in XML als Text und es ist hier kein Attribut da das " = "..." " fehlt

		BSTR bstrAttributeValue;
		hr = pDeptInnerNode->get_text(&bstrAttributeValue);
		if (SUCCEEDED(hr)) {
			//std::wcout << "Attributwert: " << bstrAttributeValue << std::endl;   //z.b. Entwicklung
		}


		// Konvertieren von BSTR zu const wchar_t*

		const wchar_t* name = bstrAttributeValue;

		if (wcscmp(name, L"John Doe") == 0) //4. welche spezielle von Was. Die Email vom Knoten namen mit TEXT hier John Doe
		{
			pDeptNode->selectSingleNode(bstrXMLDataEmail, &pEmailNode);
			BSTR bstrAttributeValueEmail;

			hr = pEmailNode->get_text(&bstrAttributeValueEmail);
			if (SUCCEEDED(hr)) {
				std::wcout << "Attributwert: von Name: " << bstrAttributeValue << std::endl;   //z.b. Entwicklung
				std::wcout << "Attributwert: von Email: " << bstrAttributeValueEmail << std::endl;   //z.b. Entwicklung

			}

			SysFreeString(bstrAttributeValueEmail);
		}
		SysFreeString(bstrAttributeValue);

	}


	SysFreeString(bstrXMLData);
	SysFreeString(bstrXMLDataMitarbeiter);

	if(pDeptNodes)
	{
		pDeptNodes->Release();
		pDeptNodes = NULL;
	}

	if(pEmailNode)
	{
		pEmailNode->Release();
		pEmailNode = NULL;
	}

	//OK
	if(pDeptInnerNode)
	{
		pDeptInnerNode->Release();
		pDeptInnerNode = NULL;
	}

	if(pDeptNode)
	{
		pDeptNode->Release();
		pDeptNode = NULL;
	}

	if(pRootNode)
	{
		pRootNode->Release();
		pRootNode = NULL;
	}

	if (pXMLDom)
	{
		pXMLDom->Release();
		pXMLDom = NULL;
	}


	CoUninitialize();

	int a1;
	std::cin >> a1;
	return 0;
}

//Hier Wie lautet die email John Doe mit der Datei
//int _tmain_SuperOKGanzgenau(int argc, _TCHAR* argv[])
int _tmain42(int argc, _TCHAR* argv[])
{
	IXMLDOMDocument* pXMLDom;
	HRESULT hr_com = CoInitialize(NULL);

	if (FAILED(hr_com)) {
			throw std::runtime_error("Fehler bei der Initialisierung der COM-Schnittstelle");
	}

	HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDom));
	if (FAILED(hr)) {
			CoUninitialize();
			throw std::runtime_error("Fehler beim Erstellen des XML-DOM-Objekts.");
	}

	VARIANT_BOOL status;
	VARIANT xmlSource;
	xmlSource.vt = VT_BSTR;
	xmlSource.bstrVal = SysAllocString(L"BeispielG.xml");
	hr = pXMLDom->load(xmlSource, &status);
	SysFreeString(xmlSource.bstrVal);

	if (FAILED(hr) || status != VARIANT_TRUE) {
			pXMLDom = NULL;
			CoUninitialize();
			throw std::runtime_error("Fehler beim Laden der XML-Datei.");
	}

	IXMLDOMElement* pRootNode;

	HRESULT hrp = pXMLDom->get_documentElement(&pRootNode);
	if (FAILED(hrp) || !pRootNode) {
		throw std::runtime_error("Fehler beim Abrufen des Wurzelknotens.");
	}

	const wchar_t* xmlData = L"<Unternehmen></Unternehmen>";
	BSTR bstrXMLData = SysAllocString(xmlData);
	pXMLDom->loadXML(bstrXMLData,&status);

	IXMLDOMNode* pDeptNode;
	IXMLDOMNode* pDeptInnerNode;
	IXMLDOMNode* pEmailNode;

	IXMLDOMNodeList *pDeptNodes;

	const wchar_t* xmlDataMitarbeiter = L"Mitarbeiter"; //ist OK
	BSTR bstrXMLDataMitarbeiter = SysAllocString(xmlDataMitarbeiter);

	hr = pRootNode->getElementsByTagName(bstrXMLDataMitarbeiter,&pDeptNodes);
	if (FAILED(hr)) {
		throw std::runtime_error("Fehler beim Abrufen des TagNames.");
	}

	const wchar_t* xmlData2 = L"Name"; //ist OK
	BSTR bstrXMLData2 = SysAllocString(xmlData2);

	const wchar_t* xmlDataEmail = L"Email"; //ist OK
	BSTR bstrXMLDataEmail = SysAllocString(xmlDataEmail);


	//In pDeptNodes ist die Liste aller Mitarbeiter-Knoten, also welche um die Daten der Arbeiter sich jeweils befindet
	while (pDeptNodes->nextNode(&pDeptNode) == S_OK)
	{
		pDeptNode->selectSingleNode(bstrXMLData2, &pDeptInnerNode);
		//Nun schauen ob es der richtige Name ist, in XML als Text und es ist hier kein Attribut da das " = "..." " fehlt

		BSTR bstrAttributeValue;
		hr = pDeptInnerNode->get_text(&bstrAttributeValue);
		if (SUCCEEDED(hr)) {
			//std::wcout << "Attributwert: " << bstrAttributeValue << std::endl;   //z.b. Entwicklung
		}


		// Konvertieren von BSTR zu const wchar_t*

		const wchar_t* name = bstrAttributeValue;

		if (wcscmp(name, L"John Doe") == 0)
		{
			pDeptNode->selectSingleNode(bstrXMLDataEmail, &pEmailNode);
			BSTR bstrAttributeValueEmail;

			hr = pEmailNode->get_text(&bstrAttributeValueEmail);
			if (SUCCEEDED(hr)) {
				std::wcout << "Attributwert: von Name: " << bstrAttributeValue << std::endl;   //z.b. Entwicklung
				std::wcout << "Attributwert: von Email: " << bstrAttributeValueEmail << std::endl;   //z.b. Entwicklung

			}

			SysFreeString(bstrAttributeValueEmail);
		}
		SysFreeString(bstrAttributeValue);

	}

	SysFreeString(bstrXMLData);
	SysFreeString(bstrXMLData2);
	SysFreeString(bstrXMLDataMitarbeiter);

	if(pDeptNodes)
	{
		pDeptNodes->Release();
		pDeptNodes = NULL;
	}

	if(pEmailNode)
	{
		pEmailNode->Release();
		pEmailNode = NULL;
	}

	//OK
	if(pDeptInnerNode)
	{
		pDeptInnerNode->Release();
		pDeptInnerNode = NULL;
	}

	if(pDeptNode)
	{
		pDeptNode->Release();
		pDeptNode = NULL;
	}

	if(pRootNode)
	{
		pRootNode->Release();
		pRootNode = NULL;
	}

	if (pXMLDom)
	{
		pXMLDom->Release();
		pXMLDom = NULL;
	}


	CoUninitialize();

	int a1;
	std::cin >> a1;
	return 0;
}

int _tmain_OK(int argc, _TCHAR* argv[])
{
	IXMLDOMDocument* pXMLDom;
	HRESULT hr_com = CoInitialize(NULL);

	if (FAILED(hr_com)) {
			throw std::runtime_error("Fehler bei der Initialisierung der COM-Schnittstelle");
	}

	HRESULT hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDom));
	if (FAILED(hr)) {
			CoUninitialize();
			throw std::runtime_error("Fehler beim Erstellen des XML-DOM-Objekts.");
	}

	VARIANT_BOOL status;
	VARIANT xmlSource;
	xmlSource.vt = VT_BSTR;
	xmlSource.bstrVal = SysAllocString(L"BeispielG.xml");
	hr = pXMLDom->load(xmlSource, &status);
	SysFreeString(xmlSource.bstrVal);

	if (FAILED(hr) || status != VARIANT_TRUE) {
			pXMLDom = NULL;
			CoUninitialize();
			throw std::runtime_error("Fehler beim Laden der XML-Datei.");
	}

	IXMLDOMElement* pRootNode;

	HRESULT hrp = pXMLDom->get_documentElement(&pRootNode);
	if (FAILED(hrp) || !pRootNode) {
		throw std::runtime_error("Fehler beim Abrufen des Wurzelknotens.");
	}

	const wchar_t* xmlData = L"<Unternehmen></Unternehmen>";
	BSTR bstrXMLData = SysAllocString(xmlData);
	pXMLDom->loadXML(bstrXMLData,&status);

	//###################################################


	// Holen Sie sich alle Abteilung-Knoten
	IXMLDOMNodeList *pDeptNodes;
		//const wchar_t* xmlData1 = L"Mitarbeiter"; // dann wäre das 0, weiles nicht direkt mit root verbunden
	const wchar_t* xmlData1 = L"Abteilung"; //ist OK
	BSTR bstrXMLData1 = SysAllocString(xmlData1);
	hr = pRootNode->getElementsByTagName(bstrXMLData1,&pDeptNodes);
	if (FAILED(hr)) {
		throw std::runtime_error("Fehler beim Abrufen des TagNames.");
	}

	//bis hier OK

	IXMLDOMNode* pDeptNode;
	IXMLDOMNode* pDeptInnerNode;

	IXMLDOMNamedNodeMap* pAttributes;
	IXMLDOMNode* pAttribute;

	const wchar_t* xmlData2 = L"Email"; //ist OK
	BSTR bstrXMLData2 = SysAllocString(xmlData2);

	while (pDeptNodes->nextNode(&pDeptNode) == S_OK)
	{


		hr = pDeptNode->get_attributes(&pAttributes);
		if (FAILED(hr)) {
			throw std::runtime_error("Fehler beim Abrufen der Attribute.");
		}

		// Durchlaufen und Verarbeiten der Attribute
		long attributeCount = 0;
		hr = pAttributes->get_length(&attributeCount);
		if (FAILED(hr)) {
			std::cerr << "Fehler beim Abrufen der Anzahl der Attribute." << std::endl;
			pRootNode->Release();
			pDeptNodes->Release();
			pXMLDom->Release();
			CoUninitialize();
			return 1;
		}

	   std::cout << "Anzahl der Attribute: " << attributeCount << std::endl;


	   for (long i = 0; i < attributeCount; ++i) {

		hr = pAttributes->get_item(i, &pAttribute);
		if (FAILED(hr)) {
            std::cerr << "Fehler beim Abrufen des Attributes." << std::endl;
			continue;
		}

	   }
	   //bis hier OK
	   // Überprüfen Sie, ob das Attribut name "Entwicklung" ist
	   BSTR bstrAttributeName;
		hr = pAttribute->get_nodeName(&bstrAttributeName);
		if (SUCCEEDED(hr)) {
			std::wcout << "Attributname: " << bstrAttributeName << std::endl;
			SysFreeString(bstrAttributeName);
		}

		BSTR bstrAttributeValue;
		hr = pAttribute->get_text(&bstrAttributeValue);
		if (SUCCEEDED(hr)) {
			std::wcout << "Attributwert: " << bstrAttributeValue << std::endl;   //z.b. Entwicklung
			SysFreeString(bstrAttributeValue);
		}

		 // Konvertieren von BSTR zu const wchar_t*
		const wchar_t* name = bstrAttributeValue;

		if (wcscmp(name, L"Entwicklung") == 0) {
			// Holen Sie sich alle Email-Knoten in dieser Abteilung
			IXMLDOMElement* pDeptElement;
			hr = pDeptNode->QueryInterface(__uuidof(IXMLDOMElement), (void**)&pDeptElement);
			if (FAILED(hr)) {
				std::cerr << "Fehler beim Extrahieren des Elements aus dem Knoten." << std::endl;
				continue;
			}

			// Holen Sie sich alle Email-Knoten in dieser Abteilung
			IXMLDOMNodeList* pEmailNodes;
			hr = pDeptElement->getElementsByTagName(bstrXMLData2, &pEmailNodes);
			if (FAILED(hr)) {
				std::cerr << "Fehler beim Abrufen der Email-Knoten." << std::endl;
				continue;
			}

			// Durchlaufen Sie alle Email-Knoten und drucken Sie den Text aus
			IXMLDOMNode* pEmailNode;
			while (pEmailNodes->nextNode(&pDeptInnerNode) == S_OK) {
			//while (pDeptNodes->nextNode(&pDeptNode) == S_OK
				BSTR bstrEmailText;
				//hr = pEmailNode->
				hr = pDeptInnerNode->get_text(&bstrEmailText);
				if (FAILED(hr)) {
					std::cerr << "Fehler beim Abrufen des Texts des Email-Knotens." << std::endl;
					continue;
				}

				// Konvertieren von BSTR zu const wchar_t*
				const wchar_t* emailText = bstrEmailText;

				std::wcout << "Email: " << emailText << std::endl;

				// Freigabe des BSTR
				SysFreeString(bstrEmailText);
			}
		}

	}

	// Beenden Sie die COM-Bibliothek
	int a;
	std::cin >> a;

	SysFreeString(bstrXMLData);
	SysFreeString(bstrXMLData1);
	SysFreeString(bstrXMLData2);

	if(pAttribute)
	{
		pAttribute->Release();
		pAttribute = NULL;
	}

	if(pAttributes)
	{
		pAttributes->Release();
		pAttributes = NULL;
	}

	if(pRootNode)
	{
		pRootNode->Release();
		pRootNode = NULL;
	}

	if (pXMLDom)
	{
		pXMLDom->Release();
		pXMLDom = NULL;
	}

	CoUninitialize();

	int a1;
	std::cin >> a1;
	return 0;
	}


