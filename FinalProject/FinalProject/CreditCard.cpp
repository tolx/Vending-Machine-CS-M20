/***************************************************
*	Final Project 2016
*	Credit Card implementation file
*	Jake Lyon
*	Sam Rice
***************************************************/
#include <iostream>
#include <string>

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
	return (!!(CC_NUM % 2)); //The !! means NOT(NOT(value)), where the value is mod 2 of the card number, so if the number is odd it's 1 (or true), or even its 0 (false). Having two ! means it'll still be the value, and no warning will be cast for unsafe conversion of int and bool
}
