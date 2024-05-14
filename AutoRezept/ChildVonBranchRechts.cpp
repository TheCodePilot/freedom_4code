#include "ChildVonBranchRechts.h"
#include "ChildVonBranchLinks.h"

// Methode zum Datenaustausch mit ChildVonBranchLinks
void ChildVonBranchRechts::sendDataToLeft(int data)
{
	if (leftChild != NULL)
	leftChild->setData(data);
}