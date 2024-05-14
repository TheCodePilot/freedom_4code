#ifndef BranchVonRootRechts_H
#define BranchVonRootRechts_H

#include "Root.h"

class BranchVonRootRechts : public Root {
public:
	virtual void setRightData(int data) { rightData = data; }
	virtual int getRightData() const { return rightData; }

protected:
	int rightData;
};
#endif