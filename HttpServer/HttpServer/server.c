#include "server.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

int initListenFd(unsigned short port)
{
	int lfd = socket(AF_INET, SOCK_STREAM, 0);

	//端口复用
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	int ret = bind(lfd, (struct sockaddr*)&addr, sizeof(addr));
	if (ret == -1)
	{
		perror("bind");
		return -1;
	}

	ret = listen(lfd, 128);
	if (ret == -1)
	{
		perror("bind");
		return -1;
	}

	return lfd;
}

int epoll_run(unsigned short port)
{
	int lfd = initListenFd(port);

	//创建epoll模型 
	int efd = epoll_create(1);
	if (efd == -1)
	{
		perror("epoll_create");
		return -1;
	}

	//初始化epoll
	struct epoll_event ev, evs[1024];
	ev.data.fd = lfd;
	ev.events = EPOLLIN;
	int ret = epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &ev);
	if (ret == -1)
	{
		perror("epoll_ctl");
		return -1;
	}

	int maxEvents = sizeof(evs) / sizeof(evs[0]);
	//检测
	while (1)
	{
		int nums = epoll_wait(efd, evs, maxEvents, -1);
		for (int i = 0; i < nums; ++i)
		{
			int curfd = evs[i].data.fd;
			int curevents = evs[i].events;
			if (curfd == lfd && curevents == EPOLLIN)
			{
				//如果提取失败重新提取,有三次提取机会,都失败直接忽略这个连接
				int errorNums = 3;
				do
				{
					static int num = 0;
					static int flag = 0;
					if (flag == 1 && num == errorNums)
					{
						num = 0;
						flag = 0;
						break;	
					}
					//提取连接
					ret = acceptConn(curfd, efd);
					if (ret == -1)
					{
						printf("acceptConn error.......\n");
						//处理错误
						num++;
						flag = 1;
						continue;
					}

					if (flag == 1)
					{
						num = 0;
						flag = 0;
					}
				} while (0);
			}
			else
			{
				//接受数据
				ret = recvHttpRequest(curfd);
				if (ret == 0)
				{
					//断开连接
				}
				else if(ret == -1)
				{
					//发生错误
				}
			}
		}
	}

	return 0;
}

int acceptConn(int lfd, int efd)
{
	int cfd = accept(lfd, NULL, NULL);
	if (cfd == -1)
	{
		perror("accept");
		return -1;
	}

	//设置文件描述符为非阻塞
	int flag = fcntl(cfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(cfd, F_SETFL, flag);

	//上树
	struct epoll_event ev;
	ev.data.fd = cfd;
	ev.events = EPOLLIN | EPOLLET;
	int ret = epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
	if (ret == -1)
	{
		perror("epoll_ctl");
		return -1;
	}

	return cfd;
}

int recvHttpRequest(int cfd)
{
	int len, total = 0;
	char temp[1024] = "";
	char buf[4096] = "";
	while ((len = recv(cfd, temp, sizeof(temp), 0)) > 0)
	{
		if (total + len < sizeof(buf))
		{
			memcpy(buf + total, temp, sizeof(len));
			total += len;
		}
		else
		{
			break;
		}
	}

	if (len == -1 && errno == EAGAIN)
	{
		char* ptr = strstr(buf, "\r\n");
		int len = ptr - buf;
		buf[len] = '\0';
		//成功读取出数据
		parseRequestLine(buf, cfd);
		return total;
	}
	else if (len == 0)
	{
		return 0;
	}
	else
	{
		//出错
		return -1;
	}
}

int parseRequestLine(char* reqLine, int cfd)
{
	//解包
	char method[5] = "";
	char path[1024] = "";
	sscanf(reqLine, " %[^ ] %[^ ]", method, path);

	if (strcmp(method, "GET"))
	{
		return -1;
	}

	char* file = NULL;
	if (!strcmp(path, "/"))
	{
		file = "./";
	}
	else
	{
		file = path + 1;
	}

	struct stat st;
	int ret = stat(file, &st);
	if (ret == -1)
	{
		//发送404
	}

	if (S_ISREG(st.st_mode))
	{
		//普通文件
		sendHeadMsg(cfd);
		sendFile(cfd, file);
	}
	else if (S_ISDIR(st.st_mode))
	{
		//目录
	}
	return 0;
}

int sendHeadMsg(int cfd)
{
	char buf[4096] = "";
	memcpy(buf, "http/1.1 200 ok\r\n", 18);

	send(cfd, buf, strlen(buf), 0);
	return 0;
}

int sendFile(int cfd, char* file)
{
	return 0;
}
