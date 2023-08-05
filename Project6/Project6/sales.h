#pragma once
#include <iostream>
#include "inventory.h"

using namespace std;

class Sales : public Inventory
{
private:
	int date;
	string customerName;
	string contactNum;
	string cardNum;

public:
	Sales();
	Sales(string a, string b, int c, string d, int e, float f, int g, string h, int i, string j, string k, string l);
	void getInput();
	virtual void outputRecord();											//function overriding
	void buyPhone();
	void findInvoices();													//special function to help vendor find invoices of a particular customer
	void generateReport();

	friend ifstream& inputSalesFromFile(ifstream& in, Sales& emptyRecord);
	friend ofstream& outputSalesToFile(ofstream& out, const Sales& filledRecord);			//functions can't modify const objects

								//////////////////////////////////////mode 1 for customer mode 2 for management
};