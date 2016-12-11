/*
CS M20 - Final Project
Team Water
Members who worked on this:
Kelton Malhotra
Sam Rice
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
	GoToIdleState();
} // end VendingMachine default constructor

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
	
} // end build state machine

void VendingMachine::GoToIdleState()
{
	currentState = "Idle";
	paidByCreditCard = false;
	total_coins = 0;
	cout << "In idle state " << endl;
} // end GoToIdleState

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
} // end GoToNextstate

// Public functions called by client

void VendingMachine::pushButton(string prodCode)
{
	ProdCodePushed = prodCode;
	bool isValidPosition = true;

	// Check if it'Idle a valid entry
	map<string, Slot>::iterator it;
	it = prodList.find(ProdCodePushed);
	if (it != prodList.end()) // ensure position is valid / find if no drinks available. 
	{
		isValidPosition = false;
	}
	else
	{
		isValidPosition = true;
	} // end if (???)

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
					// Goes to DispenseDrink and then to Idle
				} // end if (can go to next state)
			}
			else
			{
				if (GoToNextState("Valid Position & Not Enough Cash"))
				{
					// Goes to ShowPrice
				} // end if (can go to next state)
			} // end if (enough cash, or more)
		} // end if (can go to next state)
	}
	else
	{
		if (GoToNextState("Invalid Position"))
		{
				// Went to InvalidPosition and then to Update or Idle
		} // end if (can go to next state)
	} // end if (is valid position)
} // end pushButton

void VendingMachine::insertCash(double amt)
{
	if (amt == 0.01 || amt == 0.05 ||  amt == 0.1 || amt == 0.25 || amt == 0.5 || amt == 1.00 || amt == 5.00)
	{
		if (total_coins < coin_max)
		{
			total_coins += amt;
			if (GoToNextState("Insert Cash"))
			{
				// goes from Idle to Update or goes from Update to Update
			} // end if (can go to next state)
		}
		else
		{
			cout << "Too much cash in the machine. $" << amt << " has been returned." << endl; //format this later
		} // end if (not enough cash yet)
	}
	else
	{
		cout << "Cannot insert this type of cash! $" << amt << " has been returned." << endl;
	} // end if (valid amount of cash)
} // end insertCash

void VendingMachine::swipeCard(string cardType)
{
	if ((total_coins < coin_max) && !paidByCreditCard)
	{
		if (GoToNextState("Card Swiped"))
		{
			// goes from Idle to CheckCard and then to Update or Idle
		} // end if (can go to next state)
	}
	else
	{
		//cout << "Card already used, no more credit has been added." << endl; //This should not do anything, just like a real machine
	} // end if (we have enough cash alreay (which could be because we already swipped a card))
} // end swipeCard

void VendingMachine::cancelOrder()
{
	if (paidByCreditCard)
	{
		if (GoToNextState("Has Credit and Card Cancel Pressed"))
		{
			// goes from Update to CancelCard and then to Update or Idle
		} // end if (it can go to next state)
	}
	else
	{
		cout << "No card has been used, nothing to cancel." << endl;
	} // end if (already used a card)
} // end cancleOrder

void VendingMachine::coinReturn()
{
	if (GoToNextState("Coin Return"))
	{
		// goes from Update to CancelCard and then to Update or Idle
	}
	else
	{
		//cout << "Coin Return is not available!" << endl; //It should not do anything if it can't be done, just like a real machine
	} // end if (it can go to next state)
} // end voinReturn

bool VendingMachine::addSlot(string location, string Name, double Price, int Stock)
{
	prodList.insert(make_pair(location, Slot(Name, Price, Stock)));

	return true;
}

bool VendingMachine::getSlot(string location, string & Name, double & Price, int & Stock) 
{
	if (prodList.find(location) == prodList.end())
		return false;
	
	else
	{
		Name = prodList[location].getName();
		Price = prodList[location].getPrice();
		Stock = prodList[location].getStock();
	}
	return true;
}

// END of Client functions