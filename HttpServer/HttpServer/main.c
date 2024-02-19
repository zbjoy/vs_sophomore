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
		memcpy(path, argv[2], sizeof(argv[2]));
	}
	else
	{
		memset(path, "./res/", 3);
	}

	chdir(path);
	epoll_run(atoi(argv[1]));
	return 0;
}