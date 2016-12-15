/***************************************************
*	Final Project 2016
*	Team Water
*	Credit Card header file
***************************************************/
#ifndef _CREDIT_CARD
#define _CREDIT_CARD

#include "PrecondViolatedExcept.h"

class CreditCard
{
public:
	CreditCard(const int number);

	const int getCCNum() const { return CC_NUM; }

	bool isValid() const;

	bool chargeCard(double amount);
	bool checkCredit(double amount);

private:
	
	// Private Data Members are to be set by the set function
	// in the constructor, and never changed again
	const int CC_NUM;
};

#endif
