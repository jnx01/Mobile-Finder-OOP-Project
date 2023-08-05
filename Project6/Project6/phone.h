#pragma once
#include <iostream>
#include <string>

using namespace std;


class Phone
{
protected:
	string model;
	string brand;
	int price;
	string os;
	int ram;						//mentioning RAM in GBs is the norm these days. Also, when taking input, user is explicitly told to enter the value in GBs
	float screenSize;				//screenSizes usually are measured in inches and to the precision of 0.1 hence, I've made this variable a float
	int cam;						//mentioning camera resolution in Megapixels is the norm these days. Also, when taking input, user is explicitly told to enter the value in MPs
	string build;

public:
	Phone();
	Phone(string a, string b, int c, string d, int e, float f, int g, string h);

	friend istream& operator >> (istream& userInput, Phone& emptyRecord);			//operator overloaded functions for taking input and displaying object values
	friend ostream& operator << (ostream& output, const Phone& filledRecord);		//const objects can't be editted

};
