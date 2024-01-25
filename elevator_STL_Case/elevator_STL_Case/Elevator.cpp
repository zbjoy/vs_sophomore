#include "Elevator.h"

const double Elevator::weightMax = 1500;
const int Elevator::layerMax = 28;

void Elevator::runUp()
{
	for (; layer <= layerMax; ++layer)
	{
		createSomePeople();
		if (layer == layerMax)
		{
			popSomePeopleUp();
		}
		else
		{
			popSomePeopleUp();
			pushSomePeopleUp();
		}
		printElevatorStatus();
		cout << endl << endl;
	}
}

void Elevator::popSomePeopleUp()
{
	if (layer == layerMax)
	{
		while (!nowElevatorPeople.empty())
		{
			nowElevatorPeople.pop_back();
		}

		while (!nowElevatorDog.empty())
		{
			nowElevatorDog.pop_back();
		}
		return;
	}

	int popPeopleNums = rand() % 6;

	while (popPeopleNums--)
	{
		if (nowElevatorPeople.empty())
		{
			break;
		}
		int popPeoplePos = rand() % nowElevatorPeople.size();
		vector<People>::iterator it = nowElevatorPeople.begin();
		while (popPeoplePos--)
		{
			it++;
		}
		popPeople.push_back(*it);
		nowElevatorPeople.erase(it);
	}

	int popDogNums = rand() % 2;

	while (popDogNums--)
	{
		if (nowElevatorDog.empty())
		{
			break;
		}
		int popDogPos = rand() % nowElevatorDog.size();
		vector<Dog>::iterator it = nowElevatorDog.begin();
		while (popDogPos--)
		{
			it++;
		}
		popDog.push_back(*it);
		nowElevatorDog.erase(it);
	}
}

void Elevator::pushSomePeopleUp()
{
	double nowWeight = 0.0;

	for (vector<People>::iterator it = nowElevatorPeople.begin(); it != nowElevatorPeople.end(); it++)
	{
		nowWeight += it->showWeight();
	}
	for (vector<Dog>::iterator it = nowElevatorDog.begin(); it != nowElevatorDog.end(); it++)
	{
		nowWeight += it->showWeight();
	}
	while (!wantPushPeople.empty())
	{
		if (nowWeight < weightMax)
		{
			if (weightMax < nowWeight + wantPushPeople.front().showWeight())
			{
				break;
			}
			nowElevatorPeople.push_back(wantPushPeople.front());
			pushPeople.push_back(wantPushPeople.front());
			wantPushPeople.pop_front();
		}
	}

	while (!wantPushDog.empty())
	{
		if (nowWeight < weightMax)
		{
			if (weightMax < nowWeight + wantPushDog.front().showWeight())
			{
				break;
			}
			nowElevatorDog.push_back(wantPushDog.front());
			pushDog.push_back(wantPushDog.front());
			wantPushDog.pop_front();
		}
	}
}

void Elevator::printElevatorStatus()
{
	int i = 0;
	cout << endl << "------------------------------------------------------" << endl;
	double nowWeight = 0.0;

	for (vector<People>::iterator it = nowElevatorPeople.begin(); it != nowElevatorPeople.end(); it++)
	{
		nowWeight += it->showWeight();
	}
	for (vector<Dog>::iterator it = nowElevatorDog.begin(); it != nowElevatorDog.end(); it++)
	{
		nowWeight += it->showWeight();
	}
	cout << "第" << layer << "层(" << nowWeight << "公斤)" << endl;
	cout << "人的数量:" << nowElevatorPeople.size() << endl;
	for (vector<People>::iterator it = nowElevatorPeople.begin(); it != nowElevatorPeople.end(); ++it)
	{
		if (!(i % 3) && i != 0)
		{
			cout << endl;
		}
		cout << "姓名: " << it->showName() << " 体重: " << it->showWeight() << "     ";
		i++;
	}
	
	cout << endl;
	cout << "进入的人员:" << endl;
	for (list<People>::iterator it = pushPeople.begin(); it != pushPeople.end(); ++it)
	{
		if (!(i % 3) && i != 0)
		{
			cout << endl;
		}
		cout << "姓名: " << it->showName() << " 体重: " << it->showWeight() << "     ";
		i++;
	}
	cout << endl;
	cout << "出去的人员:" << endl;
	for (list<People>::iterator it = popPeople.begin(); it != popPeople.end(); ++it)
	{
		if (!(i % 3) && i != 0)
		{
			cout << endl;
		}
		cout << "姓名: " << it->showName() << " 体重: " << it->showWeight() << "     ";
		i++;
	}
	//cout << endl << "------------------------------------------------------" << endl;
	cout << endl;
	cout << "狗的数量:" << nowElevatorDog.size() << endl;
	i = 0;
	for (vector<Dog>::iterator it = nowElevatorDog.begin(); it != nowElevatorDog.end(); ++it)
	{
		if (!(i % 3) && i != 0)
		{
			cout << endl;
		}
		cout << "姓名: " << it->showName() << " 体重: " << it->showWeight() << "     ";
		i++;
	}
	cout << endl;
	cout << "进入的狗员:" << endl;
	for (list<Dog>::iterator it = pushDog.begin(); it != pushDog.end(); ++it)
	{
		if (!(i % 3) && i != 0)
		{
			cout << endl;
		}
		cout << "姓名: " << it->showName() << " 体重: " << it->showWeight() << "     ";
		i++;
	}

	cout << endl;
	cout << "出去的狗员:" << endl;
	for (list<Dog>::iterator it = popDog.begin(); it != popDog.end(); ++it)
	{
		if (!(i % 3) && i != 0)
		{
			cout << endl;
		}
		cout << "姓名: " << it->showName() << " 体重: " << it->showWeight() << "     ";
		i++;
	}
	cout << endl << "------------------------------------------------------" << endl;
}

void Elevator::createSomePeople()
{
	while (!wantPushPeople.empty())
	{
		wantPushPeople.pop_back();
	}

	while (!wantPushDog.empty())
	{
		wantPushDog.pop_back();
	}

	int peopleNum = rand() % 6;
	int dogNum = 0;
	if (peopleNum != 0)
	{
		dogNum = rand() % 2;
	}
	
	string setName = "ABCDE";
	string front = "第";
	string front2 = "层";
	char temp[10] = { 0 };
	sprintf(temp, "%d", layer);
	for (int i = 0; i < peopleNum; i++)
	{
		string name = front + string(temp) + front2;
		name += setName[i];
		double weight = rand() % 80 + 20;
		wantPushPeople.push_back(People(name, weight));
	}


	string setDogName = "A";
	for (int i = 0; i < dogNum; i++)
	{
		string name = front + string(temp) + front2 + "狗" + setDogName[i];
		double weight = rand() % 45 + 5;
		wantPushDog.push_back(Dog(name, weight));
	}

}