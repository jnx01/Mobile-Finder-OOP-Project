#pragma warning (disable:4996)				//So that compiler allows us to use localtime() function in time.h library
#include <iostream>
#include <string>
#include <fstream>
#include <time.h>							//for date and time manipulation
#include <iomanip>
#include <stdlib.h>							//to use system function
#include <windows.h>						//to use sleep function
#include "phone.h"
#include "inventory.h"
#include "search.h"
#include "sales.h"

using namespace std;

char inventoryFile[] = { "InventoryData.txt" };			//Name of file that holds all inventory info. Would have been a waste of memory if all Inventory objects had a copy of this
char invoiceFile[] = { "SalesInvoice.txt" };
char reportFile[] = { "SalesReport.txt" };


//Defining '>>' and '<<' friend operator overloaded functions declared in Phone class

istream& operator >> (istream& userInput, Phone& emptyRecord)
{
	cin.ignore();
	cout << "\n\nEnter model: ";
	getline(userInput, emptyRecord.model);
	cin.ignore();
	cout << "Enter brand: ";
	getline(userInput, emptyRecord.brand);
	cin.ignore();
	cout << "Enter price: ";
	userInput >> emptyRecord.price;
	cin.ignore();
	cout << "\nEnter OS: ";
	getline(userInput, emptyRecord.os);
	cin.ignore();
	cout << "Enter RAM size (in GBs): ";
	userInput >> emptyRecord.ram;
	cin.ignore();
	cout << "Enter screen size (in Inches): ";
	userInput >> emptyRecord.screenSize;
	cin.ignore();
	cout << "Enter camera magnitude (in MPs): ";
	userInput >> emptyRecord.cam;
	cin.ignore();
	cout << "Enter build: ";
	getline(userInput, emptyRecord.build);
	cin.ignore();
	return userInput;
}

ostream& operator << (ostream& output, const Phone& filledRecord)
{
	output << "\nModel: " << filledRecord.model;
	output << "\nBrand: " << filledRecord.brand;
	output << "\nPrice: Rs. " << filledRecord.price;
	output << "\nOS: " << filledRecord.os;
	output << "\nRAM: " << filledRecord.ram << " GB";
	output << "\nScreen size: " << filledRecord.screenSize << " Inches";
	output << "\nCamera: " << filledRecord.cam << " MP";
	output << "\nBuild: " << filledRecord.build;
	return output;
}




//Phone class functions defined below

Phone::Phone() : model(""), brand(""), price(0), os(""), ram(0), screenSize(0.0), cam(0), build("")
{}

Phone::Phone(string a, string b, int c, string d, int e, float f, int g, string h) : model(a), brand(b), price(c), os(d), ram(e), screenSize(f), cam(g), build(h)
{}




//Defining friend functions declared in Inventory class

ifstream& inputInventoryFromFile(ifstream& in, Inventory& emptyRecord)
{
	string temp;
	getline(in, emptyRecord.model);
	getline(in, emptyRecord.brand);
	getline(in, temp);
	emptyRecord.price = stoi(temp);
	getline(in, emptyRecord.os);
	getline(in, temp);
	emptyRecord.ram = stoi(temp);
	getline(in, temp);
	emptyRecord.screenSize = stof(temp);
	getline(in, temp);
	emptyRecord.cam = stoi(temp);
	getline(in, emptyRecord.build);
	getline(in, temp);
	emptyRecord.inStock = stoi(temp);
	return in;
}

ofstream& outputInventoryToFile(ofstream& out, const Inventory& filledRecord)			//functions can't modify const objects
{
	out << endl << filledRecord.model
		<< endl << filledRecord.brand
		<< endl << filledRecord.price
		<< endl << filledRecord.os
		<< endl << filledRecord.ram
		<< endl << filledRecord.screenSize
		<< endl << filledRecord.cam
		<< endl << filledRecord.build
		<< endl << filledRecord.inStock;
	return out;
}




//Inventory class functions defined below

Inventory::Inventory() : Phone::Phone()
{
	inStock = 0;
}

Inventory::Inventory(std::string a, std::string b, int c, std::string d, int e, float f, int g, string h, int i) : Phone::Phone(a, b, c, d, e, f, g, h)
{
	inStock = i;
}

void Inventory::inputRecord()
{
	cin >> *this;												//calling operator overloaded function using "this" pointer
	cout << "Enter number of units in stock: ";
	cin >> inStock;
	cin.ignore();
}

void Inventory::outputRecord()
{
	cout << *this;												//calling operator overloaded function
	cout << "\nUnits in stock: " << inStock;
}

void Inventory::addRecord()
{
	Inventory newRecord;										//newRecord stores the info of the new phone that will be added to the file

	ofstream output(::inventoryFile, ios::app);					//Ensures new record gets written in the end
	if (!output)
	{
		cout << "\n\nError opening file.\n";
	}
	else
	{
		newRecord.inputRecord();								//Taking user input
		outputInventoryToFile(output, newRecord);				//Outputting new record to inventory file

		output.close();
		cout << "\n\nRecord added successfully.";
	}
}

void Inventory::editRecord()
{
	string phoneModel, line;										//PhoneModel stores the model user is searching (for editting), line stores line read from file, temp is just a temporary data holder
	Inventory oldRecord, newRecord;										//oldRecord holds values of record to be editted. newRecord hold values that will replace oldRecord
	int check = 0, confirmation = 0;									//check is used to check whether specified model is in file. confirmation is to confirm whether user wants to edit the record

	ifstream input(::inventoryFile, ios::in);
	if (!input)
	{
		cout << "\n\nError opening file.\n";
	}
	else
	{
		cout << "\n\nEnter the model you want to edit: ";
		cin >> phoneModel;
		getline(input, line);										//Ignoring the empty line in start of file
		while (!input.eof())
		{
			inputInventoryFromFile(input, oldRecord);							//reading records from file

			if (oldRecord.model == phoneModel)
			{
				check = 1;														//if specified model found, break loop
				break;
			}

		}
		input.close();

		if (check == 0)															//operation cancelled if model not found
		{
			cout << "\n\nSpecified model does not exit in database.\n";
		}
		else
		{
			Inventory* old = &oldRecord;
			old->outputRecord();
			cout << "\n\nEnter 1 to edit this record or 0 to cancel.\n\n";		//taking user confirmation
			cin >> confirmation;

			if (confirmation == 1)
			{
				cin.ignore();
				cout << "\nEnter new values.\n";								//taking in new values
				newRecord.inputRecord();

				ifstream in(::inventoryFile, ios::in);
				ofstream out("NewInventoryFile.txt", ios::out);

				if (!in)
				{
					cout << "\n\nError opening file.\n";
				}
				else
				{
					getline(in, line);										//Ignoring the empty line in start of file
					while (!in.eof())
					{
						getline(in, line);
						if (line == oldRecord.model)							//if line matches data "model" in oldRecord, store newRecord in file
						{
							outputInventoryToFile(out, newRecord);			//outputting new record to file

							for (int i = 0; i < 8; ++i)						//loop to skip oldRecord values in original inventory file
							{
								getline(in, line);
							}
						}
						else
						{
							out << endl << line;							//Storing line as-is
						}
					}
					in.close();
					out.close();

					remove(::inventoryFile);
					int result = rename("NewInventoryFile.txt", ::inventoryFile);		//Deleting old file and renaming new file to old one
					if (result == 0)
					{
						cout << "\n\nRecord editted successfully.";
					}
					else
					{
						cout << "\n\nOperation failed.\n";
					}
				}
			}
			else
			{
				cout << "\n\nOperation cancelled.\n";
			}
		}
	}
}

void Inventory::deleteRecord()
{
	string phoneModel, line;										//PhoneModel stores the model user is searching (for deletion), line stores line read from file, temp is just a temporary data holder
	Inventory record;													//record holds values of record to be deleted
	int check = 0, confirmation = 0;									//check is used to check whether specified model is in file. confirmation is to confirm whether user wants to delete the record

	ifstream input(::inventoryFile, ios::in);
	if (!input)
	{
		cout << "\n\nError opening file.\n";
	}
	else
	{
		cout << "\n\nEnter the model you want to delete: ";
		cin >> phoneModel;

		getline(input, line);										//Ignoring the empty line in start of file
		while (!input.eof())
		{
			inputInventoryFromFile(input, record);

			if (record.model == phoneModel)									//setting check to 1 and breaking loop
			{
				check = 1;
				break;
			}
		}
		input.close();

		if (check == 0)															//operation cancelled if model not found
		{
			cout << "\n\nSpecified model does not exit in database.\n";
		}
		else
		{
			Inventory* data = &record;
			data->outputRecord();
			cout << "\n\nEnter 1 to delete this record or 0 to cancel.\n\n";		//taking user confirmation
			cin >> confirmation;

			if (confirmation == 1)
			{

				ifstream in(::inventoryFile, ios::in);
				ofstream out("NewInventoryFile.txt", ios::out);

				if (!in)
				{
					cout << "\n\nError opening file.\n";
				}
				else
				{
					getline(in, line);										//Ignoring the empty line in start of file
					while (!in.eof())
					{
						getline(in, line);
						if (line == record.model)							//if line matches data "model" in record, skip that line and the next 8 lines as well
						{
							for (int i = 0; i < 8; ++i)						//loop to skip record values in file
							{
								getline(in, line);
							}
						}
						else
						{
							out << endl << line;							//Storing line as-is
						}
					}
					in.close();
					out.close();

					remove(::inventoryFile);
					int result = rename("NewInventoryFile.txt", ::inventoryFile);		//Deleting old file and renaming new file to old one
					if (result == 0)
					{
						cout << "\n\nRecord deleted successfully.";
					}
					else
					{
						cout << "\n\nOperation failed.\n";
					}
				}
			}
			else
			{
				cout << "\n\nOperation cancelled.\n";
			}
		}
	}
}

void Inventory::lowStockPhones()
{
	string line;
	Inventory record;
	Inventory* recordPointer;
	int check = 0;													//used to check whether there are any low-stock phones or not

	ifstream in(::inventoryFile, ios::in);
	if (!in)
	{
		cout << "\nError opening file.\n";
	}
	else
	{
		getline(in, line);										//Ignoring the empty line in start of file
		while (!in.eof())
		{
			inputInventoryFromFile(in, record);

			if (record.inStock <= 10)
			{
				++check;													//Helps ensure following if condition is executed only once
				if (check == 1)
				{
					cout << "\n\nLow stock phone/s:\n\n";
				}

				recordPointer = &record;
				recordPointer->outputRecord();
				cout << endl;
			}
		}
		in.close();
		if (check == 0)
		{
			cout << "\nNo low-stock phones in inventory.\n";
		}
	}
}

void Inventory::showInventory()
{
	string line;
	Inventory record;
	Inventory* recordPointer;

	ifstream in(::inventoryFile, ios::in);
	if (!in)
	{
		cout << "\nError opening file.\n";
	}
	else
	{
		getline(in, line);										//Ignoring the empty line in start of file
		while (!in.eof())
		{
			inputInventoryFromFile(in, record);
			recordPointer = &record;
			recordPointer->outputRecord();
			cout << endl;
		}
		in.close();
		Sleep(15000);											//Enough time to see inventory

	}
}




//defining Search class function below

Search::Search(string a) : Inventory()
{
	userId = a + ".txt";
}

Search::Search(string a, string b, int c, string d, int e, float f, int g, string h, int i, string j) : Inventory(a, b, c, d, e, f, g, h, i)
{
	userId = j + ".txt";
}

void Search::outputRecord()							//function overriding (the reason behind overriding this function is that we don't want to output number of units in stock because that info is irrelevant to customer)
{
	cout << *this;
	if (inStock == 0)
	{
		cout << "\n\nSold out.\n\n";					//While we don't want to show inStock value to the customer, it still affects the customer and that's why Search class inherits from Inventory class
	}
}




//Defiding friend function declared in Search class

ofstream& outputSearchToFile(ofstream& out, const Search& filledRecord)
{
	out << endl << filledRecord.model
		<< endl << filledRecord.brand
		<< endl << filledRecord.price
		<< endl << filledRecord.os
		<< endl << filledRecord.ram
		<< endl << filledRecord.screenSize
		<< endl << filledRecord.cam
		<< endl << filledRecord.build << endl;		//not storing number of units in storage in the user's search results file because that info is not shown to the customer (because its irrelevant to customer)
	return out;
}




//Defining Search class functions

void Search::byPreference()
{
	Search record(userId);
	Inventory* recordPointer;
	int check = 0;
	int option = 0;
	string line;

	while (1)
	{

		cout << "\n\t\t\t\t\t      Enter 1 to search by model,\n\t\t\t\t\t\t  2 to search by brand,"
			<< "\n\t\t\t\t\t       3 to search by price range,\n\t\t\t\t\t\t    4 to search by OS,"
			<< "\n\t\t\t\t\t\t5 to search by RAM size,\n\t\t\t\t\t       6 to search by screen size,"
			<< "\n\t\t\t\t\t    7 to search by camera resultion,\n\t\t\t\t\t      8 to search by build material,"
			<< "\n\t\t\t\t\t      or 0 to return to main menu.\n\n\t\t\t\t\t\t\t    ";
		cin >> option;

		if (option == 1)
		{
			string phoneModel;

			cout << "Enter phone model: ";
			cin >> phoneModel;

			ifstream in(::inventoryFile, ios::in);
			if (!in)
			{
				cout << "\nError opening file.\n";
			}
			else
			{
				getline(in, line);										//Ignoring the empty line in start of file
				while (!in.eof())
				{
					inputInventoryFromFile(in, record);									//Although friend functions are not inherited, an object of Search class is still able to work with a friend function of its parent class because compiler implicitly typecasts it to Inventory

					if (record.model == phoneModel)
					{
						check = 1;
						recordPointer = &record;
						recordPointer->outputRecord();
						cout << endl;

						ofstream out(userId, ios::app);
						if (!out)
						{
							cout << "\nError opening file.\n";
						}
						else
						{
							outputSearchToFile(out, record);							//Storing users search results in file named by user's ID
						}
						out.close();
						break;										//model names are unique hence there will be only one phone to output and hence we can break the loop
					}
				}
				in.close();
				if (check == 0)
				{
					cout << "Specified model does not exist in database";
				}
			}
		}

		else if (option == 2)
		{
			string phoneBrand;

			cout << "Enter phone brand: ";
			cin >> phoneBrand;

			ifstream in(::inventoryFile, ios::in);
			if (!in)
			{
				cout << "\nError opening file.\n";
			}
			else
			{
				getline(in, line);										//Ignoring the empty line in start of file
				while (!in.eof())
				{
					inputInventoryFromFile(in, record);									//Although friend functions are not inherited, an object of Search class is still able to work with a friend function of its parent class because compiler implicitly typecasts it to Inventory


					if (record.brand == phoneBrand)
					{
						check = 1;
						recordPointer = &record;
						recordPointer->outputRecord();
						cout << endl;

						ofstream out(userId, ios::app);
						if (!out)
						{
							cout << "\nError opening file.\n";
						}
						else
						{
							outputSearchToFile(out, record);							//Storing users search results in file named by user's ID
						}
						out.close();
					}
				}
				in.close();

				if (check == 0)
				{
					cout << "\nPhones of specified brand do not exist in database.\n";
				}
			}
		}

		else if (option == 3)
		{
			int  lowerEnd, higherEnd;					//lowerEnd and higherEnd help define the price range. Instead of using just one variable to set the maximum limit I'm also using a variable to set the minimum limit. This is important because without both limits, user will get lots of irrelevant result. For exmple a user looking for a Rs. 60,000 phone will also be shown Rs. 5,000 phones

			cout << "\nEnter upper price limit: ";
			cin >> higherEnd;
			cout << "\nEnter lower price limit: ";
			cin >> lowerEnd;

			ifstream in(::inventoryFile, ios::in);
			if (!in)
			{
				cout << "\nError opening file.\n";
			}
			else
			{
				getline(in, line);										//Ignoring the empty line in start of file
				while (!in.eof())
				{
					inputInventoryFromFile(in, record);									//Although friend functions are not inherited, an object of Search class is still able to work with a friend function of its parent class because compiler implicitly typecasts it to Inventory

					if (record.price < higherEnd && record.price > lowerEnd)
					{
						check = 1;
						recordPointer = &record;
						recordPointer->outputRecord();
						cout << endl;

						ofstream out(userId, ios::app);
						if (!out)
						{
							cout << "\nError opening file.\n";
						}
						else
						{
							outputSearchToFile(out, record);							//Storing users search results in file named by user's ID
						}
						out.close();
					}
				}
				in.close();

				if (check == 0)
				{
					cout << "\nPhones in specified price range do not exist in database.\n";
				}
			}
		}

		else if (option == 4)
		{
			string phoneOS;

			cout << "Enter operating system: ";
			cin >> phoneOS;

			ifstream in(::inventoryFile, ios::in);
			if (!in)
			{
				cout << "\nError opening file.\n";
			}
			else
			{
				getline(in, line);										//Ignoring the empty line in start of file
				while (!in.eof())
				{
					inputInventoryFromFile(in, record);									//Although friend functions are not inherited, an object of Search class is still able to work with a friend function of its parent class because compiler implicitly typecasts it to Inventory

					if (record.os == phoneOS)
					{
						check = 1;
						recordPointer = &record;
						recordPointer->outputRecord();
						cout << endl;

						ofstream out(userId, ios::app);
						if (!out)
						{
							cout << "\nError opening file.\n";
						}
						else
						{
							outputSearchToFile(out, record);							//Storing users search results in file named by user's ID
						}
						out.close();
					}
				}
				in.close();

				if (check == 0)
				{
					cout << "\nPhones with specified OS do not exist in database.\n";
				}
			}

		}

		else if (option == 5)
		{
			int phoneRam;

			cin.ignore();
			cout << "Enter RAM (in GBs): ";							//Prices vary greatly according to RAM size hence we're using a limit so that results are relevant to the user. Also, the available RAM options are not that diverse so there's no need to use a range
			cin >> phoneRam;

			ifstream in(::inventoryFile, ios::in);
			if (!in)
			{
				cout << "\nError opening file.\n";
			}
			else
			{
				getline(in, line);										//Ignoring the empty line in start of file
				while (!in.eof())
				{
					inputInventoryFromFile(in, record);									//Although friend functions are not inherited, an object of Search class is still able to work with a friend function of its parent class because compiler implicitly typecasts it to Inventory

					if (record.ram <= phoneRam)
					{
						check = 1;
						recordPointer = &record;
						recordPointer->outputRecord();
						cout << endl;

						ofstream out(userId, ios::app);
						if (!out)
						{
							cout << "\nError opening file.\n";
						}
						else
						{
							outputSearchToFile(out, record);							//Storing users search results in file named by user's ID
						}
						out.close();
					}
				}
				in.close();

				if (check == 0)
				{
					cout << "\nPhones with specified RAM limit do not exist in database.\n";
				}
			}
		}

		else if (option == 6)
		{
			float phoneScreenSize;

			cin.ignore();
			cout << "Enter screen size (in Inches): ";
			cin >> phoneScreenSize;

			ifstream in(::inventoryFile, ios::in);
			if (!in)
			{
				cout << "\nError opening file.\n";
			}
			else
			{
				getline(in, line);										//Ignoring the empty line in start of file
				while (!in.eof())
				{
					inputInventoryFromFile(in, record);									//Although friend functions are not inherited, an object of Search class is still able to work with a friend function of its parent class because compiler implicitly typecasts it to Inventory

					if (record.screenSize <= phoneScreenSize)				//Screen sizes don't vary that much hence we're using only one limit
					{
						check = 1;
						recordPointer = &record;
						recordPointer->outputRecord();
						cout << endl;

						ofstream out(userId, ios::app);
						if (!out)
						{
							cout << "\nError opening file.\n";
						}
						else
						{
							outputSearchToFile(out, record);							//Storing users search results in file named by user's ID
						}
						out.close();
					}
				}
				in.close();

				if (check == 0)
				{
					cout << "\nPhones with specified screen size do not exist in database.\n";
				}
			}
		}

		else if (option == 7)
		{
			int phoneCam;

			cin.ignore();
			cout << "Enter camera resolution (in MPs): ";
			cin >> phoneCam;

			ifstream in(::inventoryFile, ios::in);
			if (!in)
			{
				cout << "\nError opening file.\n";
			}
			else
			{
				getline(in, line);										//Ignoring the empty line in start of file
				while (!in.eof())
				{
					inputInventoryFromFile(in, record);									//Although friend functions are not inherited, an object of Search class is still able to work with a friend function of its parent class because compiler implicitly typecasts it to Inventory

					if (record.cam <= phoneCam)								//Prices vary greatly according to camera resolution hence we're using a limit so that search results are relevant to the user
					{
						check = 1;
						recordPointer = &record;
						recordPointer->outputRecord();
						cout << endl;

						ofstream out(userId, ios::app);
						if (!out)
						{
							cout << "\nError opening file.\n";
						}
						else
						{
							outputSearchToFile(out, record);							//Storing users search results in file named by user's ID
						}
						out.close();
					}
				}
				in.close();

				if (check == 0)
				{
					cout << "\nPhones with specified camera resolution do not exist in database.\n";
				}
			}
		}

		else if (option == 8)
		{
			string phoneBuild;

			cin.ignore();
			cout << "Enter build material: ";
			getline(cin, phoneBuild);

			ifstream in(::inventoryFile, ios::in);
			if (!in)
			{
				cout << "\nError opening file.\n";
			}
			else
			{
				getline(in, line);										//Ignoring the empty line in start of file
				while (!in.eof())
				{
					inputInventoryFromFile(in, record);									//Although friend functions are not inherited, an object of Search class is still able to work with a friend function of its parent class because compiler implicitly typecasts it to Inventory

					if (record.build == phoneBuild)
					{
						check = 1;
						recordPointer = &record;
						recordPointer->outputRecord();
						cout << endl;

						ofstream out(userId, ios::app);
						if (!out)
						{
							cout << "\nError opening file.\n";
						}
						else
						{
							outputSearchToFile(out, record);							//Storing users search results in file named by user's ID
						}
						out.close();
					}
				}
				in.close();

				if (check == 0)
				{
					cout << "\nPhones with specified camera resolution do not exist in database.\n";
				}
			}
		}

		else if (option == 0)
		{
			break;
		}

		else
		{
			cout << "Please select a valid option";
		}
	}
}




//Defining friend functions declared in Sales class

ifstream& inputSalesFromFile(ifstream& in, Sales& emptyRecord)
{
	string temp;
	getline(in, emptyRecord.model);
	getline(in, emptyRecord.brand);
	getline(in, temp);
	emptyRecord.price = stoi(temp);
	getline(in, emptyRecord.os);
	getline(in, temp);
	emptyRecord.ram = stoi(temp);
	getline(in, temp);
	emptyRecord.screenSize = stof(temp);
	getline(in, temp);
	emptyRecord.cam = stoi(temp);
	getline(in, emptyRecord.build);											//inStock value is not stored in invoice file hence we're not reading that from the file
	getline(in, temp);
	emptyRecord.date = stoi(temp);
	getline(in, emptyRecord.customerName);
	getline(in, emptyRecord.contactNum);
	getline(in, emptyRecord.cardNum);
	return in;
}

ofstream& outputSalesToFile(ofstream& out, const Sales& filledRecord)			//functions can't modify const objects
{
	out << endl << filledRecord.model
		<< endl << filledRecord.brand
		<< endl << filledRecord.price
		<< endl << filledRecord.os
		<< endl << filledRecord.ram
		<< endl << filledRecord.screenSize
		<< endl << filledRecord.cam
		<< endl << filledRecord.build
		<< endl << filledRecord.date
		<< endl << filledRecord.customerName
		<< endl << filledRecord.contactNum
		<< endl << filledRecord.cardNum;
	return out;
}




//Defining Sales class functions below

Sales::Sales()
{
	time_t t = time(0);					//time_t is a datatype used to hold values returned by functions like time(). time(0) is a function that returns the current calendar time as a value of type time_t.
	tm* now = localtime(&t);			//tm is a structure made for holding time and date info. The time_t datatype is hard to read and isn't portable, the function localtime() helps convert time_t values into a tm object so that values can be easily manipulated.
	
	string year = to_string((now->tm_year + 1900));					//converting year value to a string
	string month, day, currentDate;

	if ((now->tm_mon + 1) >= 10)									//this if-else condition helps ensure we end up with a two letter string for month
	{
		month = to_string((now->tm_mon + 1));
	}
	else
	{
		month = "0" + to_string((now->tm_mon + 1));
	}

	if ((now->tm_mday) >= 10)									//this if-else condition helps ensure we end up with a two letter string for day
	{
		day = to_string((now->tm_mday));
	}
	else
	{
		day = "0" + to_string((now->tm_mday));
	}

	currentDate = year + month + day;									//storing year, month and day as one combined string. the string is stored in this particular order (YYYYMMDD) after being converted to an int, the year will be the most significant number and the day will be the last significant. This will help with the comparisons in report generation
	date = stoi(currentDate);											//converting currentDate string to integer so that comparison can be carried out
}

Sales::Sales(string a, string b, int c, string d, int e, float f, int g, string h, int i, string j, string k, string l) : Inventory(a,b,c,d,e,f,g,h,i)
{
	time_t t = time(0);
	tm* now = localtime(&t);

	string year = to_string((now->tm_year + 1900));					//converting year value to a string
	string month, day, currentDate;

	if ((now->tm_mon + 1) >= 10)									//this if-else condition helps ensure we end up with a two letter string for month
	{
		month = to_string((now->tm_mon + 1));
	}
	else
	{
		month = "0" + to_string((now->tm_mon + 1));
	}

	if ((now->tm_mday) >= 10)									//this if-else condition helps ensure we end up with a two letter string for day
	{
		day = to_string((now->tm_mday));
	}
	else
	{
		day = "0" + to_string((now->tm_mday));
	}

	currentDate = year + month + day;									//storing year, month and day as one combined string
	date = stoi(currentDate);											//converting currentDate string to integer so that comparison can be carried out
	
	customerName = j;
	contactNum = k;
	cardNum = l;
}

void Sales::getInput()
{
	cin.ignore();
	cout << "\n\nEnter full name: ";
	getline(cin, customerName);
	cin.ignore();
	cout << "Enter contact number: ";
	getline(cin, contactNum);
	cin.ignore();
	cout << "Enter credit/debit card number to confirm purchase: ";
	getline(cin, cardNum);
	cin.ignore();
}

void Sales::outputRecord()												//function overriding
{
	cout << "\nCustomer Invoice\n\n";
	cout << *this;
	cout << "\nPurchased by: " << customerName;
	cout << "\nContact number: " << contactNum;							//invoice doesn't carry customer's credit/debit card number
}


void Sales::buyPhone()
{
	int check = 0;												//checks whether specified model is in file or not
	string phoneModel, line;
	Inventory* record;											//pointer used to access output function

	cout << "\nEnter phone model: ";
	cin >> phoneModel;											//no spaces

	ifstream in(::inventoryFile, ios::in);
	if (!in)
	{
		cout << "\nError opening file";
	}
	else
	{
		getline(in, line);										//Ignoring the empty line in start of file
		while (!in.eof())
		{
			inputInventoryFromFile(in, *this);						//implicit typecasting to Inventory (upcasting)
			if (model == phoneModel)
			{
				check = 1;
				break;
			}
		}
		in.close();

		if (check == 0)
		{
			cout << "\nSpecified model doesn't exist in database.\b";
		}
		else
		{
			if (this->inStock == 0)
			{
				cout << "\n\nSorry, the phone has sold out.\n";
			}
			else
			{
				--(this->inStock);										//decrementing inStock number
				this->getInput();										//getting name, contact info and card number from user;
				record = this;
				record->outputRecord();									//accessing output function of Sales class

				ofstream out(::invoiceFile, ios::app);
				outputSalesToFile(out, *this);							//Storing invoice data
				out.close();

				ifstream input(::inventoryFile, ios::in);				//updating inStock number in inventory file
				ofstream output("NewInventoryFile.txt", ios::out);

				getline(input, line);										//Ignoring the empty line in start of file
				while (!input.eof())
				{
					getline(input, line);
					if (line == model)
					{
						output << endl << line;										//outputting model and the next 7 data values as-is
						for (int i = 0; i < 7; ++i)
						{
							getline(input, line);
							output << endl << line;
						}
						getline(input, line);										//ignoring old inStock value
						output << endl << inStock;									//storing updated value
					}
					else
					{
						output << endl << line;
					}
				}

				input.close();
				output.close();

				remove(::inventoryFile);
				int temp = rename("NewInventoryFile.txt", ::inventoryFile);
				if (temp == 0)
				{
					cout << "\n\nPurchase completed successfully.";
				}
				else
				{
					cout << "\n\nOperation failed.";
				}
			}
		}
	}
}

void Sales::findInvoices()
{
	Inventory* record;
	Sales emptyRecord;
	string customerContactNumber, line;
	int check = 0;
	cout << "\nEnter customer's contact number: ";
	cin >> customerContactNumber;								//no spaces
	cin.ignore();

	ifstream in(::invoiceFile, ios::in);

	if (!in)
	{
		cout << "\nError opening file.\n";
	}
	else
	{
		getline(in, line);										//Ignoring the empty line in start of file
		while (!in.eof())
		{
			inputSalesFromFile(in, emptyRecord);
			if (emptyRecord.contactNum == customerContactNumber)
			{
				check = 1;
				record = &emptyRecord;
				record->outputRecord();
				cout << endl;
			}
		}
		in.close();

		if (check == 0)
		{
			cout << "\nNo records with specified contact number found.\n";
		}
		else 
		{
			Sleep(7000);											//Enough time to look at invoice
		}
	}
}

void Sales::generateReport()
{
	string temp;
	string startYear, endYear;						//used for taking date inputs
	string startMonth, endMonth;
	string startDay, endDay;
	int startingDate, endingDate;
	int totalRevenue = 0;							//stores amount of total sales made in specified period
	Sales emptyRecord;								//used for reading data from invoice file, and for writing data to report file

	cout << "\nEnter starting date of report (in DD/MM/YYYY format): ";
	cin >> setw(2) >> startDay;
	cin.ignore();						//ignore '/'
	cin >> setw(2) >> startMonth;
	cin.ignore();						//ignore '/'
	cin >> setw(4) >> startYear;
	cin.ignore();						//ignore '/'

	temp = startYear + startMonth + startDay;
	startingDate = stoi(temp);


	cout << "\nEnter ending date of report (in DD/MM/YYYY format): ";
	cin >> setw(2) >> endDay;
	cin.ignore();						//ignore '/'
	cin >> setw(2) >> endMonth;
	cin.ignore();						//ignore '/'
	cin >> setw(4) >> endYear;
	cin.ignore();						//ignore '/'

	temp = endYear + endMonth + endDay;
	endingDate = stoi(temp);

	ifstream in(::invoiceFile, ios::in);
	ofstream out(::reportFile, ios::out);				//truncating file in case there was any other data in it

	if (!in)
	{
		cout << "\nError opening file.\n";
	}
	else
	{
		out << "\nSales Report\n\nPeriod: " << startDay << "/" << startMonth << "/" << startYear << " to " << endDay << "/" << endMonth << "/" << endYear << "\n\n";
		getline(in, temp);														//ignoring the empty first line
		while (!in.eof())
		{
			inputSalesFromFile(in, emptyRecord);
			if (emptyRecord.date >= startingDate && emptyRecord.date <= endingDate)
			{
				outputSalesToFile(out, emptyRecord);
				out << endl;
				totalRevenue += emptyRecord.price;
			}
		}
		out << endl << "\nTotal revenue generated: " << totalRevenue << endl;

		in.close();
		out.close();

		cout << "\nReport generated.\n";
	}
}




void programInterface()
{
	int option = 0, optionTwo = 0, optionThree = 0;
	string userIdentity;
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t Welcome to Phone Finder Application\n\n\t\t\t\t\t\t\t   ";
	Sleep(2000);
	system("CLS");

	while (1)
	{
		cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t   Enter 1 to access inventory management features,"
			<< "\n\t\t\t\t      2 to search phones on basis of preferences,"
			<< "\n\t\t\t\t\t\t  3 to buy a phone,"
			<< "\n\t\t\t\t\t4 to access sales management features,"
			<< "\n\t\t\t\t\t\tor 0 to exit the program.\n\n\t\t\t\t\t\t\t  ";
		cin >> option;

		if (option == 1)
		{
			system("CLS");
			cout << "\n\n\n\n\n\n\n\n\n\t\t\t\t\t     Enter 1 to add a new phone,"
				<< "\n\t\t\t\t\t\t  2 to edit a phone,\n\t\t\t\t\t\t 3 to delete a phone,"
				<< "\n\t\t\t\t\t     4 to view low-stock phones,\n\t\t\t\t\t\t5 to see all inventory,"
				<< "\n\t\t\t\t\t    or 0 to go back to main menu\n\n\t\t\t\t\t\t\t  ";
			cin >> optionTwo;
			cout << endl << endl;
			if (optionTwo == 1)
			{
				Inventory::addRecord();
			}
			else if (optionTwo == 2)
			{
				Inventory::editRecord();
			}
			else if (optionTwo == 3)
			{
				Inventory::deleteRecord();
			}
			else if (optionTwo == 4)
			{
				Inventory::lowStockPhones();
			}
			else if (optionTwo == 5)
			{
				Inventory::showInventory();
			}
			else
			{
				continue;
			}
		}
		else if (option == 2)
		{
			system("CLS");
			cin.ignore();
			cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tEnter userID: ";
			getline(cin, userIdentity);
			Search s(userIdentity);
			system("CLS");
			cout << "\n\n";
			s.byPreference();
		}
		else if (option == 3)
		{
			system("CLS");
			cout << "\n\n";
			Sales customer;
			customer.buyPhone();
		}
		else if (option == 4)
		{
			system("CLS");
			cout << "\n\n";
			Sales salesman;
			cout << "\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\tEnter 1 to find invoice/s of a customer,"
				<< "\n\t\t\t\t\t     or 2 to generate sales report.\n\n\t\t\t\t\t\t\t  ";
			cin >> optionThree;

			if (optionThree == 1)
			{
				system("CLS");
				cout << "\n\n";
				salesman.findInvoices();
			}
			else if (optionThree == 2)
			{
				system("CLS");
				cout << "\n\n";
				salesman.generateReport();
			}
			else			
			{
				cout << "\nInvalid value. Going back to main menu.";
				Sleep(3000);
				system("CLS");
				continue;
			}
		}
		else if (option == 0)
		{
			system("CLS");
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tgoodbye!\n\n\n\n\n\n\n\n\n\n\n\n\n";
			exit(0);
		}
		else
		{
			cout << "\n\nInvalid value.";
		}
		Sleep(3000);
		system("CLS");
	}
}
