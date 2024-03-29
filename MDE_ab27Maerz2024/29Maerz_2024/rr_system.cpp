
#include <stdexcept>
#include "rr_system.h"

//Class Pair2Strings
 XMLProcess::~XMLProcess()
 {
	ReleaseResources();
 }

 XMLProcess::XMLProcess()
 {
	pXMLDom = NULL;
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


