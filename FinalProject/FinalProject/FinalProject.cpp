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

struct drinkSlot 
{
	string name;
	int amount;
};

bool inputMachine(vector<vector<drinkSlot>> &inputMachine);
void displayMachine(const vector<vector<drinkSlot>> inputMachine) throw(PrecondViolatedExcept);
char simulateMachine(vector<vector<drinkSlot>> &inputMachine);
void consoleOutput();
void machineOutput(const vector<vector<drinkSlot>> inputMachine);
void programWait();
void endProgram(const int reason);

const string lineH{ "----------------------------------------------" }, lineV{ "|" };

int main()
{
	cout << lineH << endl
		 << lineV << setw(40) << "Welcome to the WWVM VM001 Simulator!" << setw(5) << lineV << endl
		 << lineH << endl
		 << endl;
	
	vector<vector<drinkSlot>> machine;
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

	consoleOutput(); //this might merge with machineOutput, or we might not even have machine output on the final version
	//^ or this might be ongoing in the simulate machine function...

	machineOutput(machine);

	cout << lineH << endl
		 << "  Thank you for using the" << endl
		 << "Water World Vending Machines" << endl
		 << "      VM001 Simulator" << endl
		 << lineH << endl;

	endProgram(0);
    return 0;
} // end main

bool inputMachine(vector<vector<drinkSlot>> &inputMachine)
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
	//vector<vector<drinkSlot>> machine; //passed as an argument now
	inputMachine.clear(); //This function will empty the machine and fill exactly per the input file, any previous drinks will no longer be present
	inputMachine.reserve(4); //reserving the space before push_back apparently is more effecient, according to http://lemire.me/blog/2012/06/20/do-not-waste-time-with-stl-vectors/
	vector<drinkSlot> row;
	row.reserve(9); //reserve does NOT change the .size, and push_back still starts at 0
	drinkSlot input;
	for (size_t j{ 0 }; j < 4; j++) //letter rows A->D
	{
		getline(inFile, placeHolder); //for each letter row
									  //cout << placeHolder << endl;
		for (size_t i{ 1 }; i <= 9 && !placeHolder.empty() && placeHolder.find_first_of(',') != 0; i++) //numbered colums 1->9
		{
			input.name = placeHolder.substr(0, placeHolder.find_first_of(',')); //get the name
			placeHolder.erase(0, placeHolder.find_first_of(',') + 1); //remove the name from the file
			if (!placeHolder.empty() && placeHolder.find_first_of(',') != 0) //there is still content
			{
				input.amount = static_cast<int>(strtol(placeHolder.substr(0, placeHolder.find_first_of(',')).c_str(), &endPtr, 10)); //get the quantity and convert to int
				placeHolder.erase(0, placeHolder.find_first_of(',') + 1); //remove the quantity from the file
			}
			else
			{
				input.amount = 0;
			} // end if
			row.push_back(input);
		} // end for (numbered colums)
		inputMachine.push_back(row);
		row.clear();
	} // end for (letter rows)
	inFile.close();


	//Now, inspect the machine to ensure every slot is filled with something. If not, fill it with Empty 0
	input.amount = 0;
	input.name = "Empty";
	//row.clear(); redundant, the last thing row did is clear()

	if (inputMachine.size() != 4)
	{
		for (size_t i{ 0 }; i < 9; i++)
			row.push_back(input); //generate a row of 9 empty inputs
		inputMachine.resize(4, row); //resize will remove extra data, or fill missing data with empty row
	}

	for (size_t j{ 0 }; j < 4; j++) //letter rows A->D
	{
		if (inputMachine.at(j).size() != 9)
			inputMachine.at(j).resize(9, input); //resize will remove extra data, or fill missing data with empty input
	}

	return true;
}

//Pre-Condition: There are exactly 4 rows, and exactly 9 colums for every row
void displayMachine(const vector<vector<drinkSlot>> inputMachine) throw(PrecondViolatedExcept)
{
	if (inputMachine.size() != 4)
		throw PrecondViolatedExcept("Incorrect rows in Machine"); //Error, there are not exactly 4 rows (A->D)

	char posL{ 'A' }, posN{ '1' }; //A = 65, 1 = 49
	for (size_t j{ 0 }; j < 4; j++) //letter rows A->D
	{
		if (inputMachine.at(j).size() != 9)
			throw PrecondViolatedExcept("Incorrect colums in the Machine"); //Error, atleast one row doesn't have exactly 9 colums

		for (size_t i{ 0 }; i < 9; i++) //numbered colums 1->9
			cout << (posL = 65 + (char)j) << (posN = 49 + (char)i) << " = " << inputMachine.at(j).at(i).name << ", " << inputMachine.at(j).at(i).amount << endl;
	} //end for (letter rows)
} // end displayMachine

char simulateMachine(vector<vector<drinkSlot>> &inputMachine)
{
	string filename("VM001-Command-Input.txt");
	cout << "This program needs to input commands using the inputfile:" << endl
		 << filename << endl;
	programWait();

	ifstream inFile(filename);
	while (!inFile)
	{
		cout << filename << " cannot be opened. Please enter another file name, or [C]ancle: ";
		cin >> filename;
		if (filename == "C" || filename == "c" || filename == "[C]ancle" || filename == "[C]" || filename == "[c]" || filename == "Cancle") //for the smart @$$ out there
			return 1;
		cin.ignore();  // get rid of newline after filename entry
		inFile.clear();
		inFile.open(filename);
	} // end while
	
	//File opened success
	VendingMachine vm;
	/*vm.PushButton("D4");
	vm.InsertCash(1.50);
	vm.PushButton("D6");
	vm.swipeCard("VISA");
	vm.PushButton("D4");
	vm.swipeCard("AMX");
	vm.PushButton("D4");
	vm.InsertCash(0.70);
	vm.CancelOrder();*/

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
				vm.CancelOrder();
			}
			else if (entry == "CoinReturn")
			{
				vm.CoinReturn();
			}
			else
			{
				vm.PushButton(entry);
			}
		}
		else if (action == "Swipe")
		{
			vm.swipeCard(entry);
		}
		else if (action == "InsertCash")
		{
			cash = stod(entry);
			vm.InsertCash(cash);
		}

	}

	return 0; //or any other number for an error at anypoint!
} // end simulateMachine

void consoleOutput()
{
	const string filename("VM001-Console-Output.txt");
	cout << "This program has completed running. You can find the console data in the file:" << endl
		 << filename << endl;
} // end consoleOutput

void machineOutput(const vector<vector<drinkSlot>> inputMachine)
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

	//Output current amount of drinks
	if (inputMachine.size() != 4)
		return; //Error, there are not exactly 4 rows (A->D)

	for (size_t j{ 0 }; j < 4; j++) //letter rows A->D
	{
		if (inputMachine.at(j).size() != 9)
			return; //Error, atleast one row doesn't have exactly 9 colums

		for (size_t i{ 0 }; i < 8; i++) //numbered colums 1->9
			outFile << inputMachine.at(j).at(i).name << "," << inputMachine.at(j).at(i).amount << ",";
		outFile << inputMachine.at(j).at(8).name << "," << inputMachine.at(j).at(8).amount << endl; //the last item is different
	} //end for (letter rows)

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