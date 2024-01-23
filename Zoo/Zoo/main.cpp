#include <iostream>

using namespace std;

class Animal
{
public:
	virtual void speaker() = 0;

};

class Dark: public Animal
{
public:
	Dark(string tName, string tType):name(tName),type(tType)
	{

	}
	void speaker()
	{
		cout << type << "ÅÆÑ¼×Ó:" << "¸Â¸Â¸Â" << endl;
	}
private:
	string name;
	string type;
};

class Tiger: public Animal
{
public:
	Tiger(string tName, int tAge) :name(tName), age(tAge)
	{

	}
	void speaker()
	{
		cout << age << "Ëê ÀÏ»¢: " << name << "à»à»à»" << endl;
	}
private:
	string name;
	int age;
};

class Dog: public Animal
{
public:
	Dog(string tName) :name(tName)
	{

	}
	void speaker()
	{
		cout << "¹·×Ó " << name << ": ÍôÍôÍô" << endl;
	}
private:
	string name;
};

class Zoo
{
public:
	Zoo()
	{
		size = 0;
		capacity = 50;
		zoo = new Animal * [capacity];
	}
	void startSpeak()
	{
		for (int i = 0; i < size; i++)
		{
			zoo[i]->speaker();
		}
	}

	void addAnimal(Animal* an)
	{
		if (size == capacity)
		{
			return;
		}

		zoo[size++] = an;
	}

	void destory()
	{
		for (int i = 0; i < size; i++)
		{
			delete zoo[i];
			zoo[i] = NULL;
		}

		size = 0;
	}

	~Zoo()
	{
		for (int i = 0; i < size; i++)
		{
			delete zoo[i];
			zoo[i] = NULL;
		}
		delete[] zoo;
		zoo = NULL;
		size = 0;
		capacity = 0;
	}
private:

	Animal** zoo;
	int size;
	int capacity;
};

void test()
{
	Zoo* zoo = new Zoo;
	Dark* dark = new Dark("ÀÏºÚ", "ÖÜºÚÑ¼");
	Tiger* tiger = new Tiger("ÀÏÁõ", 9);
	Dog* dog = new Dog("Ð¡°×");

	zoo->addAnimal(dark);
	zoo->addAnimal(tiger);
	zoo->addAnimal(dog);

	zoo->startSpeak();
	zoo->destory();
}

int main()
{
	test();
	return 0;
}