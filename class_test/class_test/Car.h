#pragma once
#include <iostream>
#include <string>

using namespace std;

class Car
{

public:
	void print();
	void setInformation(string tName, int age, string tType);
private:
	string name;
	int age;
	string type;
};

