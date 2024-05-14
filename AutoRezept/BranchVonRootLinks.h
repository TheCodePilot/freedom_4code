#ifndef BranchVonRootLinks_H
#define BranchVonRootLinks_H

#include "Root.h"

class BranchVonRootLinks : public Root {
public:
	virtual void setLeftData(int data) { leftData = data; }
	virtual int getLeftData() const { return leftData; }

protected:
	int leftData;
};
#endif
