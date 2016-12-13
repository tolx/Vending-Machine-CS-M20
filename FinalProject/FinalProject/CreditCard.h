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

	const int getCCNum() const { return CCNum; }

	bool isValid() const;

private:
	// No credit card should be constructed with default values.
	CreditCard() = delete;

	// No credit card should be copied. Credit cards must be unique.
	CreditCard(const CreditCard&) = delete;
	CreditCard operator =(const CreditCard&) = delete;

	// Set functions inaccessible to the client
	void setCCNum(const int number);

	// Private Data Members are to be set by the set function
	// in the constructor, and never changed again
	int CCNum;
};

#endif
