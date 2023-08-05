#pragma once
#include <iostream>
#include "phone.h"

using namespace std;

class Inventory : public Phone
{
protected:
	int inStock;								//number of articles in stock of each phone

public:
	Inventory();
	Inventory(string a, string b, int c, string d, int e, float f, int g, string h, int i);
	void inputRecord();
	virtual void outputRecord();
	static void addRecord();
	static void editRecord();
	static void deleteRecord();
	static void lowStockPhones();				//This is a special feature made just for the vendor so that they can quickly find the models with low stock
	static void showInventory();				//vendor can use this feature to quickly see the entire inventory

	friend ifstream& inputInventoryFromFile(ifstream& in, Inventory& emptyRecord);
	friend ofstream& outputInventoryToFile(ofstream& out, const Inventory& filledRecord);			//functions can't modify const objects

};
