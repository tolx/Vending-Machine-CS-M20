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
	void BuildProdList();
	void GoToIDleState();
	void DispenseProduct(string prodName);
	void DisplayProdPrice(string prodCode);
	void CancelCardTransaction();
	void Refund(double coins);
	bool paidByCreditCard;
	double  total_coins;
	int currentState;

public:
	VendingMachine();

	void PushButton(string prodCode);
	void InsertCash(double amt);
	void swipeCard(string cardType);
	void CancelOrder();

};