#include <iostream>

using namespace std;

class Maker
{
private:
	Maker() {};
	Maker(const Maker&) {};
private:
	static Maker* p;
public:
	static Maker* getMaker()
	{
		return p;
	}

	void func()
	{
		cout << "µ¥ÀıÄ£Ê½" << endl;
	}
};

Maker* Maker::p = new Maker;

int main()
{
	Maker* pMaker = Maker::getMaker();
	pMaker->func();
	return 0;
}