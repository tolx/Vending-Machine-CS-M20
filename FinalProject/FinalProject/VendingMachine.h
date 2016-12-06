/*
CS M20 - Final Project
Team Water
Members who worked on this:
Kelton Malhotra
*/

#include "Products.h"

enum States
{
	IDLE,
	DISPLAY_PRICE,
	CARD_ACCEPTED,
	CASH_INSERTED,
	REFUND,
	DISPENCE_PRODUCT

};
enum Actions
{
	PUSH_BUTTON,
	INSERT_CASH,
	SWIPE_CREDIT_CARD,
	CANCEL_ORDER

};




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

	map <string, class prod> prodList;

	typedef void (VendingMachine::*DoAction)(void);


	struct StateMachine
	{
		int curState;
		int action;
		DoAction cb;

	};
	StateMachine sm[20];
	void BuildProdList();
	void BuildStateMachine();
	void GoToIDleState();

	// Callbacks called by state machine

	void DispenseProduct();
	void DisplayProdPrice();
	void CardSwiped();
	void CashInserted();
	void OrderCancelled();


	void CancelCardTransaction();
	void Refund(double coins);
	bool GoToNextState(int action);

	string ProdCodePushed;
	bool  paidByCreditCard;
	double total_coins;
	int currentState;


public:
	VendingMachine();

	void PushButton(string prodCode);
	void InsertCash(int amt);
	void swipeCard(string cardType);
	void CancelOrder();

};
