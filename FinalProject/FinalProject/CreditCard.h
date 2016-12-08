/*************************************************
*	Final Project 2016
*	Credit Card class file
*	Jake Lyon
*************************************************/
#ifndef _CREDIT_CARD
#define _CREDIT_CARD

#include <string>

#include "PrecondViolatedExcept.h"

class CreditCard
{
public:
	CreditCard(const std::string number, const std::string name,
			   const std::string exp,	 const std::string code);

	const std::string getCCNum	 () const { return CC_NUM; }
	const std::string getName	 () const { return NAME; }
	const std::string getExpDate () const { return (EXP_MONTH + "/" + EXP_YEAR); }
	const std::string getCode	 () const { return SEC_CODE; }

//	const CreditCard swipe() const;
private:
	// No credit card should be constructed with default values.
	CreditCard() = delete;

	// No credit card should be copied. Credit cards must be unique.
	CreditCard(const CreditCard&) = delete;
	CreditCard operator =(const CreditCard&) = delete;

	// Set functions inaccessible to the client
	void setCCNum (const std::string number);
	void setName  (const std::string name);
	void setExp   (const std::string exp);
	void setCode  (const std::string code);


	// Private Data Members are to be set by the set function
	// in the constructor, and never changed again
	mutable std::string CC_NUM;
	mutable std::string NAME;
	mutable int			EXP_MONTH;
	mutable int			EXP_YEAR;
	mutable std::string	SEC_CODE;
};

#endif
