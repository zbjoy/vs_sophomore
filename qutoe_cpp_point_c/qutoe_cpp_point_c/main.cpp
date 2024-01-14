#include <iostream>

using namespace std;

void printArr(int* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%d  ", arr[i]);
	}
}

//数组的引用
void test()
{
	int arr[] = { 1, 2, 3, 4, 5 };
	typedef int(My_Arr)[5];
	My_Arr& myArr = arr;
	printArr(myArr, sizeof(myArr) / sizeof(myArr[0]));
}

int main()
{
	test();
	return 0;
}