/*
CS M20 - Final Project
Team Water
Members who worked on this:
Kelton Malhotra
*/

#include "VendingMachine.h"
using namespace std;
#include <iostream>


VendingMachine::VendingMachine()
{

	BuildProdList();
	GoToIDleState();
}

void VendingMachine::BuildProdList()
{
	prod p1("Costco Water", 1.50);
	prod p2("Crystal Geyser", 1.50);
	prod p3("Aquafina", 1.50);


	prodList.insert(make_pair("D4", p1));

	prodList.insert(make_pair("D5", p2));

	prodList.insert(make_pair("D6", p3));



}

void VendingMachine::GoToIDleState()
{

	currentState = IDLE;
	paidByCreditCard = false;
	total_coins = 0;
	cout << "In idle state " << endl;

}
void VendingMachine::DisplayProdPrice(string prodCode)
{
	//Check if prod code is valid
	map<string, prod>::iterator it;
	it = prodList.find(prodCode);
	if (it == prodList.end())
	{
		cout << "Invalid entry " << endl;
		return;
	}
	prod p = it->second;
	double prodCost = p.GetProdPrice();
	string prodname = p.GetProdName();
	cout << prodname << " Costs " << prodCost << endl;

}
void VendingMachine::DispenseProduct(string prodName)
{

	cout << "Please pick up your " << prodName << endl;
}

void VendingMachine::PushButton(string prodCode)
{
	//Check if prod code is valid
	map<string, prod>::iterator it;
	it = prodList.find(prodCode);
	if (it == prodList.end())
	{
		cout << "Invalid entry " << endl;
		return;
	}
	prod p = it->second;
	double prodCost = p.GetProdPrice();
	if (currentState == IDLE)
	{
		DisplayProdPrice(prodCode);
		GoToIDleState();
	}
	else if (currentState == CASH_INSERTED)
	{
		if (total_coins >= prodCost)
		{
			DispenseProduct(p.GetProdName());
			Refund(total_coins - prodCost);
			GoToIDleState();
		}
		else
		{
			DisplayProdPrice(prodCode);
		}
	}
	else if (currentState == CARD_ACCEPTED)
	{
		DispenseProduct(p.GetProdName());

		GoToIDleState();
	}

}
void VendingMachine::Refund(double cost)
{

	cout << " Refunding  cash " << cost << endl;

}
void VendingMachine::CancelCardTransaction()
{

	cout << " Cancelling Credit card Transactions " << endl;
	GoToIDleState();

}
void VendingMachine::InsertCash(double amt)
{
	currentState = CASH_INSERTED;
	total_coins += amt;
	cout << "Total coins inserted " << total_coins << endl;
}
void VendingMachine::swipeCard(string cardType)
{
	if (cardType == "VISA" || cardType == "MASTER") // Good card
	{
		currentState = CARD_ACCEPTED;
		paidByCreditCard = true;
		cout << "credit card accepted" << endl;
		if (total_coins > 0)
		{
			Refund(total_coins);
		}
	}
	else
	{
		cout << "Invalid Credit card" << endl;
	}
}
void VendingMachine::CancelOrder()
{
	if (paidByCreditCard)
	{
		CancelCardTransaction();
	}
	if (total_coins > 0)
	{
		Refund(total_coins);
	}

	GoToIDleState();
}
