#include "ChildVonBranchLinks.h"
#include "ChildVonBranchRechts.h"

// Methode zum Datenaustausch mit ChildVonBranchRechts
void ChildVonBranchLinks::sendDataToRight(int data)
{
		if (rightChild != NULL)
			rightChild->setData(data);
}