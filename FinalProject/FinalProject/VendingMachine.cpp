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
	coin_max = COIN_MAX;
	currentState = "Idle";
	// Build State Machine table - Connect states (vertices) with Action(Edges)	
	BuildStateMachine();
	BuildProdList();
	GoToIdleState();
}

void VendingMachine::BuildStateMachine()
{
	int i = 0;

	statesMachine.Init(8);

	Edge<string, string> S_U("Idle", "Update", "Insert Cash");
	statesMachine.add(S_U);

	Edge<string, string> U_U("Update", "Update", "Insert Cash");
	statesMachine.add(U_U);


	Edge<string, string> S_SP("Idle", "ShowPrice", "Valid Position");
	statesMachine.add(S_SP);

	Edge<string, string> S_IP("Idle", "InvalidPosition", "Invalid Position");
	statesMachine.add(S_IP);

	Edge<string, string> S_CH("Idle", "CheckCard", "Card Swiped");
	statesMachine.add(S_CH);

	Edge<string, string> SP_U("ShowPrice", "Update", "Has Cash");
	statesMachine.add(SP_U);

	Edge<string, string> SP_S("ShowPrice", "Idle", "No Cash");
	statesMachine.add(SP_S);

	Edge<string, string> U_SP("Update", "ShowPrice", "Valid Position & Not Enough Cash");
	statesMachine.add(U_SP);

	Edge<string, string> U_D("Update", "DispenseDrink", "Valid Position & Enough Cash/Credit");
	statesMachine.add(U_D);

	Edge<string, string> D_S("DispenseDrink", "Idle", "No Leftover Cash");
	statesMachine.add(D_S);

	Edge<string, string> D_DC("DispenseDrink", "DispenseChange", "Cash Leftover");
	statesMachine.add(D_DC);

	Edge<string, string> DC_S("DispenseChange", "Idle", "Change Dispensed");
	statesMachine.add(DC_S);

	Edge<string, string> IP_U("InvalidPosition", "Update", "Has Cash/Credit");
	statesMachine.add(IP_U);

	Edge<string, string> U_IP("Update", "InvalidPosition", "Invalid Position");
	statesMachine.add(U_IP);

	Edge<string, string> IP_S("InvalidPosition", "Idle", "No Cash/Credit");
	statesMachine.add(IP_S);

	Edge<string, string> U_CH("Update", "CheckCard", "Card Swiped");
	statesMachine.add(U_CH);

	Edge<string, string> CH_U("CheckCard", "Update", "Card Approved Or Declined and Has Cash");
	statesMachine.add(CH_U);
	
	Edge<string, string> CH_S("CheckCard", "Idle", "Card Declined and No Cash");
	statesMachine.add(CH_S);
	
	Edge<string, string> U_CA("Update", "CancelCard", "Has Credit and Card Cancel Pressed");
	statesMachine.add(U_CA);

	Edge<string, string> CA_U("CancelCard", "Update", "Has Cash");
	statesMachine.add(CA_U);

	Edge<string, string> CA_S("CancelCard", "Idle", "No Cash");
	statesMachine.add(CA_S);

	Edge<string, string> U_DC("Update", "DispenseChange", "Coin Return");
	statesMachine.add(U_DC);

	Edge<string, string> DC_U("DispenseChange", "Update", "Has Credit");
	statesMachine.add(DC_U);
	/*

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

	*/

}

void VendingMachine::BuildProdList()
{
	prod p1("Costco Water", coin_max);
	prod p2("Crystal Geyser", coin_max);
	prod p3("Aquafina", coin_max);


	prodList.insert(make_pair("D4", p1));

	prodList.insert(make_pair("D5", p2));

	prodList.insert(make_pair("D6", p3));
}

void VendingMachine::GoToIdleState()
{

	currentState = "Idle";
	paidByCreditCard = false;
	total_coins = 0;
	cout << "In idle state " << endl;

}

bool VendingMachine::GoToNextState(string transition)
{
		try
		{
			string newState = statesMachine.findEnd(currentState, transition);
			currentState = newState;


			if (currentState == "ShowPrice")
			{
				cout << "Price is $" << coin_max << " for " << ProdCodePushed << endl;
				if (total_coins > 0)
				{
					GoToNextState("Has Cash");
				}
				else
				{
					GoToNextState("No Cash");
				}
			}

			else if (currentState == "Update")
			{
				cout << "We have " << total_coins << endl;
			}
			else if (currentState == "CancelCard")
			{
				paidByCreditCard = false;
				total_coins -= coin_max;
				cout << "Card Transaction Canceled" << endl;
				if (total_coins > 0)
				{
					GoToNextState("Has Cash");
				}
				else
				{
					GoToNextState("No Cash");
				}
			}
			else if (currentState == "DispenseDrink")
			{
				cout << "Dispensing Drink.." << endl;
				total_coins -= coin_max;
				paidByCreditCard = false;
				if (total_coins == 0)
				{
					GoToNextState("No Leftover Cash");
				}
				else
				{
					GoToNextState("Cash Leftover");
				}
			}
			else if (currentState == "CheckCard")
			{
				bool is_card_approved = true; //FIX THIS!!?!?!?!? -- Set to the bank class function call

				if (is_card_approved)
				{
					cout << "Checking Card... Approved" << endl;
					paidByCreditCard = true;
				}
				else
				{
					cout << "Checking Card... Declined" << endl;
				}
				
				total_coins += coin_max;
				if (is_card_approved || (!is_card_approved && total_coins > 0))
				{
					GoToNextState("Card Approved Or Declined and Has Cash");
				}
				else if (!is_card_approved && total_coins == 0)
				{
					GoToNextState("Card Declined and No Cash");
				}

			}
			else if (currentState == "DispenseChange")
			{
					
					if (!paidByCreditCard)
					{
						cout << "Dispensing cash " << total_coins << endl;
						total_coins = 0;
						GoToNextState("Change Dispensed");
					}
					else
					{
						cout << "Dispensing cash " << (total_coins - 1.5) << endl;
						total_coins = coin_max;
						GoToNextState("Has Credit");
					}
					
			}
			else if (currentState == "Idle")
			{
				cout << "In idle state" << endl;
			}

			else if (currentState == "InvalidPosition")
			{
				cout << "Invalid Input!" << endl;
				if (total_coins > 0)
				{
					GoToNextState("Has Cash/Credit");
				}
				else
				{
					GoToNextState("No Cash/Credit");
				}
			}

			return true;
		}
		catch (...)
		{
			return false;
		}



	return false;
}

/*
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
	if (currentState == "CASH_INSERTED")
	{
		if (total_coins >= prodCost)
		{
			cout << "Please pick up " << prodname << endl;
			if (total_coins > prodCost)
			{
				Refund(total_coins - prodCost);
			}
			GoToIdleState();
		}
		else
		{
			DisplayProdPrice();
		}
	}
	else if (currentState == "CARD_ACCEPTED")
	{
		cout << "Please pick up " << prodname << endl;

		GoToIdleState();
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

	GoToIdleState();


}
// end of callbacks
*/

// Public functions called by client

void VendingMachine::pushButton(string prodCode)
{

	ProdCodePushed = prodCode;
	bool isValidPosition = true;

	// Check if it'Idle a valid entry
	map<string, prod>::iterator it;
	it = prodList.find(ProdCodePushed);
	if (it != prodList.end()) // ensure position is valid / find if no drinks available. 
	{
		isValidPosition = false;
	}
	else
	{
		isValidPosition = true;
	}


	if (isValidPosition)
	{

		if (GoToNextState("Valid Position"))
		{

			// Went to ShowPrice and then to Idle
		}
		else
		{
			if (total_coins >= coin_max)
			{

				if (GoToNextState("Valid Position & Enough Cash/Credit"))
				{
					// Went to DispenseDrink and then to Idle
				}
			}
			else
			{
				if (GoToNextState("Valid Position & Not Enough Cash"))
				{
					// Went to ShowPrice
				}
			}
		}

	}
	else
	{
		if (GoToNextState("Invalid Position"))
		{

				// Went to InvalidPosition and then to Update or Idle
		}
	}



}

void VendingMachine::insertCash(double amt)
{
	if (amt == 0.25 || amt == 0.05 ||  amt == 0.5 ||amt == 0.10 || amt == 1.00 || amt == 5.00)
	{
		if (total_coins < coin_max)
		{
			total_coins += amt;
			if (GoToNextState("Insert Cash"))
			{
				// goes from Idle to Update or goes from Update to Update
			}
		}
		else
		{
			cout << "Cash insert is not available!" << endl;
		}
	}
	else
	{
		cout << "Cannot insert this type of cash!" << endl;
	}

	//total_coins += amt;
	//bool b = GoToNextState(INSERT_CASH);
	//CashInserted();

}

void VendingMachine::swipeCard(string cardType)
{
	if (total_coins < coin_max)
	{
		if (GoToNextState("Card Swiped"))
		{
			// goes from Idle to CheckCard and then to Update or Idle
		}
	}
	else
	{
		cout << "Swipe card is not available!" << endl;
	}

	if (paidByCreditCard)
	{

	}
}

void VendingMachine::cancelOrder()
{
	if (paidByCreditCard)
	{
		if (GoToNextState("Has Credit and Card Cancel Pressed"))
		{
			// goes from Update to CancelCard and then to Update or Idle
		}
	}
	else
	{
		cout << "Nothing to cancel" << endl;
	}
}

void VendingMachine::coinReturn()
{
		if (GoToNextState("Coin Return"))
		{
			// goes from Update to CancelCard and then to Update or Idle
		}
	else
	{
		cout << "Coin Return is not available!" << endl;
	}
}

// END of Client functions

// Private (utility functions)

/*
void VendingMachine::Refund(double cost)
{

	cout << " Refunding  cash " << cost << endl;
	total_coins -= cost;

}
void VendingMachine::CancelCardTransaction()
{

	cout << " Cancelling Credit card Transactions " << endl;


}

*/