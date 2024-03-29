
#include <stdexcept>
#include "rr_system.h"
#include <string>

//Class Pair2Strings
 XMLProcess::~XMLProcess()
 {
	ReleaseResources();
 }

 XMLProcess::XMLProcess()
 {
	pXMLDom = NULL;
	findstart = false;
	tagCounter = 0;
 }

 void XMLProcess::ReleaseResources()
 {
			if (pXMLDom) {
				pXMLDom->Release();
				pXMLDom = NULL;
			}

			CoUninitialize();
 }

 void XMLProcess::LoadXMLDocument(const std::wstring& filename) {

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
		xmlSource.bstrVal = SysAllocString(filename.c_str());
		hr = pXMLDom->load(xmlSource, &status);
		SysFreeString(xmlSource.bstrVal);

		if (FAILED(hr) || status != VARIANT_TRUE) {
			pXMLDom = NULL;
			CoUninitialize();
			throw std::runtime_error("Fehler beim Laden der XML-Datei.");
		}
}

void XMLProcess::ProcessNodePlus(IXMLDOMNode* pNode)
{
		if (pNode == NULL) {
			return;
		}

		std::pair<std::wstring, std::wstring> nodeInfo = GetNodeInfo(pNode);  //auto von bcc32 nicht unterstützt :(

		// Extrahieren des Knotennamens und des Knotenwerts
		BSTR nodeName, nodeValue, nodeNameSib, sibValue;
		pNode->get_nodeName(&nodeName);
		pNode->get_text(&nodeValue);

		// Hinzufügen der Knoteninformationen zum Vektor
		//Zunächst ist nodeInfo.first für Suche wichtig
		std::pair<std::wstring, std::wstring> info(nodeInfo.first, nodeValue);

		std::string s( info.first.begin(), info.first.end() );
		std::string::size_type n, no, nsib;
		n = s.find("bstrName");

		//Auch schauen ob Sibling relevant
		IXMLDOMNode* pNextNode;
		pNode->get_nextSibling(&pNextNode);
		if(pNextNode)
		{
			pNextNode->get_nodeName(&nodeNameSib);
			pNextNode->get_text(&sibValue);
			//Konvertierung
			std::wstring nodeNametemp;
			std::wstring wssib = nodeNameSib;

			nodeNametemp = std::wstring(nodeNameSib, SysStringLen(nodeNameSib));
			std::string ssib(nodeNametemp.begin(), nodeNametemp.end() );

			nsib = ssib.find("Date");
			no = ssib.find("Time");
			if((n != std::string::npos) && (nsib != std::string::npos) && (no == std::string::npos))
			{
			// Hinzufügen der Knoteninformationen zum Vektor hier aber nun für den Menschen interassenten Values
			std::pair<std::wstring, std::wstring> info(nodeValue, sibValue);

			xmlInfo.push_back(info);
			//Nun die eigentlichen werte in den Hilfsvektor
			}
			SysFreeString(nodeNameSib);
			SysFreeString(sibValue);
		}

		// Freigeben der Speicherressourcen
		SysFreeString(nodeName);
		SysFreeString(nodeValue);

		// Verarbeitung der Kindknoten
		IXMLDOMNodeList* pChildren;
		pNode->get_childNodes(&pChildren);
		long length;
		pChildren->get_length(&length);

		for (long i = 0; i < length; i++) {
			IXMLDOMNode* pChild;
			pChildren->get_item(i, &pChild);
			ProcessNodePlus(pChild);
		}
}

//Test wenn eine Echte Messreihe gefunden
void XMLProcess::ProcessNodePlusSpezial(IXMLDOMNode* pNode)
{
		if (pNode == NULL) {
			return;
		}

		std::pair<std::wstring, std::wstring> nodeInfo = GetNodeInfo(pNode);  //auto von bcc32 nicht unterstützt :(

		// Extrahieren des Knotennamens und des Knotenwerts
		BSTR nodeName, nodeValue, nodeNameSib, sibValue;
		pNode->get_nodeName(&nodeName);
		pNode->get_text(&nodeValue);

		// Hinzufügen der Knoteninformationen zum Vektor
		//Zunächst ist nodeInfo.first für Suche wichtig
		std::pair<std::wstring, std::wstring> info(nodeInfo.first, nodeValue);

		std::string s( info.first.begin(), info.first.end() );
		std::string::size_type n, no, nsib;
		n = s.find("Output");     //Achtung später noch wenn nicht auch
		if(n != std::string::npos){
			findstart = true;
		}

		if((n != std::string::npos)||findstart == true)
		{
		//Auch schauen ob Sibling relevant
		IXMLDOMNode* pNextNode;
		pNode->get_nextSibling(&pNextNode);
		if(pNextNode)
		{
			pNextNode->get_nodeName(&nodeNameSib);
			pNextNode->get_text(&sibValue);
			//Konvertierung
			std::wstring nodeNametemp;
			std::wstring wssib = nodeNameSib;

			nodeNametemp = std::wstring(nodeNameSib, SysStringLen(nodeNameSib));
			std::string ssib(nodeNametemp.begin(), nodeNametemp.end() );

			nsib = ssib.find("Date");
			no = ssib.find("Time");
			if(AcceptValues(pNextNode))  //RRR25Maerz  wieder rein

			{
			// Hinzufügen der Knoteninformationen zum Vektor hier aber nun für den Menschen interassenten Values
				//std::pair<std::wstring, std::wstring> info(nodeValue, sibValue);    //RRR25Maerz  wieder rein
			std::pair<std::wstring, std::wstring> info(nodeName, nodeValue);  //Für andere Sizurtiom

			xmlInfo.push_back(info);
			//Nun die eigentlichen werte in den Hilfsvektor
			}
			SysFreeString(nodeNameSib);
			SysFreeString(sibValue);
		}

		// Freigeben der Speicherressourcen
		SysFreeString(nodeName);
		SysFreeString(nodeValue);
		 }
		// Verarbeitung der Kindknoten
		IXMLDOMNodeList* pChildren;
		pNode->get_childNodes(&pChildren);
		long length;
		pChildren->get_length(&length);

		for (long i = 0; i < length; i++) {
			IXMLDOMNode* pChild;
			pChildren->get_item(i, &pChild);
			ProcessNodePlusSpezial(pChild);
		}
}



//Ein Paar-Wert zurück
std::pair<std::wstring, std::wstring> XMLProcess::GetNodeInfo(IXMLDOMNode* pNode) const {

	std::wstring nodeName, nodeValue;
	nodeName = L"";
	nodeValue = L"";

	if (pNode != NULL)
	{
		BSTR bstrNodeName, bstrNodeValue;
		pNode->get_nodeName(&bstrNodeName);
		pNode->get_text(&bstrNodeValue);
		if(bstrNodeName)
		{
			nodeName = std::wstring(bstrNodeName, SysStringLen(bstrNodeName));
		}

		if(bstrNodeValue)
		{
			nodeValue = std::wstring(bstrNodeValue, SysStringLen(bstrNodeValue));
		}

		SysFreeString(bstrNodeName);
		SysFreeString(bstrNodeValue);
	}
	std::pair<std::wstring, std::wstring> temp;
	temp.first = nodeName;
	temp.second = nodeValue;
	return temp;
}
//=============================================================================================================================================
//Hilfsfunktion erstmal einfach oder
//=============================================================================================================================================
bool XMLProcess::AcceptValues(IXMLDOMNode* pNode)
{
		if (pNode == NULL) {
			return false;
		}

		bool returner = false;
		std::pair<std::wstring, std::wstring> nodeInfo = GetNodeInfo(pNode);  //auto von bcc32 nicht unterstützt :(

		// Extrahieren des Knotennamens und des Knotenwerts
		BSTR nodeName, nodeValue; //, nodeNameSib, sibValue;
		pNode->get_nodeName(&nodeName);
		pNode->get_text(&nodeValue);

		// Hinzufügen der Knoteninformationen zum Vektor
		//Zunächst ist nodeInfo.first für Suche wichtig
		std::pair<std::wstring, std::wstring> info(nodeInfo.first, nodeValue);

		std::string s( info.first.begin(), info.first.end() );
		std::string::size_type n; //, no, nsib;
			//n = s.find("Output");     //Achtung später noch wenn nicht auch

		//Auch schauen ob Sibling relevant
		//IXMLDOMNode* pNextNode;
		//pNode->get_nextSibling(&pNextNode);
		//if(pNextNode)
		{
			//pNextNode->get_nodeName(&nodeNameSib);
			//pNextNode->get_text(&sibValue);
			//Konvertierung
			//std::wstring nodeNametemp;
			//std::wstring wssib = nodeNameSib;

			//nodeNametemp = std::wstring(nodeNameSib, SysStringLen(nodeNameSib));
			//std::string ssib(nodeNametemp.begin(), nodeNametemp.end() );

			//nsib = ssib.find("Date");
			//no = ssib.find("Time");

			std::string str1 = "Vx";
			std::string str2 = "Vxd";

		if( /*(s.find("Output")!= std::string::npos) || */(s.find("Vx")!= std::string::npos) /*(s.compare(str2) == 0) */|| (s.find("Ix")!= std::string::npos) )  //RRR25Maerz  wieder rein
			//if (str1.compare(s) == 0)
			{
			// Hinzufügen der Knoteninformationen zum Vektor hier aber nun für den Menschen interassenten Values
			//Nun die eigentlichen werte in den Hilfsvektor
				returner = true;
			}
			//SysFreeString(nodeNameSib);
			//SysFreeString(sibValue);
		}

		// Freigeben der Speicherressourcen
		SysFreeString(nodeName);
		SysFreeString(nodeValue);
		return returner;

}

void XMLProcess::getNodeInfoByTagName(IXMLDOMNode* pNode, const std::wstring& tagName) {
		BSTR nodeName;
		pNode->get_nodeName(&nodeName);
		std::wstring wnodeName(nodeName);
		SysFreeString(nodeName);

		if (wnodeName == tagName) {
			// Die Bezeichnung des aktuellen Knotens entspricht der Ziel-Zeichenkette
			processNode(pNode); //achtung sonst hier
		}

		// Rekursiv die Kinderknoten durchlaufen
		IXMLDOMNode* pChildNode;
		pNode->get_firstChild(&pChildNode);
		while (pChildNode) {
			getNodeInfoByTagName(pChildNode, tagName);
			IXMLDOMNode* pNextNode;
			pChildNode->get_nextSibling(&pNextNode);
			pChildNode->Release();
			pChildNode = pNextNode;
		}

}

 void XMLProcess::processNode(IXMLDOMNode* pNode) {
		if (pNode == NULL) {
			return;
		}

		//Hier wird gefüllt das  info
		std::pair<std::wstring, std::wstring> info = GetNodeInfo(pNode);
		xmlInfo.push_back(info);
		//std::wcout << L"Knotentyp: " << nodeInfo.first << L", Hat Kinder: " << (nodeInfo.second ? L"Ja" : L"Nein") << std::endl;

		IXMLDOMNodeList* pChildren;
		pNode->get_childNodes(&pChildren);
		long length;
		pChildren->get_length(&length);

		for (long i = 0; i < length; i++) {
			IXMLDOMNode* pChild;
			pChildren->get_item(i, &pChild);
			processNode(pChild);
		}
	}

//Vom Root aus
void XMLProcess::getNodeInfoByTagNameNativ(IXMLDOMNode* pNode, const std::wstring& tagName)
{
	BSTR nodeName;
	pNode->get_nodeName(&nodeName);
	std::wstring wnodeName(nodeName);
	SysFreeString(nodeName);

	if (/*(wnodeName == tagName)*/(tagCounter>0)&&(tagCounter<2))
		{
			std::string s( wnodeName.begin(), wnodeName.end() );
			std::string::size_type n, no, nsib;
			n = s.find("#text");
			if( (n == std::string::npos)||(s.find_first_of("#") == std::string::npos) )
				processNodeNativ(pNode); //achtung sonst hier
		}

	if (wnodeName == tagName)
		tagCounter++;


	//if (/*(wnodeName == tagName)*/(tagCounter>0)&&(tagCounter<2))
	{

		/*
		// Rekursiv die Kinderknoten durchlaufen
		IXMLDOMNode* pChildNode;
		pNode->   >get_firstChild(&pChildNode);
		while (pChildNode) {                                //hier schon schauen
			getNodeInfoByTagNameNativ(pChildNode, tagName);
			IXMLDOMNode* pNextNode;
			pChildNode->Sibling(&pNextNode);
			pChildNode->Release();
			pChildNode = pNextNode;
		}
		*/
	 }
}

void XMLProcess::processNodeNativ(IXMLDOMNode* pNode) {
		if (pNode == NULL) {
			return;
		}

		//Hier wird gefüllt das info der Form name, value
		std::pair<std::wstring, std::wstring> info = GetNodeInfo(pNode);
		xmlInfo.push_back(info);
		//std::wcout << L"Knotentyp: " << nodeInfo.first << L", Hat Kinder: " << (nodeInfo.second ? L"Ja" : L"Nein") << std::endl;

		IXMLDOMNodeList* pChildren;
		pNode->get_childNodes(&pChildren);
		long length;
		pChildren->get_length(&length);

		for (long i = 0; i < length; i++) {
			IXMLDOMNode* pChild;
			pChildren->get_item(i, &pChild);
			processNodeNativ(pChild);
		}
	}
//=============================================================================================================================================
//Class Pair2Strings //Hilfsklasse
//=============================================================================================================================================
	Pair2Strings::Pair2Strings()
	{

	}

	Pair2Strings::~Pair2Strings()
	{

	}

	std::pair<std::wstring, std::wstring> Pair2Strings::GetThePair(int Index)const
	{
		const std::pair < std::wstring, std::wstring>& pair = (xmlNodePair)[Index];
		return pair;
	}

	void Pair2Strings::SetThePair(std::pair < std::wstring, std::wstring >  xmlNodePairIn) //einfach nexte füllen ohne Index
	{
		//const std::pair < std::wstring, std::wstring>& pair = (xmlNodePair)[i];
		xmlNodePair.push_back(xmlNodePairIn);
	}


