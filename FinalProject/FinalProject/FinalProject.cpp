// FinalProject.cpp : Defines the entry point for the console application.
// CS M20 - Object Oreinted Data Structure and Algorithm Detection
/*	Team Water
* Group Members:
*	Sam Rice
*	Ivan Martinez
*	Justin Kephart
*	Kelton Malhotra
*	Jake Lyon
*	Kiefer Soldberg
*	Riley Wallace
* Program Status:
*	Complete:	No
*	Builds:		Yes
*	Errors:		No
* Members who worked on this file:
*   Sam Rice
*   Kelton Malhotra
*/

#pragma warning( disable: 4290 )  // disable warnings about the use of throw in function headers

#include "VendingMachine.h"
#include "PrecondViolatedExcept.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


bool inputMachine(VendingMachine &inputMachine);
void displayMachine(VendingMachine &inputMachine) throw(PrecondViolatedExcept);
char simulateMachine(VendingMachine &inputMachine);
void machineOutput(VendingMachine &inputMachine);
void programWait();
void endProgram(const int reason);

const string lineH{ "----------------------------------------------" }, lineV{ "|" };

int main()
{
	cout << lineH << endl
		 << lineV << setw(40) << "Welcome to the WWVM VM001 Simulator!" << setw(5) << lineV << endl
		 << lineH << endl
		 << endl;
	
	VendingMachine machine;

	if (!inputMachine(machine))
		endProgram(1); //Input file cancel

	try
	{
		displayMachine(machine); //This is more for debugging, and probably won't be here on final version.
		cout << endl;
	}
	catch (PrecondViolatedExcept &exc)
	{
		cout << exc.what();
	}

	switch (simulateMachine(machine))
	{
		case 1:
			endProgram(2); //Input file cancle
		default:
			endProgram(3); //All other errors
		case 0:
			cout << endl;//Do nothing, no errors!
	}

	machineOutput(machine);

	cout << lineH << endl
		 << "  Thank you for using the" << endl
		 << "Water World Vending Machines" << endl
		 << "      VM001 Simulator" << endl
		 << lineH << endl;

	endProgram(0);
    return 0;
} // end main

bool inputMachine(VendingMachine &inputMachine)
{
	string filename("VM001-Drinks-Input.csv"), placeHolder("");

	cout << "This program needs to fill the machine using the inputfile: " << endl
		 << filename << endl;
	programWait();

	ifstream inFile(filename);
	while (!inFile)
	{
		cout << filename << " cannot be opened. Please enter another file name, or [C]ancle: ";
		cin >> filename;
		if (filename == "C" || filename == "c" || filename == "[C]ancle" || filename == "[C]" || filename == "[c]" || filename == "Cancle") //for the smart @$$ out there
			return false;
		cin.ignore();  // get rid of newline after filename entry
		inFile.clear();
		inFile.open(filename);
	} // end while

	char *endPtr; //for strtol
	
	string name;
	double price = COIN_MAX;
	int stock;

	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	for (char c : alphabet)
	{
		if (inFile.peek() == EOF)
			break;

		getline(inFile, placeHolder); //for each letter row
									  //cout << placeHolder << endl;
		
		for (size_t i{ 1 }; i <= 9 && !placeHolder.empty() && placeHolder.find_first_of(',') != 0; i++) //numbered colums 1->9
		{
			name = placeHolder.substr(0, placeHolder.find_first_of(',')); //get the name
			placeHolder.erase(0, placeHolder.find_first_of(',') + 1); //remove the name from the file
			
			if (!placeHolder.empty() && placeHolder.find_first_of(',') != 0) //there is still content
			{
				stock = static_cast<int>(strtol(placeHolder.substr(0, placeHolder.find_first_of(',')).c_str(), &endPtr, 10)); //get the quantity and convert to int
				placeHolder.erase(0, placeHolder.find_first_of(',') + 1); //remove the quantity from the file
			}

			else
			{
				stock = 0;
			} // end if

			string location(1,c);
			location += to_string(i);
			inputMachine.addSlot(location, name, price, stock);
		} // end for (numbered colums)
	} // end for (letter rows)
	inFile.close();


	////Now, inspect the machine to ensure every slot is filled with something. If not, fill it with Empty 0
	//input.amount = 0;
	//input.name = "Empty";
	////row.clear(); redundant, the last thing row did is clear()

	//if (inputMachine.size() != 4)
	//{
	//	for (size_t i{ 0 }; i < 9; i++)
	//		row.push_back(input); //generate a row of 9 empty inputs
	//	inputMachine.resize(4, row); //resize will remove extra data, or fill missing data with empty row
	//}

	//for (size_t j{ 0 }; j < 4; j++) //letter rows A->D
	//{
	//	if (inputMachine.at(j).size() != 9)
	//		inputMachine.at(j).resize(9, input); //resize will remove extra data, or fill missing data with empty input
	//}

	return true;
}

//Pre-Condition: There are exactly 4 rows, and exactly 9 colums for every row
void displayMachine(VendingMachine &inputMachine) throw(PrecondViolatedExcept)
{
	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string name;
	double price = COIN_MAX;
	int stock;

	for (char c : alphabet)
	{
		for (size_t i{ 1 }; i <= 9; i++) //numbered colums 1->9
		{
			string location(1, c);
			location += to_string(i);
			if (inputMachine.getSlot(location, name, price, stock))
				cout << c << i << " = " << name << ", " << stock << endl;
		}
	} //end for (letter rows)
} // end displayMachine

char simulateMachine(VendingMachine &inputMachine)
{
	string filename("VM001-Command-Input.txt");
	cout << "This program needs to input commands using the inputfile:" << endl
		 << filename << endl;
	programWait();

	ifstream inFile(filename);
	while (!inFile)
	{
		cout << filename << " cannot be opened. Please enter another file name, or [C]ancel: ";
		cin >> filename;
		if (filename == "C" || filename == "c" || filename == "[C]ancel" || filename == "[C]" || filename == "[c]" || filename == "Cancel") //for the smart @$$ out there
			return 1;
		cin.ignore();  // get rid of newline after filename entry
		inFile.clear();
		inFile.open(filename);
	} // end while
	
	//File opened success
	/*inputMachine.pushButton("D4");
	inputMachine.insertCash(1.50);
	inputMachine.pushButton("D6");
	inputMachine.swipeCard("VISA");
	inputMachine.pushButton("D4");
	inputMachine.swipeCard("AMX");
	inputMachine.pushButton("D4");
	inputMachine.insertCash(0.70);
	inputMachine.cancelOrder();*/

	string action, entry;
	double cash;
	while (!inFile.eof())
	{

		inFile >> action;
		inFile >> entry;

		inFile.ignore();

		cout << "****Simulating the command " << action << " " << entry << "****" << endl;
		if (action == "PressButton")
		{
			if (entry == "Cancel")
			{
				inputMachine.cancelOrder();
			}
			else if (entry == "CoinReturn")
			{
				inputMachine.coinReturn();
			}
			else
			{
				inputMachine.pushButton(entry);
			}
		}
		else if (action == "Swipe")
		{
			inputMachine.swipeCard(entry);
		}
		else if (action == "InsertCash")
		{
			cash = stod(entry);
			inputMachine.insertCash(cash);
		}

	}

	return 0; //or any other number for an error at anypoint!
} // end simulateMachine

void machineOutput(VendingMachine &inputMachine)
{
	const string filename("VM001-Machine-Output.csv");
	ifstream checkFile(filename);
	if (checkFile)
	{
		cout << filename << " already exists, do you want to overwrite? [Y]/[N]" << endl;
		string input;
		cin >> input;
		cin.ignore();  // get rid of newline after filename entry
		if (!(input == "Y" || input == "y" || input == "[Y]" || input == "[y]" || input == "Yes" || input == "yes" || input == "YES"))
			return;
	}
	checkFile.clear();
	checkFile.close();
	ofstream outFile(filename, ios::trunc);

	string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string name;
	double price = COIN_MAX;
	int stock;

	for (char c : alphabet)
	{
		for (size_t i{ 1 }; i <= 9; i++) //numbered colums 1->9
		{
			string location(1, c);
			location += to_string(i);
			if (inputMachine.getSlot(location, name, price, stock))
			{
				location.clear();
				location += c + to_string(i + 1);
				if (inputMachine.getSlot(location, name, price, stock))
					outFile << name << "," << stock << ",";
				else
					outFile << name << "," << stock << endl; //the last item is different
			}
		} //end for (letter rows)
	}
		
	outFile.close();
	cout << "And the vending machine quantities in the file:" << endl
		 << filename << endl;
} // end machineOutput
	
void programWait()
{
	cout << "Press Enter to continue...";
	cin.ignore();
	cout << endl
		 << endl;
} // end programWait

void endProgram(const int reason)
{
	//Inspired by the same named function written by Prof.
	//It's functionally the exact same function
	cout << endl
		 << endl
		 << "Program ending ";
	cout << ((reason == 0) ? "successfully." : "unexpectantly due to error(s)!") << endl;
	cout << endl
		 << "Press Enter to end";
	cin.ignore();
	exit(reason);
} // end endProgram