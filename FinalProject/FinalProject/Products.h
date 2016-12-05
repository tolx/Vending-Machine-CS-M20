#include <map>
#include <iostream>
#include <string>
using namespace std;

class prod
{
private:
	string prodName;
	double  prodPrice; // in cents
public:
	prod(string pName, double pPrice);
	string GetProdName();
	double GetProdPrice();




};