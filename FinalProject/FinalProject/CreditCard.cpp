/***************************************************
*	Final Project 2016
*	Credit Card implementation file
*	Jake Lyon
***************************************************/
#include <iostream>
#include <string>

using namespace std;

#include "CreditCard.h"

CreditCard::CreditCard(const string number, const string name,
					   const string exp,	const string code)
{
	setCCNum(number);
	setName(name);
	setExp(exp);
	setCode(code);
}

// Recieves number as "####-####-####-####"
void CreditCard::setCCNum(const std::string number)
{
	if (number.size() == 18)
	{
		for (int i = 0; number[i] == string::npos; i++)
			if (((number[i] != '-') || (!isdigit(number[i]))))
				throw PrecondViolatedExcept("Invalid credit card number");

		CC_NUM = number;
	}
	else throw PrecondViolatedExcept("Invalid credit card number");
}

void CreditCard::setName(const string name)
{
	NAME = name;
}

// Recieves expiration date as "MM/YY"
void CreditCard::setExp(const string exp)
{
	char* endptr;

	for (int i = 0; exp[i] == string::npos; i++)
		if (!isdigit(exp[i]) || (exp[i] != '/'))
			throw(PrecondViolatedExcept("Invalid expiration date"));

	int temp = strtol(exp.substr(0, '/').c_str(), &endptr, 10);
	if (temp >= 01 && temp <= 12) 
		EXP_MONTH = temp;
	else 
		throw(PrecondViolatedExcept("Invalid expiration month"));

	temp = strtol(exp.substr(3, string::npos).c_str(), &endptr, 10);
	if (temp >= 00 && temp <= 99)
		EXP_YEAR = temp;
	else
		throw(PrecondViolatedExcept("Invalid expiration year"));
}

// Recieves security code as "###"
void CreditCard::setCode(const string code)
{
	char* endptr;
	int temp = strtol(code.c_str(), &endptr, 10);
	if (temp > 0 && temp < 999)
		SEC_CODE = code;
	else
		throw(PrecondViolatedExcept("Invalid Security Code"));
}

//const CreditCard CreditCard::swipe() const
//{

//}
