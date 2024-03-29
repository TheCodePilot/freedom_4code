
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

 void XMLProcess::ProcessXMLDocument() {
		if (!pXMLDom) {
			throw std::runtime_error("XML-Dokument wurde nicht geladen.");
		}

		IXMLDOMElement* pRootNode;
		HRESULT hr = pXMLDom->get_documentElement(&pRootNode);
		if (FAILED(hr) || !pRootNode) {
			throw std::runtime_error("Fehler beim Abrufen des Wurzelknotens.");
		}

		// Verarbeitungsfunktionen aufrufen
		// Beispiel: processNodePlus(pRootNode);

		pRootNode->Release();
 }

void XMLProcess::LoadXMLDocument(const std::wstring& filename) {

		HRESULT hr_com = CoInitialize(NULL);
		if (FAILED(hr_com)) {
			throw std::runtime_error("Fehler bei der Initialisierung der COM-Schnittstelle");
		}
			//IXMLDOMDocument* pXMLDom;
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
		BSTR nodeName, nodeValue;
		pNode->get_nodeName(&nodeName);
		pNode->get_text(&nodeValue);

		// Hinzufügen der Knoteninformationen zum Vektor
		std::pair<std::wstring, std::wstring> info(nodeInfo.first, nodeValue);
		xmlInfo.push_back(info);

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



std::pair<std::wstring, std::wstring> XMLProcess::getNodeInfo(IXMLDOMNode* pNode) const {
    std::wstring nodeName, nodeValue;
	if (pNode != NULL) {
        BSTR bstrNodeName, bstrNodeValue;
        pNode->get_nodeName(&bstrNodeName);
		pNode->get_text(&bstrNodeValue);
		if(bstrNodeName)
		{
			nodeName = std::wstring(bstrNodeName, SysStringLen(bstrNodeName));
		}
		else
		{
			nodeName = L"";
		}
		//nodeName = bstrNodeName ? std::wstring(bstrNodeName, SysStringLen(bstrNodeName)) : L"";   bcc32 mag nicht :(

		if(bstrNodeValue)
		{
			nodeValue = std::wstring(bstrNodeValue, SysStringLen(bstrNodeValue));
		}
		else
		{
			nodeValue = L"";
		}
		//nodeValue = bstrNodeValue ? std::wstring(bstrNodeValue, SysStringLen(bstrNodeValue)) : L"";   bcc32 mag nicht :(

		SysFreeString(bstrNodeName);
		SysFreeString(bstrNodeValue);
    }
	std::pair<std::wstring, std::wstring> temp;
	temp.first = nodeName;
	temp.second = nodeValue;
	return temp;
}



/*
std::pair<std::wstring, std::wstring> XMLProcess::getNodeInfo(int index) const {
		if (index >= 0 && index < xmlInfo.size()) {
			return xmlInfo[index];
		} else {
			std::pair<std::wstring, std::wstring> temp;
			temp.first = L"";
			temp.second = L"";
			return temp; // Rückgabe eines leeren Paares, wenn der Index ungültig ist
		}
	}
*/




