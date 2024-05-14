#ifndef ChildVonBranchLinks_H
#define ChildVonBranchLinks_H

#include "BranchVonRootLinks.h"

// Vorwärtsdeklaration
class ChildVonBranchRechts;

// Klasse ChildVonBranchLinks
class ChildVonBranchLinks : public BranchVonRootLinks {
public:
	// Konstruktor
	ChildVonBranchLinks() : rightChild(NULL) {}

	// Methode zum Setzen des Zeigers auf ChildVonBranchRechts
	void setRightChildPtr(ChildVonBranchRechts* ptr) {
		rightChild = ptr;
	}

	// Methode zum Datenaustausch mit ChildVonBranchRechts
	void sendDataToRight(int data);
	//Bei inline wäre //#include "ChildVonBranchRechts.h"  nötig !!!
	/*
	void sendDataToRight(int data)
	{
		if (rightChild != nullptr)
			rightChild->setData(data);
	}*/

private:
    ChildVonBranchRechts* rightChild; // Zeiger auf ChildVonBranchRechts
};

#endif