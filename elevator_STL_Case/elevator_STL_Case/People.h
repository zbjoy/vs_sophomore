#pragma once
#include <iostream>
#include <string>

using namespace std;

class People
{
public:
	People() {}
	People(string tName, double weight)
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

