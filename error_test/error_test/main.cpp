#include <iostream>

using namespace std;

class MyOut_Of
{
public:
	MyOut_Of()
	{
		error = "";
	}

	MyOut_Of(string tError) : error(tError) {};
	//MyOut_Of(char* tError) : error(string(tError)) {};
	MyOut_Of(char* tError) : error(tError) {};

	virtual char const* what()
	{
		return this->error.c_str();
	}


private:
	string error;
};

void func()
{
	/*throw out_of_range("Òç³ö");*/
	throw MyOut_Of("Òç³ö");
}

void test()
{
	try
	{
		func();
	}
	catch (int&)
	{

	}
	catch (out_of_range& err)
	{
		cout << err.what() << endl;
	}
	catch (MyOut_Of& err)
	{
		cout << err.what() << endl;
	}
	catch (...)
	{

	}
}

int main()
{
	test();
	return 0;
}