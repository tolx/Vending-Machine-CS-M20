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
*/

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
void displayMachine(const vector<vector<drinkSlot>> inputMachine);
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
		endProgram(1);

	displayMachine(machine); //This is more for debugging, and probably won't be here on final version.
	cout << endl;

	if (!simulateMachine(machine))
		endProgram(2);

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
		if (filename == "C" || filename == "c")
			return false;
		cin.ignore();  // get rid of newline after filename entry
		inFile.clear();
		inFile.open(filename);
	} // end while

	char *endPtr; //for strtol
	//vector<vector<drinkSlot>> machine; //passed as an argument now
	vector<drinkSlot> row;
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
	return true;
}

void displayMachine(const vector<vector<drinkSlot>> inputMachine)
{
	if (inputMachine.size() != 4)
		return; //Error, there are not exactly 4 rows (A->D)

	char posL{ 'A' }, posN{ '1' }; //A = 65, 1 = 49
	for (size_t j{ 0 }; j < 4; j++) //letter rows A->D
	{
		if (inputMachine.at(j).size() != 9)
			return; //Error, atleast one row doesn't have exactly 9 colums

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

	if (false)
		return 1; //for file open cancled

	//File opened success
	cout << lineH << endl;

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