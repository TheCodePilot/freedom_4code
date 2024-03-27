void printChildrenOfNode(IXMLDOMNode* pNode, const std::wstring& targetName, const std::wstring& targetValue, int depth = 0) {
    BSTR nodeName, nodeValue;
    pNode->get_nodeName(&nodeName);
    pNode->get_text(&nodeValue);
    std::wstring wnodeName(nodeName);
    std::wstring wnodeValue(nodeValue);
    SysFreeString(nodeName);
    SysFreeString(nodeValue);

    if (wnodeName == targetName && wnodeValue == targetValue) {
        // Dieser Knoten entspricht dem Zielnamen und -wert, drucke seine Kinder
        IXMLDOMNode* pChildNode;
        pNode->get_firstChild(&pChildNode);
        while (pChildNode) {
            BSTR childName, childValue;
            pChildNode->get_nodeName(&childName);
            pChildNode->get_text(&childValue);
            for (int i = 0; i < depth; ++i) std::wcout << L"  ";
            std::wcout << L"Kindknoten hat den Namen \"" << childName << L"\" und den Wert \"" << childValue << L"\"." << std::endl;
            SysFreeString(childName);
            SysFreeString(childValue);

            IXMLDOMNode* pNextNode;
            pChildNode->get_nextSibling(&pNextNode);
            pChildNode->Release();
            pChildNode = pNextNode;
        }
    }
    else {
        // Dieser Knoten entspricht nicht dem Zielnamen und -wert, durchsuche seine Kinder
        IXMLDOMNode* pChildNode;
        pNode->get_firstChild(&pChildNode);
        while (pChildNode) {
            printChildrenOfNode(pChildNode, targetName, targetValue, depth + 1);
            IXMLDOMNode* pNextNode;
            pChildNode->get_nextSibling(&pNextNode);
            pChildNode->Release();
            pChildNode = pNextNode;
        }
    }
}

void printChildrenUntilNode0(IXMLDOMNode* pNode, const std::wstring& targetName, const std::wstring& targetValue, const std::wstring& stopName, int depth = 0) {
    BSTR nodeName, nodeValue;
    pNode->get_nodeName(&nodeName);
    pNode->get_text(&nodeValue);
    std::wstring wnodeName(nodeName);
    std::wstring wnodeValue(nodeValue);
    SysFreeString(nodeName);
    SysFreeString(nodeValue);

    if (wnodeName == targetName && wnodeValue == targetValue) {
        // Dieser Knoten entspricht dem Zielnamen und -wert, drucke seine Kinder
        IXMLDOMNode* pChildNode;
        //pNode->get_firstChild(&pChildNode);
        pNode->get_nextSibling(&pChildNode);
        while (pChildNode) {
            BSTR childName, childValue;
            pChildNode->get_nodeName(&childName);
            pChildNode->get_text(&childValue);
            std::wstring wchildName(childName);
            SysFreeString(childName);
            SysFreeString(childValue);

            if (wchildName == stopName) {
                // Wir haben den Stoppknoten erreicht, beenden Sie die Ausgabe
                pChildNode->Release();
                break;
            }

            for (int i = 0; i < depth; ++i) std::wcout << L"  ";
            std::wcout << L"Kindknoten hat den Namen \"" << wchildName << L"\" und den Wert \"" << childValue << L"\"." << std::endl;

            IXMLDOMNode* pNextNode;
            pChildNode->get_nextSibling(&pNextNode);
            pChildNode->Release();
            pChildNode = pNextNode;
        }
    }
    /*else {
        // Dieser Knoten entspricht nicht dem Zielnamen und -wert, durchsuche seine Kinder
        IXMLDOMNode* pChildNode;
        pNode->get_firstChild(&pChildNode);
        while (pChildNode) {
            printChildrenUntilNode0(pChildNode, targetName, targetValue, stopName, depth + 1);
            IXMLDOMNode* pNextNode;
            pChildNode->get_nextSibling(&pNextNode);
            pChildNode->Release();
            pChildNode = pNextNode;
        }
    }*/
}

void printChildrenUntilNode1(IXMLDOMNode* pNode, const std::wstring& targetName, const std::wstring& targetValue, const std::wstring& stopName, int depth = 0) {
    BSTR nodeName, nodeValue;
    pNode->get_nodeName(&nodeName);
    pNode->get_text(&nodeValue);
    std::wstring wnodeName(nodeName);
    std::wstring wnodeValue(nodeValue);
    SysFreeString(nodeName);
    SysFreeString(nodeValue);

    if (wnodeName == targetName && wnodeValue == targetValue) {
        // Dieser Knoten entspricht dem Zielnamen und -wert, drucke seine Kinder
        for (int i = 0; i < depth; ++i) std::wcout << L"  ";
        std::wcout << L"Knoten hat den Namen \"" << wnodeName << L"\" und den Wert \"" << wnodeValue << L"\"." << std::endl;

        IXMLDOMNode* pChildNode;
        pNode->get_firstChild(&pChildNode);
        while (pChildNode) {
            BSTR childName, childValue;
            pChildNode->get_nodeName(&childName);
            pChildNode->get_text(&childValue);
            std::wstring wchildName(childName);
            SysFreeString(childName);
            SysFreeString(childValue);

            if (wchildName == stopName) {
                // Wir haben den Stoppknoten erreicht, beenden Sie die Ausgabe
                pChildNode->Release();
                break;
            }

            for (int i = 0; i < depth; ++i) std::wcout << L"  ";
            std::wcout << L"Kindknoten hat den Namen \"" << wchildName << L"\" und den Wert \"" << childValue << L"\"." << std::endl;

            IXMLDOMNode* pNextNode;
            pChildNode->get_nextSibling(&pNextNode);
            pChildNode->Release();
            pChildNode = pNextNode;
        }
    }
    else {
        // Dieser Knoten entspricht nicht dem Zielnamen und -wert, durchsuche seine Kinder
        IXMLDOMNode* pChildNode;
        pNode->get_firstChild(&pChildNode);
        while (pChildNode) {
            printChildrenUntilNode1(pChildNode, targetName, targetValue, stopName, depth + 1);
            IXMLDOMNode* pNextNode;
            pChildNode->get_nextSibling(&pNextNode);
            pChildNode->Release();
            pChildNode = pNextNode;
        }
    }
}

void printChildrenUntilNode(IXMLDOMNode* pNode, const std::wstring& targetName, const std::wstring& targetValue, const std::wstring& stopName, int depth = 0) {
    if (pNode == nullptr) return;

    BSTR nodeName, nodeValue;
    pNode->get_nodeName(&nodeName);
    pNode->get_text(&nodeValue);
    std::wstring wnodeName(nodeName);
    std::wstring wnodeValue(nodeValue);
    SysFreeString(nodeName);
    SysFreeString(nodeValue);

    // Schritt 1: Überprüfen Sie, ob der aktuelle Knoten den Zielnamen und den Zielwert hat.
    if (wnodeName == targetName && wnodeValue == targetValue) {
        // Ausgabe des aktuellen Knotens
        for (int i = 0; i < depth; ++i) std::wcout << L"  "; // Einrücken entsprechend der Tiefe
        std::wcout << L"Name: " << wnodeName << L", Wert: " << wnodeValue << std::endl;
    }

    // Schritt 2: Überprüfen Sie, ob der aktuelle Knoten den Stop-Namen hat.
    if (wnodeName == stopName) {
        return; // Beenden Sie die Funktion
    }

    // Schritt 3: Überprüfen Sie, ob der aktuelle Knoten Kindknoten hat.
    IXMLDOMNodeList* pChildren;
    pNode->get_childNodes(&pChildren);
    if (pChildren != nullptr) {
        long length;
        pChildren->get_length(&length);
        for (long i = 0; i < length; ++i) {
            IXMLDOMNode* pChildNode = nullptr;
            pChildren->get_item(i, &pChildNode);
            printChildrenUntilNode(pChildNode, targetName, targetValue, stopName, depth+1); // Rekursiver Aufruf für Kindknoten
            pChildNode->Release();
        }
        pChildren->Release();
    }
}
