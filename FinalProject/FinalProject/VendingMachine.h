/*
CS M20 - Final Project
Team Water
Members who worked on this:
Kelton Malhotra
Justin Kephart
Riley Wallace
*/

#include "Graph.h"
#include "Timer.h"
#include <fstream>
#include <iostream>

using namespace std;

static const double COIN_MAX = 1.50;

class VendingMachine
{
private:
	Graph<string, string> statesMachine;
	map <string, class Slot> prodList;
    ostream& displayObj;
    Timer systemTimer;

	void BuildProdList();
	void BuildStateMachine();
	void GoToIdleState();

	bool GoToNextState(string transition);

	double coin_max;
	string ProdCodePushed;
	bool  paidByCreditCard;
	double total_coins;
	string currentState;

public:
    VendingMachine();
    VendingMachine(ostream& obj);

	void pushButton(string prodCode);
	void insertCash(double amt);
	void swipeCard(string cardType);
	void cancelOrder();
	void coinReturn();
	bool addSlot(string location, string Name = "", double Price = 0, int Stock = 0);
	bool getSlot(string location, string &Name, double &Price, int &Stock);
};

class Slot
{
private:
	string name;
	double  price; // in cents
	int stock;
public:
	Slot(string Name = "", double Price = 0, int Stock = 0)
		: name(Name), price(Price), stock(Stock) {}

	string	getName() { return name; }
	double	getPrice() { return price; }
	int		getStock() { return stock; }

	void	setName(string Name) { name = Name; }
	void	setPrice(double Price) { price = Price; }
	void	setStock(int Stock) { stock = Stock; }

};