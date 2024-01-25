#pragma once
#include <iostream>
#include <string>

using namespace std;

class Dog
{
public:
	Dog() {};
	Dog(string tName, double weight)
	{
		this->name = tName;
		this->weight = weight;
	}

	double showWeight()
	{
		return weight;
	}

	string showName()
	{
		return name;
	}
private:
	string name;
	double weight;
};

