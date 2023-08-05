#pragma once
#include <iostream>
#include <string>
#include "inventory.h"

using namespace std;

class Search : public Inventory
{
private:
	string userId;

public:
	Search(string a);															//Can't make an object of search without giving UserID
	Search(string a, string b, int c, string d, int e, float f, int g, string h, int i, string j);
	virtual void outputRecord();												//function overriding
	void byPreference();														//Making just one object of Phone class and taking user input in that to search for phones would have actually been a lot easier for me to implement but it won't be very useful for the user in a lot of cases (for example if the user wants to see all phones within a specific price range)


	friend ofstream& outputSearchToFile(ofstream& out, const Search& filledRecord);			//functions can't modify const objects

};
