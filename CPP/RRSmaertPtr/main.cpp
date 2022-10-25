#include <iostream>
#include <memory>
using namespace std;


class A
{
	public:
	void doSomething(){};
};

class UseA
{
private:
	std::shared_ptr<A> pA;
public:
	UseA()
	{
		pA = std::shared_ptr<A>(new A());
	}

	std::shared_ptr<A> getA()
	{
		return pA;
	}
};

//#########################################Test-Functions#####################################################
//#########################################shared_ptr#####################################################
//#########################################Test-Functions#####################################################



void SomeAction()
{
	UseA* pUseA = new UseA(); //with the UseA object, a new A is created and stored, reference counter = 1

	std::shared_ptr<A> pA = pUseA->getA(); //a copy of the shared pointer is created, reference counter = 2

    pA->doSomething();

	delete pUseA; //with delete pUseA the private pA is destroyed, reference counter = 1

	return; //with the return the local pA is destroyed automatically, reference counter = 0
	//the reason:
	//internally the std::shared_ptr destroys the reference to the A object
}


void SomeOtherAction(std::shared_ptr<UseA> pUseA)
{
	std::shared_ptr<A> pA = pUseA->getA(); //a copy of the shared pointer is created
	//reference counter = 2

	pA->doSomething();

	return; //local pFoo is destroyed
	//reference counter = 1
}

//#########################################Test-Functions#####################################################
//#########################################unique_ptr#####################################################
//#########################################Test-Functions#####################################################

void rrtest_leakPtr()
{
    int* valuePtr = new int(15);
    int x = 45;
    // ...
    if (x == 45)
        return;   // here we have a memory leak, valuePtr is not deleted
    // ...
    delete valuePtr;
}

void rrtest_noleakPtr()
{
    std::unique_ptr<int> valuePtr(new int(15));

    int x = 45;
    // ...
    if (x == 45)
        return;   // no memory leak anymore!
    // ...
}

int main()
{
    string anyKey;

    std::unique_ptr<int> ptrBeforeMove(new int(15));
    std::cout << "The original pointer is created with value of 15 " << '\n' << endl;

    std::cout << "Ready to move the pointer to next? ATTENTION: " << '\n' << "THIS WILL INVALIDATE THE original pointer.Yes: press return please. No: close window" << endl;
    std::getline (std::cin,anyKey);

    //The unique_ptr<> does not support copying. If you try to copy a
    //unique_ptr<> you will get compiler errors.
    std::unique_ptr<int> ptrMovedAndNow(std::move(ptrBeforeMove));

    std::cout << "It was moved to another pointer. This new pointer now has the value of the original pointer = " << *ptrMovedAndNow << '\n' << endl;
    std::cout << "Does the original pointer still have an associated object? " << '\n' << endl;
    std::cout << "Answer : " << std::boolalpha << static_cast<bool>(ptrBeforeMove) << '\n' << endl;
    std::cout << "You see the original pointer is invalidate !" << '\n' << endl;

    return 0;
}



