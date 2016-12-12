/***************************************************
*	Final Project 2016
*	Team Water
*	Vending Machine implementation file
***************************************************/

#include "VendingMachine.h"

VendingMachine::VendingMachine() : displayObj(std::cout)
{
	coin_max = COIN_MAX;
	currentState = "Idle";
	// Build State Machine table - Connect states (vertices) with Action(Edges)	
	buildStateMachine();
	goToIdleState();
	displayObj << std::setprecision(2) << std::fixed;
} // end VendingMachine default constructor

VendingMachine::VendingMachine(std::ostream& obj) : displayObj(obj)
{
	coin_max = COIN_MAX;
	currentState = "Idle";
	// Build State Machine table - Connect states (vertices) with Action(Edges)	
	buildStateMachine();
	buildActionTable(); // Input new current state and callback function to complete action
	goToIdleState();
	displayObj << std::setprecision(2) << std::fixed;
} // end VendingMachine constructor

void VendingMachine::displayMenu() const
{
	displayObj << lineH << std::endl
		<< std::left << std::setw(2) << lineV << std::setw(67) << "Please type an Action, followed by an Entry, seperated by a space:" << std::right << std::setw(2) << lineV << std::endl
		<< std::left << std::setw(3) << lineV << std::setw(22) << "ACTION" << std::setw(44) << "ENTRY" << std::right << std::setw(2) << lineV << std::endl
		<< std::left << std::setw(3) << lineV << std::setw(22) << "Swipe" << std::setw(44) << "Card Number" << std::right << std::setw(2) << lineV << std::endl
		<< std::left << std::setw(3) << lineV << std::setw(22) << "InsertCash" << std::setw(44) << "0.01, 0.05, 0.10, 0.25, 0.50, 1.00, or 5.00" << std::right << std::setw(2) << lineV << std::endl
		<< std::left << std::setw(3) << lineV << std::setw(22) << "PressButton" << std::setw(44) << "Cancel" << std::right << std::setw(2) << lineV << std::endl
		<< std::left << std::setw(3) << lineV << std::setw(22) << "PressButton" << std::setw(44) << "CoinReturn" << std::right << std::setw(2) << lineV << std::endl
		<< std::left << std::setw(3) << lineV << std::setw(22) << "PressButton" << std::setw(44) << "A->Z, 0->9 (Example: \"D4\")" << std::right << std::setw(2) << lineV << std::endl
		<< lineH << std::endl;
}

void VendingMachine::buildActionTable()
{
	int i = 0;

	Atable[i].curState = "ShowPrice";
	Atable[i].cb = &VendingMachine::displayPrice;

	i++;
	Atable[i].curState = "Update";
	Atable[i].cb = &VendingMachine::displayTotalCoins;

	i++;
	Atable[i].curState = "dispenseDrink";
	Atable[i].cb = &VendingMachine::dispenseDrink;

	i++;
	Atable[i].curState = "CheckCard";
	Atable[i].cb = &VendingMachine::processCreditCard;

	i++;
	Atable[i].curState = "DispenseChange";
	Atable[i].cb = &VendingMachine::refundChange;

	i++;
	Atable[i].curState = "Idle";
	Atable[i].cb = &VendingMachine::processIdleState;

	i++;
	Atable[i].curState = "InvalidPosition";
	Atable[i].cb = &VendingMachine::processInvalidState;
} // end buildActionTable

void VendingMachine::buildStateMachine()
{
	int i = 0;

	statesMachine.Init(8);

	Edge<std::string, std::string> S_U("Idle", "Update", "Insert Cash");
	statesMachine.add(S_U);

	Edge<std::string, std::string> S_S("Idle", "Idle", "Insert Invalid Cash");
	statesMachine.add(S_S);

	Edge<std::string, std::string> U_U("Update", "Update", "Insert Cash");
	statesMachine.add(U_U);

	Edge<std::string, std::string> S_SP("Idle", "ShowPrice", "Valid Position");
	statesMachine.add(S_SP);

	Edge<std::string, std::string> S_IP("Idle", "InvalidPosition", "Invalid Position");
	statesMachine.add(S_IP);

	Edge<std::string, std::string> S_CH("Idle", "CheckCard", "Card Swiped");
	statesMachine.add(S_CH);

	Edge<std::string, std::string> SP_U("ShowPrice", "Update", "Has Cash");
	statesMachine.add(SP_U);

	Edge<std::string, std::string> SP_S("ShowPrice", "Idle", "No Cash");
	statesMachine.add(SP_S);

	Edge<std::string, std::string> U_SP("Update", "ShowPrice", "Valid Position & Not Enough Cash");
	statesMachine.add(U_SP);

	Edge<std::string, std::string> U_D("Update", "dispenseDrink", "Valid Position & Enough Cash/Credit");
	statesMachine.add(U_D);

	Edge<std::string, std::string> D_S("dispenseDrink", "Idle", "No Leftover Cash");
	statesMachine.add(D_S);

	Edge<std::string, std::string> D_DC("dispenseDrink", "DispenseChange", "Cash Leftover");
	statesMachine.add(D_DC);

	Edge<std::string, std::string> DC_S("DispenseChange", "Idle", "Change Dispensed");
	statesMachine.add(DC_S);

	Edge<std::string, std::string> IP_U("InvalidPosition", "Update", "Has Cash/Credit");
	statesMachine.add(IP_U);

	Edge<std::string, std::string> U_IP("Update", "InvalidPosition", "Invalid Position");
	statesMachine.add(U_IP);

	Edge<std::string, std::string> IP_S("InvalidPosition", "Idle", "No Cash/Credit");
	statesMachine.add(IP_S);

	Edge<std::string, std::string> U_CH("Update", "CheckCard", "Card Swiped");
	statesMachine.add(U_CH);

	Edge<std::string, std::string> CH_U("CheckCard", "Update", "Card Approved Or Declined and Has Cash");
	statesMachine.add(CH_U);

	Edge<std::string, std::string> CH_S("CheckCard", "Idle", "Card Declined and No Cash");
	statesMachine.add(CH_S);

	Edge<std::string, std::string> U_CA("Update", "CancelCard", "Has Credit and Card Cancel Pressed");
	statesMachine.add(U_CA);

	Edge<std::string, std::string> CA_U("CancelCard", "Update", "Has Cash");
	statesMachine.add(CA_U);

	Edge<std::string, std::string> CA_S("CancelCard", "Idle", "No Cash");
	statesMachine.add(CA_S);

	Edge<std::string, std::string> U_DC("Update", "DispenseChange", "Coin Return");
	statesMachine.add(U_DC);

	Edge<std::string, std::string> DC_U("DispenseChange", "Update", "Has Credit");
	statesMachine.add(DC_U);
} // end build state machine

void VendingMachine::goToIdleState()
{
	currentState = "Idle";
	paidByCreditCard = false;
	total_coins = 0;
} // end goToIdleState
// Following methods called by goToNextState

void VendingMachine::displayPrice()
{
	displayObj	<< lineH << std::endl
				<< std::left << std::setw(2) << lineV << "Price for selection " << prodCodePushed << " : $" << std::setw(41) << coin_max << std::right << std::setw(2) << lineV << std::endl;
	if (total_coins > 0)
	{
		goToNextState("Has Cash");
	}
	else
	{
		goToNextState("No Cash");
	}
} // end displayPrice

void VendingMachine::displayTotalCoins()
{
	displayObj	<< lineH << std::endl
				<< std::left << std::setw(2) << lineV << "Current amount: $" << std::setw(50) << total_coins << std::right << std::setw(2) << lineV << std::endl;
	displayMenu();
} // end displayTotalCoins

void VendingMachine::cancelCreditTransaction()
{
	paidByCreditCard = false;
	total_coins -= coin_max;
	displayObj	<< lineH << std::endl
				<< std::left << std::setw(2) << lineV << std::setw(67) << "Credit Card transaction canceled." << std::right << std::setw(2) << lineV << std::endl;
	if (total_coins > 0)
	{
		goToNextState("Has Cash");
	}
	else
	{
		goToNextState("No Cash");
	}
} // end cancelCreditTransaction

void VendingMachine::dispenseDrink()
{
	displayObj	<< lineH << std::endl
				<< std::left << std::setw(2) << lineV << "Dispensing " << std::setw(56) << prodList[prodCodePushed].getName() << std::right << std::setw(2) << lineV << std::endl;
	prodList[prodCodePushed].dispense();

	total_coins -= coin_max;
	paidByCreditCard = false;
	if (total_coins == 0)
	{
		goToNextState("No Leftover Cash");
	}
	else
	{
		goToNextState("Cash Leftover");
	}
} // end dispenseDrink

void VendingMachine::processCreditCard()
{
	CreditCard c(ccNum);

	bool is_card_approved = c.isValid();

	if (is_card_approved)
	{
		displayObj	<< lineH << std::endl
					<< std::left << std::setw(2) << lineV << std::setw(67) << "Credit Card approved." << std::right << std::setw(2) << lineV << std::endl;
		paidByCreditCard = true;
	}
	else
	{
		displayObj	<< lineH << std::endl
					<< std::left << std::setw(2) << lineV << std::setw(67) << "Credit Card declined." << std::right << std::setw(2) << lineV << std::endl;
	}

	total_coins += coin_max;
	if (is_card_approved || (!is_card_approved && total_coins > 0))
	{
		goToNextState("Card Approved Or Declined and Has Cash");
	}
	else if (!is_card_approved && total_coins == 0)
	{
		goToNextState("Card Declined and No Cash");
	}
} // end processCreditCard

void VendingMachine::refundChange()
{
	if (!paidByCreditCard)
	{
		displayObj	<< lineH << std::endl
					<< std::left << std::setw(2) << lineV << "Returning Change: $" << std::setw(48) << total_coins << std::right << std::setw(2) << lineV << std::endl;
		total_coins = 0;
		goToNextState("Change Dispensed");
	}
	else
	{
		displayObj	<< lineH << std::endl
					<< std::left << std::setw(2) << lineV << "Returning Change: $" << std::setw(48) << (total_coins-1.5) << std::right << std::setw(2) << lineV << std::endl;
		total_coins = coin_max;
		goToNextState("Has Credit");
	}
} // end refundChange

void VendingMachine::processInvalidState()
{
	displayObj	<< lineH << std::endl
				<< std::left << std::setw(2) << lineV << "There are no drinks in the slot " << std::setw(35) << prodCodePushed << std::right << std::setw(2) << lineV << std::endl;
	if (total_coins > 0)
	{
		goToNextState("Has Cash/Credit");
	}
	else
	{
		goToNextState("No Cash/Credit");
	}
} // end processInvalidState

void VendingMachine::processIdleState()
{
	displayObj	<< lineH << std::endl
				<< std::left << std::setw(2) << lineV << std::setw(67) << "Welcome to the Water World Vending Machine!" << std::right << std::setw(2) << lineV << std::endl;
	displayMenu();
} // end ProccessIdleState

bool VendingMachine::goToNextState(std::string transition)
{
	try
	{
		std::string newState = statesMachine.findEnd(currentState, transition);
		currentState = newState;

		// Take Action based on new state

		for (int i = 0; i < TOTAL_STATE; i++)
		{
			if (Atable[i].curState == currentState)
			{
				(this->*Atable[i].cb)();
				i = TOTAL_STATE;
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

void VendingMachine::pushButton(std::string prodCode)
{
	prodCodePushed = prodCode;
	bool isValidPosition = (prodList.find(prodCodePushed) != prodList.end() && prodList[prodCodePushed].notEmpty()) ;

	if (isValidPosition)
	{
		if (goToNextState("Valid Position"))
		{
			// Went to ShowPrice and then to Idle
		}
		else
		{
			if (total_coins >= coin_max)
			{
				if (goToNextState("Valid Position & Enough Cash/Credit"))
				{
					// Goes to dispenseDrink and then to Idle
				} // end if (can go to next state)
			}
			else
			{
				if (goToNextState("Valid Position & Not Enough Cash"))
				{
					// Goes to ShowPrice
				} // end if (can go to next state)
			} // end if (enough cash, or more)
		} // end if (can go to next state)
	}
	else
	{
		if (goToNextState("Invalid Position"))
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
			if (goToNextState("Insert Cash"))
			{
				// goes from Idle to Update or goes from Update to Update
			} // end if (can go to next state)
		}
		else
		{
			displayObj	<< lineH << std::endl
						<< std::left << std::setw(2) << lineV << "Too much cash in the machine. Returning Change: $" << std::setw(18) << amt << std::right << std::setw(2) << lineV << std::endl;
			if (goToNextState("Insert Cash"))
			{
				// goes from Update to Update
			} // end if (can go to next state)
		} // end if (not enough cash yet)
	}
	else
	{
		displayObj	<< lineH << std::endl
					<< std::left << std::setw(2) << lineV << "Inserted invalid cash type. Returning Change: $" << std::setw(20) << amt << std::right << std::setw(2) << lineV << std::endl;
		if (goToNextState("Insert Invalid Cash"))
		{
			// goes from idle to idle
		}
		else if (goToNextState("Insert Cash"))
		{
			// goes from Update to Update
		} // end if (can go to next state)
	} // end if (valid amount of cash)
} // end insertCash

void VendingMachine::swipeCard(std::string cardNum)
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
				if (goToNextState("Card Swiped"))
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
		if (goToNextState("Has Credit and Card Cancel Pressed"))
		{
			// goes from Update to CancelCard and then to Update or Idle
		} // end if (it can go to next state)
	}
	else
	{
		//displayObj << "No card has been used, nothing to cancel." << std::endl; //Does not display anything, just like the real machines
	} // end if (already used a card)
} // end cancleOrder

void VendingMachine::coinReturn()
{
	if (goToNextState("Coin Return"))
	{
		// goes from Update to CancelCard and then to Update or Idle
	}
	else
	{
		//displayObj << "Coin Return is not available!" << endl; //It should not do anything if it can't be done, just like a real machine
	} // end if (it can go to next state)
} // end voinReturn

bool VendingMachine::addSlot(std::string location, std::string Name, double Price, int Stock)
{
	prodList.insert(make_pair(location, Slot(Name, Price, Stock)));
	return true;
} // end addSlot

bool VendingMachine::getSlot(std::string location, std::string & Name, double & Price, int & Stock)
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
} // end getSlot

// END of Client functions
