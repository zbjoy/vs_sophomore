#include "Cube.h"

void Cube::setLength(int tLength = 0)
{
	length = tLength;
}

void Cube::setWeight(int tWeight = 0)
{
	weight = tWeight;
}

void Cube::setHeight(int tHeight = 0)
{
	height = tHeight;
}

int Cube::getLength()
{
	return length;
}

int Cube::getWeight()
{
	return weight;
}

int Cube::getHight()
{
	return height;
}

bool isSame(Cube temp1, Cube temp2)
{

	if (temp1.getHight() == temp2.getHight() && temp1.getLength() == temp2.getLength() && temp1.getWeight() == temp2.getWeight())
	{
		return true;
	}

}
