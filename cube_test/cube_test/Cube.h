#pragma once
class Cube
{
public:

	void setLength(int tLength);
	void setWeight(int tWeight);
	void setHeight(int tHeight);
	int getLength();
	int getWeight();
	int getHight();

	int calculateS()
	{
		return (length * weight + length * height + weight * height) * 2;
	}

	int calculateV()
	{
		return length * weight * height;
	}

	bool isSame(Cube temp)
	{
		if (temp.getHight() == getHight() && temp.getLength() == getLength() && temp.getWeight() == getWeight())
		{
			return true;
		}
		return false;
	}

private:
	int length;
	int weight;
	int height;
};

bool isSame(Cube temp1, Cube temp2);

