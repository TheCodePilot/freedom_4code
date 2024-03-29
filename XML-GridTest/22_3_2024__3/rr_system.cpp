
#include <stdexcept>
#include "rr_system.h"
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

		std::pair<std::wstring, std::wstring> nodeInfo = getNodeInfo(pNode);  //auto von bcc32 nicht unterstützt :(

		// Extrahieren des Knotennamens und des Knotenwerts
		BSTR nodeName, nodeValue, nodeNameSib;
		pNode->get_nodeName(&nodeName);
		pNode->get_text(&nodeValue);

		// Hinzufügen der Knoteninformationen zum Vektor
		std::pair<std::wstring, std::wstring> info(nodeInfo.first, nodeValue);

			//std::wstring ws = info.first;
		std::string s( info.first.begin(), info.first.end() );
		std::string::size_type n, no, nsib;
		n = s.find("bstrName");//("bstrComment");

				//xmlInfo.push_back(info);    //hier OK

		//Auch schauen ob Sibling relevant
		IXMLDOMNode* pNextNode;
		//pNode->getNext(&pNextNode);
		pNode->get_nextSibling(&pNextNode);
		if(pNextNode)
		{
			pNextNode->get_nodeName(&nodeNameSib);

			//Konvertierung
			std::wstring nodeNametemp;//, nodeValuetemp;
			std::wstring wssib = nodeNameSib;

			nodeNametemp = std::wstring(nodeNameSib, SysStringLen(nodeNameSib));
			std::string ssib(nodeNametemp.begin(), nodeNametemp.end() );

			nsib = ssib.find("Date");//("bstrComment");
			no = ssib.find("Time");
			if((n != std::string::npos) && (nsib != std::string::npos) && (no == std::string::npos))

			{
			// Hinzufügen der Knoteninformationen zum Vektor
			std::pair<std::wstring, std::wstring> info(nodeInfo.first, nodeValue);
			xmlInfo.push_back(info);
			}
			SysFreeString(nodeNameSib);

		}
		//xmlInfo.push_back(info);

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
std::pair<std::wstring, std::wstring> XMLProcess::getNodeInfo(IXMLDOMNode* pNode) const {

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




