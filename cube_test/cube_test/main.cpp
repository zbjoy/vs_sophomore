#include <iostream>
#include "Cube.h"

using namespace std;

int main()
{
	Cube c;
	c.setHeight(10);
	c.setLength(10);
	c.setWeight(10);
	cout << c.calculateS() << endl;
	cout << c.calculateV() << endl;

	Cube c1;
	c1.setHeight(10);
	c1.setLength(10);
	c1.setWeight(10);

	cout << isSame(c, c1) << endl;
	cout << c.isSame(c1) << endl;
	return 0;
}