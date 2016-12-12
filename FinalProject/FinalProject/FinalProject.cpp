// FinalProject.cpp : Defines the entry point for the console application.
// CS M20 - Object Oreinted Data Structure and Algorithm Detection
/************************************************
*	Water World Vending Machines (WWVM)
*	AKA, Team Water
*	GitHub:
* https://github.com/tolx/Vending-Machine-CS-M20
*************************************************
*	GROUP MEMBER'S NAME	-	Git USERNAME		*
*-----------------------------------------------*
*	Sam Rice			-	RoboticRice			*
*	Ivan Martinez		-	tolx				*
*	Justin Kephart		-	AshnakAGQ			*
*	Kelton Malhotra		-	KeltonAM			*
*	Jake Lyon			-	jlyon805			*
*	Kiefer Solberg		-	potatoMoorpark		*
*	Riley Wallace		-	RileyWallace		*
*************************************************
* Program Status:
*	Complete:	Pending
*	Builds:		Yes
*	Errors:		No
************************************************/

//#pragma warning( disable: 4290 )  // disable warnings about the use of throw in function headers

#include "VendingMachine.h"
#include "PrecondViolatedExcept.h"
#include <iostream> //This is included in VendingMachine.h, but as this is the client file, it is good practice to include it here as well
#include <iomanip>
#include <fstream> //This is included in VendingMachine.h, but as this is the client file, it is good practice to include it here as well
#include <string> //This is included in VendingMachine.h, but as this is the client file, it is good practice to include it here as well
using namespace std;

void headerSplash();
ofstream openOutputFile();
bool inputMachine(VendingMachine &inputMachine);
char simulateMachine(VendingMachine &inputMachine, ostream& outFileObj);
void machineOutput(VendingMachine &inputMachine);
void footerSplash();
void programWait();
void endProgram(const int reason);

static const string lineH{ "---------------------------------------" }, lineV{ "|" }; //These are used for graphic-like interface

int main()
{
	headerSplash(); //Display the Header cout stuff, like the name of the program and who made it

	ofstream outFile = openOutputFile(); //This prompts the user to acknowledge we will be outputting to a file, and what file
	VendingMachine machine(outFile); //Creates the virtual vending machine

	if (!inputMachine(machine)) //Attempts to fill the vending machine with drinks
		endProgram(1); //Drinks (Machine) Input file cancel

	switch (simulateMachine(machine, outFile)) //Attempts to smulate user input in the machine
	{
		case 1:
			endProgram(2); //Input file cancel
		default:
			endProgram(4); //All other errors
		case 0:
			cout << "Simulation Complete!" << endl
				 << endl; //No errors, yay!
	}
	outFile.close();

	machineOutput(machine); //Saves the machine drink stock to an output file

	footerSplash(); //This essentially says goodbye to the user

	endProgram(0); //0 means success!!!
    return 0;
} // end main

/*******************************************/
// --- Begin Function Definitions ---
/*******************************************/

void headerSplash()
{
	cout << lineH << endl
		 << left << setw(2) << lineV << setw(35) << "Welcome to the WWVM VM001 Simulator"	<< right << setw(2) << lineV << endl
		 << left << setw(2) << lineV << setw(35) << "    CS M20 Final - Team Water"			<< right << setw(2) << lineV << endl
		 << lineH << endl
		 << left << setw(2) << lineV << setw(35) << "          Created By:"					<< setw(2) << right << lineV << endl
		 << left << setw(3) << lineV << setw(16) << "MEMBER'S NAME"		<< setw(3) << "-" << setw(15) << "Git USERNAME"		<< right << setw(2) << lineV << endl
		 << lineH << endl
		 << left << setw(3) << lineV << setw(16) << "Sam Rice"			<< setw(3) << "-" << setw(15) << "RoboticRice"		<< right << setw(2) << lineV << endl
		 << left << setw(3) << lineV << setw(16) << "Ivan Martinez"		<< setw(3) << "-" << setw(15) << "tolx"				<< right << setw(2) << lineV << endl
		 << left << setw(3) << lineV << setw(16) << "Justin Kephart"	<< setw(3) << "-" << setw(15) << "AshnakAGQ"		<< right << setw(2) << lineV << endl
		 << left << setw(3) << lineV << setw(16) << "Kelton Malhotra"	<< setw(3) << "-" << setw(15) << "KeltonAM"			<< right << setw(2) << lineV << endl
		 << left << setw(3) << lineV << setw(16) << "Jake Lyon"			<< setw(3) << "-" << setw(15) << "jlyon805"			<< right << setw(2) << lineV << endl
		 << left << setw(3) << lineV << setw(16) << "Kiefer Solberg"	<< setw(3) << "-" << setw(15) << "potatoMoorpark"	<< right << setw(2) << lineV << endl
		 << left << setw(3) << lineV << setw(16) << "Riley Wallace"		<< setw(3) << "-" << setw(15) << "RileyWallace"		<< right << setw(2) << lineV << endl
		 << lineH << endl
		 << left << setw(35) << " github.com/tolx/Vending-Machine-CS-M20" << right << endl
		 << lineH << endl
		 << endl;
} // end headerSplash

ofstream openOutputFile()
{
	string filename("VM001-Console-Output.txt");
	cout << "This program will write console output to the file:" << endl
		 << filename << endl;
	programWait();
	ifstream checkFile(filename);
	if (checkFile)
	{
		cout << filename << " already exists, do you want to overwrite? [Y]/[N]" << endl
			 << "Selecting [N]o will end the program." << endl;
		string input;
		cin >> input;
		cin.ignore();
		cout << endl;
		if (!(input == "Y" || input == "y" || input == "[Y]" || input == "[y]" || input == "Yes" || input == "yes" || input == "YES"))
			endProgram(3);
	}
	checkFile.clear();
	checkFile.close();
	ofstream outFile(filename, ios::trunc);
	return outFile;
} // end openOutputFile

bool inputMachine(VendingMachine &inputMachine)
{
	string filename("VM001-Drinks-Input.csv"), placeHolder("");
	cout << "This program needs to fill the machine using the inputfile:" << endl
		 << filename << endl;
	programWait();

	ifstream inFile(filename);
	while (!inFile)
	{
		cout << filename << " cannot be opened. Please enter another file name, or [C]ancel: ";
		cin >> filename;
		if (filename == "C" || filename == "c" || filename == "[C]ancel" || filename == "[C]" || filename == "[c]" || filename == "Cancel" || filename == "cancel") //for the smart @$$ out there
			return false;
		cin.ignore();
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
	return true;
}

char simulateMachine(VendingMachine &inputMachine, ostream& outFileObj)
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
		if (filename == "C" || filename == "c" || filename == "[C]ancel" || filename == "[C]" || filename == "[c]" || filename == "Cancel" || filename == "cancel") //for the smart @$$ out there
			return 1;
		cin.ignore();
		inFile.clear();
		inFile.open(filename);
	} // end while

	string action, entry;
	double cash;
	size_t position;
	while (!inFile.eof())
	{
	getline(inFile, action);
	entry = action;
	position = action.find_first_of(" ");
	if (!(position == string::npos || position == action.size()))
	{
		entry.erase(0, action.find_first_of(" ")+1);
		action.erase(action.find_first_of(" "), action.size());

        //outFileObj << "****" << action << " " << entry << "****" << endl; //<-for debugging
		outFileObj	<< action << " " << entry << endl
					<< endl;

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
	}
	return 0; //or any other number for an error at anypoint!
} // end simulateMachine

void machineOutput(VendingMachine &inputMachine)
{
	const string filename("VM001-Machine-Output.csv");
	cout << "This program will save the machine's drink stock to the file:" << endl
		 << filename << endl;
	programWait();
	ifstream checkFile(filename);
	if (checkFile)
	{
		cout << filename << " already exists, do you want to overwrite? [Y]/[N]" << endl;
		string input;
		cin >> input;
		cin.ignore();
		cout << endl;
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
				outFile << name << "," << stock;
				location.clear();
				location += c + to_string(i + 1);
				if (inputMachine.getSlot(location, name, price, stock))
					outFile << ",";
				else
					outFile << endl; //the last item is different
			}
		} //end for (letter rows)
	}

	outFile.close();
} // end machineOutput
	
void footerSplash()
{
	cout << lineH << endl
		 << left << setw(2) << lineV << setw(35) << "     Thank you for using the"		<< right << setw(2) << lineV << endl
		 << left << setw(2) << lineV << setw(35) << "   Water World Vending Machines"	<< right << setw(2) << lineV << endl
		 << left << setw(2) << lineV << setw(35) << "         VM001 Simulator"			<< right << setw(2) << lineV << endl
		 << lineH << endl;
} // end footerSplash

void programWait()
{
	cout << "Press Enter to continue...";
	cin.ignore();
	cout << endl;
} // end programWait

void endProgram(const int reason)
{
	//Inspired by the same named function written by Prof.
	cout << endl
		 << "Program ending ";
	cout << ((reason == 0) ? "successfully." : ((reason < 4) ? "due to user canceling the program." : "unexpectantly due to error(s)!")) << endl;
	cout << endl
		 << "Press Enter to end";
	cin.ignore();
	exit(reason);
	/*
	ERROR LIST:
	0 - No Errors
	1 - Canceled Filling the Machine with Drinks from an Input File
	2 - Canceled Providing Commands for the Machines from an Input File
	3 - Canceled Writing Console Output to an Output File
	4 - Other Errors Related to SimulateMachine
	*/
} // end endProgram
