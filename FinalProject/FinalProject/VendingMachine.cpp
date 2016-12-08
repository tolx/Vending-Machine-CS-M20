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
	currentState = "S";
	// Build State Machine table - Connect states (vertices) with Action(Edges)	
	BuildStateMachine();
	BuildProdList();
	GoToIDleState();
}

void VendingMachine::BuildStateMachine()
{
	int i = 0;

	statesMachine.Init(8);

	Edge<string, string> S_U("S", "U", "Insert Cash");
	statesMachine.add(S_U);

	Edge<string, string> U_U("U", "U", "Insert Cash");
	statesMachine.add(U_U);


	Edge<string, string> S_SP("S", "SP", "Valid Position");
	statesMachine.add(S_SP);

	Edge<string, string> S_IP("S", "IP", "Invalid Position");
	statesMachine.add(S_IP);

	Edge<string, string> S_CH("S", "CH", "Card Swiped");
	statesMachine.add(S_CH);

	Edge<string, string> SP_U("SP", "U", "Has Cash");
	statesMachine.add(SP_U);

	Edge<string, string> SP_S("SP", "S", "No Cash");
	statesMachine.add(SP_S);

	Edge<string, string> U_SP("U", "SP", "Valid Position & Not enough cash");
	statesMachine.add(U_SP);

	Edge<string, string> U_D("U", "D", "Valid Position & enough cash");
	statesMachine.add(U_D);

	Edge<string, string> D_S("D", "S", "No leftover cash");
	statesMachine.add(D_S);

	Edge<string, string> D_DC("D", "DC", "Leftover cash");
	statesMachine.add(D_DC);

	Edge<string, string> DC_S("DC", "S", "Cash returned");
	statesMachine.add(DC_S);

	Edge<string, string> IP_U("IP", "U", "Has Cash");
	statesMachine.add(IP_U);

	Edge<string, string> U_IP("U", "IP", "Invalid Position");
	statesMachine.add(U_IP);

	Edge<string, string> IP_S("IP", "S", "No Cash");
	statesMachine.add(IP_S);

	Edge<string, string> U_CH("U", "CH", "Card Swiped");
	statesMachine.add(U_CH);

	Edge<string, string> CH_U("CH", "U", "Card Approved Or Declined and has cash");
	statesMachine.add(CH_U);
	
	Edge<string, string> CH_S("CH", "S", "Card Declined and no cash");
	statesMachine.add(CH_S);
	
	Edge<string, string> U_CA("U", "CA", "Cancel Pressed");
	statesMachine.add(U_CA);

	Edge<string, string> CA_U("CA", "U", "Has Cash");
	statesMachine.add(CA_U);

	Edge<string, string> CA_S("CA", "S", "No Cash");
	statesMachine.add(CA_S);

	Edge<string, string> U_DC("U", "DC", "Coin Return");
	statesMachine.add(U_DC);

	Edge<string, string> DC_U("DC", "U", "Has Credit");
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
	prod p1("Costco Water", 1.50);
	prod p2("Crystal Geyser", 1.50);
	prod p3("Aquafina", 1.50);


	prodList.insert(make_pair("D4", p1));

	prodList.insert(make_pair("D5", p2));

	prodList.insert(make_pair("D6", p3));



}


void VendingMachine::GoToIDleState()
{

	currentState = "S";
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


			if (currentState == "SP")
			{
				cout << "Price is 1.5$ for " << ProdCodePushed << endl;
				if (total_coins > 0)
				{
					GoToNextState("Has Cash");
				}
				else
				{
					GoToNextState("No Cash");
				}
			}

			else if (currentState == "U")
			{
				cout << "We have " << total_coins << endl;
			}
			else if (currentState == "CA")
			{
				paidByCreditCard = false;
				total_coins -= 1.5;
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
			else if (currentState == "D")
			{
				cout << "Dispensing Drink.." << endl;
				total_coins -= 1.5;
				paidByCreditCard = false;
				if (total_coins == 0)
				{
					GoToNextState("No leftover cash");
				}
				else
				{
					GoToNextState("Leftover cash");
				}
			}
			else if (currentState == "CH")
			{
				

				bool is_card_approved = true;

				if (is_card_approved)
				{
					cout << "Checking Card... Approved" << endl;
					paidByCreditCard = true;
				}
				else
				{
					cout << "Checking Card... Declined" << endl;
				}
				
				total_coins += 1.5;
				if (is_card_approved || (!is_card_approved && total_coins > 0))
				{
					GoToNextState("Card Approved Or Declined and has cash");
				}
				else if (!is_card_approved && total_coins == 0)
				{
					GoToNextState("Card Declined and no cash");
				}

			}
			else if (currentState == "DC")
			{
					
					if (!paidByCreditCard)
					{
						cout << "Dispensing cash " << total_coins << endl;
						total_coins = 0;
						GoToNextState("Cash returned");
					}
					else
					{
						cout << "Dispensing cash " << (total_coins - 1.5) << endl;
						total_coins = 1.5;
						GoToNextState("Has Credit");
					}
					
			}
			else if (currentState == "S")
			{
				cout << "In idle state" << endl;
			}

			else if (currentState == "IP")
			{
				cout << "Invalid Input!" << endl;
				if (total_coins > 0)
				{
					GoToNextState("Has Cash");
				}
				else
				{
					GoToNextState("No Cash");
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
			GoToIDleState();
		}
		else
		{
			DisplayProdPrice();
		}
	}
	else if (currentState == "CARD_ACCEPTED")
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
*/
// Public functions called by client

void VendingMachine::PushButton(string prodCode)
{

	ProdCodePushed = prodCode;
	bool isValidPosition = true;

	// Check if it's a valid entry
	map<string, prod>::iterator it;
	it = prodList.find(ProdCodePushed);
	if (it == prodList.end())
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

			// Went to SP and then to S
		}
		else
		{
			if (total_coins >= 1.5)
			{

				if (GoToNextState("Valid Position & enough cash"))
				{
					// Went to D and then to S
				}
			}
			else
			{
				if (GoToNextState("Valid Position & Not enough cash"))
				{
					// Went to SP
				}
			}
		}

	}
	else
	{
		if (GoToNextState("Invalid Position"))
		{

				// Went to IP and then to U or S
		}
	}



}

void VendingMachine::InsertCash(double amt)
{
	if (amt == 0.25 || amt == 0.05 ||  amt == 0.5 ||amt == 0.10 || amt == 1.00 || amt == 5.00)
	{
		if (total_coins < 1.5)
		{
			total_coins += amt;
			if (GoToNextState("Insert Cash"))
			{
				// goes from S to U or goes from U to U
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
	if (total_coins < 1.5)
	{
		if (GoToNextState("Card Swiped"))
		{
			// goes from S to CH and then to U or S
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
void VendingMachine::CancelOrder()
{
	if (paidByCreditCard)
	{
		if (GoToNextState("Cancel Pressed"))
		{
			// goes from U to CA and then to U or S
		}
	}
	else
	{
		cout << "Nothing to cancel" << endl;
	}
}

void VendingMachine::CoinReturn()
{
		if (GoToNextState("Coin Return"))
		{
			// goes from U to CA and then to U or S
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

