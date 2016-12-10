/*
CS M20 - Final Project
Team Water
Members who worked on this:
Kelton Malhotra
Justin Kephart
*/

#include "Slot.h"
#include "Graph.h"

static const double COIN_MAX = 1.50;

class VendingMachine
{
private:
	Graph<string, string> statesMachine;
	map <string, class prod> prodList;

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

	void pushButton(string prodCode);
	void insertCash(double amt);
	void swipeCard(string cardType);
	void cancelOrder();
	void coinReturn();
};
