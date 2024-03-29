#ifndef RRTOOLS_H
#define RRTOOLS_H

#include <Windows.h> //BSTR
#include <iostream>
#include <map>
#include <string>
#include <vector>
//#include <comdef.h>  // _bstr_t
#include <msxml6.h>  // IXMLDOMNode
#pragma comment(lib, "msxml6.lib")

#include <memory>

//bcc32 akzeptiert kein nullptr, kein shared_ptr, keine Initialliste bei Konstruktoren, kein auto
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

            std::pair<std::wstring, bool> nodeinfotemp;
			nodeinfotemp.first = L"";
			nodeinfotemp.second = hasChild;
			return nodeinfotemp;
				//return { nodeType, hasChild }; wird von bcc32 nicht unterstützt
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

        // �berpr�fen, ob der Knoten Kinder hat
        IXMLDOMNodeList* pChildren;
        pNode->get_childNodes(&pChildren);
        long length;
        pChildren->get_length(&length);
        hasChild = (length > 0);
		std::pair<std::wstring, bool> nodeinfotemp;
		nodeinfotemp.first = nodeType;
		nodeinfotemp.second = hasChild;
		return nodeinfotemp; // { nodeType, hasChild };
	}

    static void processNode(IXMLDOMNode* pNode) {
        if (pNode == NULL) {
            return;
        }

		std::pair<std::wstring, bool> nodeInfo = RRTools::getNodeInfo(pNode);
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

		std::pair<std::wstring, bool> nodeInfo = RRTools::getNodeInfo(pNode);

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
            // F�hren Sie hier die gew�nschten Aktionen aus, z. B. den aktuellen Knoten verarbeiten
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


}// namespace RRTools

//HINEINNNNNNNNNNNNNNNNNNNNNNNNNN
	//class 4 XML-Handling
	class XMLProcess {

	public:
		XMLProcess();
		~XMLProcess();

		/*HRESULT InitializeCOM() {
			return CoInitialize(nullptr);
		}*/

		int getNodeCount() const {
			return xmlInfo.size();
		}

		void LoadXMLDocument(const std::wstring& filename);

		void ProcessNodePlus(IXMLDOMNode* pNode);
		void ProcessXMLDocument();

		const std::vector<std::pair<std::wstring, std::wstring> >& getXMLInfo() const {
			return xmlInfo;
		}

		std::vector < std::pair < std::wstring, std::wstring > > xmlInfo;
		std::pair<std::wstring, std::wstring> getNodeInfo(IXMLDOMNode* pNode)const; //int index) const;
			//std::shared_ptr<IXMLDOMDocument> GetDom()const {return pXMLDom;}  wird nicht mit bcc32 unterstützt :(
		IXMLDOMDocument* pXMLDom;
	private:
		void ReleaseResources();
			//std::shared_ptr<IXMLDOMDocument>pXMLDom;  wird nicht mit bcc32 unterstützt :(

			//std::vector < std::pair < std::wstring, std::wstring > > xmlInfo;
	};


#endif  // RRTOOLS_H


