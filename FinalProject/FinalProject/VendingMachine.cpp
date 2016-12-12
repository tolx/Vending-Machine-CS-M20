/*
CS M20 - Final Project
Team Water
Members who worked on this:
Kelton Malhotra
Sam Rice
Justin Kephart
Riley Wallace
*/

#include "VendingMachine.h"



VendingMachine::VendingMachine() : displayObj(cout)
{
	coin_max = COIN_MAX;
	currentState = "Idle";
	// Build State Machine table - Connect states (vertices) with Action(Edges)	
	BuildStateMachine();
	GoToIdleState();
} // end VendingMachine default constructor

VendingMachine::VendingMachine(ostream& obj) : displayObj(obj)
{
	coin_max = COIN_MAX;
	currentState = "Idle";
	// Build State Machine table - Connect states (vertices) with Action(Edges)	
	BuildStateMachine();
	BuildActionTable(); // Input new current state and callback function to complete action
	GoToIdleState();
} // end VendingMachine constructor

void VendingMachine::BuildActionTable()
{
	int i = 0;

	Atable[i].curState = "ShowPrice";
	Atable[i].cb = &VendingMachine::DisplayPrice;
	i++;

	Atable[i].curState = "Update";
	Atable[i].cb = &VendingMachine::DisplayTotalCoins;

	i++;
	Atable[i].curState = "DispenseDrink";
	Atable[i].cb = &VendingMachine::DispenseDrink;

	i++;
	Atable[i].curState = "CheckCard";
	Atable[i].cb = &VendingMachine::ProcessCreditCard;

	i++;
	Atable[i].curState = "DispenseChange";
	Atable[i].cb = &VendingMachine::RefundChange;

	i++;

	Atable[i].curState = "Idle";
	Atable[i].cb = &VendingMachine::ProcessIdleState;

	i++;
	Atable[i].curState = "InvalidPosition";
	Atable[i].cb = &VendingMachine::ProcessInvalidState;



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

} // end build state machine

void VendingMachine::GoToIdleState()
{
	currentState = "Idle";
	paidByCreditCard = false;
	total_coins = 0;
	displayObj << "In idle state " << endl;
} // end GoToIdleState
// Following methods called by GoToNextState

void VendingMachine::DisplayPrice()
{
	displayObj << "Price is $" << coin_max << " for " << ProdCodePushed << endl;
	if (total_coins > 0)
	{
		GoToNextState("Has Cash");
	}
	else
	{
		GoToNextState("No Cash");
	}
}

void VendingMachine::DisplayTotalCoins()
{
	displayObj << "We have " << total_coins << endl;
}

void VendingMachine::CancelCreditTransaction()
{
	paidByCreditCard = false;
	total_coins -= coin_max;
	displayObj << "Card Transaction Canceled" << endl;
	if (total_coins > 0)
	{
		GoToNextState("Has Cash");
	}
	else
	{
		GoToNextState("No Cash");
	}
}

void VendingMachine::DispenseDrink()
{
	displayObj << "Dispensing Drink.." << endl;
	prodList[ProdCodePushed].dispense();

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

void VendingMachine::ProcessCreditCard()
{
	CreditCard c(ccNum);

	bool is_card_approved = c.isValid();

	if (is_card_approved)
	{
		displayObj << "Checking Card... Approved" << endl;
		paidByCreditCard = true;
	}
	else
	{
		displayObj << "Checking Card... Declined" << endl;
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

void VendingMachine::RefundChange()
{
	if (!paidByCreditCard)
	{
		displayObj << "Dispensing cash " << total_coins << endl;
		total_coins = 0;
		GoToNextState("Change Dispensed");
	}
	else
	{
		displayObj << "Dispensing cash " << (total_coins - 1.5) << endl;
		total_coins = coin_max;
		GoToNextState("Has Credit");
	}
}

void VendingMachine::ProcessInvalidState()
{
	displayObj << "Invalid Input!" << endl;
	if (total_coins > 0)
	{
		GoToNextState("Has Cash/Credit");
	}
	else
	{
		GoToNextState("No Cash/Credit");
	}
}

void VendingMachine::ProcessIdleState()
{
	displayObj << "In idle state" << endl;
}

bool VendingMachine::GoToNextState(string transition)
{
	try
	{
		string newState = statesMachine.findEnd(currentState, transition);
		currentState = newState;


		// Take Action based on new state

		for (int i = 0; i < TOTAL_STATE; i++)
		{
			if (Atable[i].curState == currentState)

				(this->*Atable[i].cb)();
		}
		/*
		if (currentState == "ShowPrice")
		{
		DisplayPrice();

		}
		else if (currentState == "Update")
		{
		DisplayTotalCoins();
		}
		else if (currentState == "CancelCard")
		{
		CancelCreditTransaction();

		}
		else if (currentState == "DispenseDrink")
		{
		DispenseDrink();


		}
		else if (currentState == "CheckCard")
		{

		ProcessCreditCard();

		}
		else if (currentState == "DispenseChange")
		{
		RefundChange();

		}
		else if (currentState == "Idle")
		{
		ProcessIdleState();

		}
		else if (currentState == "InvalidPosition")
		{
		ProcessInvalidState();

		}
		*/
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
	bool isValidPosition = (prodList.find(ProdCodePushed) != prodList.end() && prodList[ProdCodePushed].notEmpty()) ;

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
	if (amt == 0.01 || amt == 0.05 || amt == 0.1 || amt == 0.25 || amt == 0.5 || amt == 1.00 || amt == 5.00)
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
			displayObj << "Too much cash in the machine. $" << amt << " has been returned." << endl; //format this later
		} // end if (not enough cash yet)
	}
	else
	{
		displayObj << "Cannot insert this type of cash! $" << amt << " has been returned." << endl;
	} // end if (valid amount of cash)
} // end insertCash

void VendingMachine::swipeCard(string cardNum)
{
	if (!cardNum.empty())
	{
		bool isDeclined = false;
		for (size_t i = 0; i < cardNum.size(); i++)
			if (!isdigit(cardNum[i]))
			{
				isDeclined = true;
			}
		if (!isDeclined)
		{
			char* endptr;
			ccNum = strtol(cardNum.c_str(), &endptr, 10);
			if ((total_coins < coin_max) && !paidByCreditCard)
			{
				if (GoToNextState("Card Swiped"))
				{
					// goes from Idle to CheckCard and then to Update or Idle
				} // end if (can go to next state)
			}
			else
			{
				//displayObj << "Card already used, no more credit has been added." << endl; //This should not do anything, just like a real machine
			} // end if (we have enough cash alreay (which could be because we already swipped a card))
		}
		else
		{
			// Card declined - do nothing
		}
	}
	else
	{
		// Decline card - do nothing
	}

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
		displayObj << "No card has been used, nothing to cancel." << endl;
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
		//displayObj << "Coin Return is not available!" << endl; //It should not do anything if it can't be done, just like a real machine
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
