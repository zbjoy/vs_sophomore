#include "SonCar.h"

void SonCar::print()
{
	Car::print();
	cout << number;
}

void SonCar::setInformation(string tName, int tAge, string tType, string tNumber)
{
	Car::setInformation(tName, tAge, tType);
	number = tNumber;
}
