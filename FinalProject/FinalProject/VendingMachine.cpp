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

	// Build State Machine table - Connect states (vertices) with Action(Edges)	
	BuildStateMachine();
	BuildProdList();
	GoToIDleState();
}

void VendingMachine::BuildStateMachine()
{
	int i = 0;

	sm[i].action = PUSH_BUTTON;
	sm[i].curState = IDLE;
	sm[i].cb = &VendingMachine::DisplayProdPrice;
	++i;

	sm[i].action = INSERT_CASH;
	sm[i].curState = IDLE;
	sm[i].cb = &VendingMachine::CashInserted;
	i++;
	sm[i].action = INSERT_CASH;
	sm[i].curState = CASH_INSERTED;
	sm[i].cb = &VendingMachine::CashInserted;
	i++;
	sm[i].action = PUSH_BUTTON;
	sm[i].curState = CASH_INSERTED;
	sm[i].cb = &VendingMachine::DispenseProduct;
	i++;
	sm[i].action = PUSH_BUTTON;
	sm[i].curState = CARD_ACCEPTED;
	sm[i].cb = &VendingMachine::DispenseProduct;
	i++;

	sm[i].action = SWIPE_CREDIT_CARD;
	sm[i].curState = IDLE;
	sm[i].cb = &VendingMachine::CardSwiped;
	i++;


	sm[i].action = SWIPE_CREDIT_CARD;
	sm[i].curState = CASH_INSERTED;
	sm[i].cb = &VendingMachine::CardSwiped;
	i++;


	sm[i].action = CANCEL_ORDER;
	sm[i].curState = CASH_INSERTED;
	sm[i].cb = &VendingMachine::OrderCancelled;
	i++;
	sm[i].action = CANCEL_ORDER;
	sm[i].curState = CARD_ACCEPTED;
	sm[i].cb = &VendingMachine::OrderCancelled;

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

bool VendingMachine::GoToNextState(int action)
{
	//Check if action/Current state are valid



	int i;
	for (i = 0; i < 20; i++)
	{
		if (sm[i].action == action && sm[i].curState == currentState)
		{

			(this->*sm[i].cb)();
			return true;
		}
	}


	return false;
}

// Callbacks called by state Machine
void VendingMachine::DisplayProdPrice()
{
	//Check if prod code is valid
	map<string, prod>::iterator it;
	it = prodList.find(ProdCodePushed);
	if (it == prodList.end())
	{
		cout << "Invalid entry " << endl;
		return;
	}
	prod p = it->second;
	double prodCost = p.GetProdPrice();
	string prodname = p.GetProdName();
	cout << prodname << "Costs " << prodCost << endl;

}
void VendingMachine::DispenseProduct()
{
	map<string, prod>::iterator it;
	it = prodList.begin();
	it = prodList.find(ProdCodePushed);

	prod p = it->second;
	double prodCost = p.GetProdPrice();
	string prodname = p.GetProdName();
	if (currentState == CASH_INSERTED)
	{
		if (total_coins >= prodCost)
		{
			cout << "Please pick up " << prodname << endl;
			if (total_coins > prodCost)
			{
				Refund(total_coins - prodCost);
			}
			GoToIDleState();
		}
		else
		{
			DisplayProdPrice();
		}
	}
	else if (currentState == CARD_ACCEPTED)
	{
		cout << "Please pick up " << prodname << endl;

		GoToIDleState();
	}



}



void VendingMachine::CardSwiped()
{

	currentState = CARD_ACCEPTED;
	paidByCreditCard = true;
	if (total_coins >0)
	{
		Refund(total_coins);
		total_coins = 0;
	}
	cout << "Card accepted " << endl;
}

void VendingMachine::CashInserted()
{
	currentState = CASH_INSERTED;
	cout << "Total coins inserted " << total_coins << endl;

}

void VendingMachine::OrderCancelled()
{

	if (total_coins > 0)

		Refund(total_coins);
	if (paidByCreditCard)
		CancelCardTransaction();

	GoToIDleState();
}
// end of callbacks

// Public functions called by client

void VendingMachine::PushButton(string prodCode)
{

	ProdCodePushed = prodCode;
	bool b;


	b = GoToNextState(PUSH_BUTTON);
	if (b == false)
		cout << "Invalid Action " << endl;


}

void VendingMachine::InsertCash(int amt)
{

	total_coins += amt;
	bool b = GoToNextState(INSERT_CASH);
	//CashInserted();

}
void VendingMachine::swipeCard(string cardType)
{
		bool b = GoToNextState(SWIPE_CREDIT_CARD);
		//CardSwiped();
	
		if (b == false)
	{
		cout << "Invalid action" << endl;
	}
}
void VendingMachine::CancelOrder()
{
	bool b = GoToNextState(CANCEL_ORDER);
	if (b == false)
		cout << "Invalid state and action can't be performed" << endl;


}
// END of Client functions

// Private (utility functions)

void VendingMachine::Refund(double cost)
{

	cout << " Refunding  cash " << cost << endl;
	total_coins -= cost;

}
void VendingMachine::CancelCardTransaction()
{

	cout << " Cancelling Credit card Transactions " << endl;


}

