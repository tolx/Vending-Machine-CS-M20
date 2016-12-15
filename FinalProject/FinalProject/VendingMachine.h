/***************************************************
*	Final Project 2016
*	Team Water
*	Vending Machine header file
***************************************************/

#include "Graph.h"
#include "CreditCard.h"
#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <iomanip>

using namespace std;
//#include <map> //This is not required, as we know Graph.h includes it, and this will always require Graph.h

static const int TOTAL_STATE = 8;

class VendingMachine
{
private:
	Graph<std::string, std::string> statesMachine;
	std::map<std::string, class Slot> prodList;
	std::ostream& displayObj;

	void buildStateMachine();
	void goToIdleState();
	void buildActionTable();

	bool goToNextState(std::string transition);

	double coin_max;
	std::string prodCodePushed;
	bool  paidByCreditCard;
	int   ccNum; // Credit card Number 
	double total_coins;
	std::string currentState;

	const std::string lineH{ "***********************************************************************" }, lineV{ "*" }; //These are used for graphic-like interface
	void displayMenu() const;

private: // called by GotoNextState
	void displayPrice();
	void displayTotalCoins();

	void cancelCreditTransaction();
	void dispenseDrink();
	void processCreditCard();
	void processIdleState();
	void refundChange();

	void processInvalidState();

private:// Action table - Execeute callback based on new current state
	typedef void (VendingMachine::*DoAction)(void);

	struct ActionTable
	{
		std::string curState;
		DoAction cb;
	};
	ActionTable Atable[TOTAL_STATE];

	std::unique_ptr<CreditCard> CC;

public:
	VendingMachine();
	VendingMachine(std::ostream& obj);

	void pushButton(std::string prodCode);
	void insertCash(double amt);
	void swipeCard(std::string cardType);
	void cancelOrder();
	void coinReturn();
	bool addSlot(std::string location, std::string Name = "", double Price = 0, int Stock = 0);
	bool getSlot(std::string location, std::string &Name, double &Price, int &Stock);
};

class Slot
{
private:
	std::string name;
	double price; // in cents
	int stock;
public:
	Slot(std::string Name = "", double Price = 0, int Stock = 0)
		: name(Name), price(Price), stock(Stock) {}

	std::string	getName() { return name; }
	double	getPrice() { return price; }
	int		getStock() { return stock; }

	void	setName(std::string Name) { name = Name; }
	void	setPrice(double Price) { price = Price; }
	void	setStock(int Stock) { stock = Stock; }

	bool notEmpty() { return stock > 0; }
	std::string dispense() { stock--; return name + " dispensed"; }
};
