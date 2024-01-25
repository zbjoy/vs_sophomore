#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <ctime>
#include "People.h"
#include "Dog.h"

using namespace std;

class Elevator
{
public:
	Elevator()
	{
		this->layer = 1;
	}

public:
	static const double weightMax;
	int layer;
	static const int layerMax;
	void runUp();
	void createSomePeople();
	void popSomePeopleUp();
	void pushSomePeopleUp();
	void printElevatorStatus();
private:
	vector<People> nowElevatorPeople;
	vector<Dog> nowElevatorDog;
	deque<People> wantPushPeople;
	deque<Dog> wantPushDog;
	list<People> pushPeople;
	list<Dog> pushDog;
	list<People> popPeople;
	list<Dog> popDog;
};