#include <iostream>
#include <string>
#include <ctime>

#include "Elevator.h"

using namespace std;

int main()
{
	srand((unsigned int)time(NULL));
	Elevator elevator;
	elevator.runUp();
	return 0;
}