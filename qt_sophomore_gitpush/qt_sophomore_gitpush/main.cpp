#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>


void git_Push(char* cmd)
{
	system("git add ./");
	char cmdStr[1024] = { 0 };
	sprintf(cmdStr, "%s\"%s\"", "git commit -m", cmd);
	system(cmdStr);
	system("git push");
}

int main()
{
	char temp[1024] = { 0 };
	std::cin >> temp;
	git_Push(temp);
	return 0;
}