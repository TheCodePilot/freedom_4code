#ifndef ChildVonBranchRechts_H
#define ChildVonBranchRechts_H

#include "BranchVonRootRechts.h"

class ChildVonBranchLinks;

class ChildVonBranchRechts : public BranchVonRootRechts {
public:
	// Konstruktor
	ChildVonBranchRechts() : leftChild(NULL) {}

    // Methode zum Setzen des Zeigers auf ChildVonBranchLinks
    void setLeftChildPtr(ChildVonBranchLinks* ptr) {
        leftChild = ptr;
    }

    // Methode zum Datenaustausch mit ChildVonBranchLinks
	void sendDataToLeft(int data);
	//Bei inline w�re //#include "ChildVonBranchLinks.h"  n�tig !!!
	/*
	void sendDataToLeft(int data)
	{
		if (leftChild != nullptr)
			leftChild->setData(data);
	} */

private:
    ChildVonBranchLinks* leftChild; // Zeiger auf ChildVonBranchLinks
};

#endif