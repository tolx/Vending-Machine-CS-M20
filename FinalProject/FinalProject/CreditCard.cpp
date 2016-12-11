/***************************************************
*	Final Project 2016
*	Credit Card implementation file
*	Jake Lyon
***************************************************/
#include <iostream>
#include <string>

using namespace std;

#include "CreditCard.h"

CreditCard::CreditCard(const int number)
{
	setCCNum(number);
}

// Recieves number as "####-####-####-####"
void CreditCard::setCCNum(const int number)
{
	CC_NUM = number;
}


bool CreditCard::is_valid() const
{
	return bool(CC_NUM % 2);
}
