/*
CS M20 - Final Project
Team Water
Members who worked on this:
Justin Kephart
*/

#include <string>
using namespace std;

class Slot
{
private:
	string name;
	double  price; // in cents
	int stock;
public:
	Slot(string Name = "", double Price = 0, int Stock = 0)
		: name(Name), price(Price), stock(Stock) {}

	string	getName()		{ return name; }
	double	getPrice()		{ return price; }
	int		getStock()		{ return stock; }

	void	setName(string Name)	{ name  = Name; }
	void	setPrice(double Price)	{ price = Price; }
	void	setStock(int Stock)		{ stock = Stock; }

};