#include "Products.h"


prod::prod(string pName, double pPrice)
{
	prodName = pName;
	prodPrice = pPrice;
}
string prod::GetProdName()
{
	return prodName;
}

double prod::GetProdPrice()
{
	return prodPrice;
}
