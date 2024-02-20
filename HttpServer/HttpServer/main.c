#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("usage: ./a.out port [resPath]\n");
		return -1;
	}

	char path[1024] = "";
	if (argc == 3)
	{
		memcpy(path, argv[2], strlen(argv[2]) + 1);
	}
	else
	{
		//memset(path, "./res/", 7);
		memcpy(path, "./res/", 7);
	}

	chdir(path);
	epoll_run(atoi(argv[1]));
	return 0;
}