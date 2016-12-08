/*
CS M20 - Final Project
Team Water
Members who worked on this:
Kelton Malhotra
*/

#include "Products.h"
#include "Graph.h"

/*
enum States
{
	IDLE,
	DISPLAY_PRICE,
	CARD_ACCEPTED,
	CASH_INSERTED,
	REFUND,
	DISPENCE_PRODUCT

};
*/


class VendingMachine
{
private:

	/*
	Prod List (key = prodCode)
	Prodct code , prod name and prices

	D4 , "soda ", 70
	D5 , Water, 80
	D6 , Juice, 90
	*/

	Graph<string, string> statesMachine;

	map <string, class prod> prodList;

	/*
	typedef void (VendingMachine::*DoAction)(void);
	*/

	void BuildProdList();
	void BuildStateMachine();
	void GoToIDleState();

	// Callbacks called by state machine

	/*
	void DispenseProduct();
	void DisplayProdPrice();
	void CardSwiped();
	void CashInserted();
	void OrderCancelled();
	

	void CancelCardTransaction();
	void Refund(double coins);

	*/

	bool GoToNextState(string transition);

	string ProdCodePushed;
	bool  paidByCreditCard;
	double total_coins;
	string currentState;
	


public:
	VendingMachine();

	void PushButton(string prodCode);
	void InsertCash(double amt);
	void swipeCard(string cardType);
	void CancelOrder();
	void CoinReturn();

};
