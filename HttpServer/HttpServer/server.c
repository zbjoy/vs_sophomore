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

	//�˿ڸ���
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

	//����epollģ�� 
	int efd = epoll_create(1);
	if (efd == -1)
	{
		perror("epoll_create");
		return -1;
	}

	//��ʼ��epoll
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
	//���
	while (1)
	{
		int nums = epoll_wait(efd, evs, maxEvents, -1);
		for (int i = 0; i < nums; ++i)
		{
			int curfd = evs[i].data.fd;
			int curevents = evs[i].events;
			if (curfd == lfd && curevents == EPOLLIN)
			{
				//�����ȡʧ��������ȡ,��������ȡ����,��ʧ��ֱ�Ӻ����������
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
					//��ȡ����
					ret = acceptConn(curfd, efd);
					if (ret == -1)
					{
						printf("acceptConn error.......\n");
						//�������
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
				//��������
				ret = recvHttpRequest(curfd);
				if (ret == 0)
				{
					//�Ͽ�����
				}
				else if(ret == -1)
				{
					//��������
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

	//�����ļ�������Ϊ������
	int flag = fcntl(cfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(cfd, F_SETFL, flag);

	//����
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
		//�ɹ���ȡ������
		parseRequestLine(buf, cfd);
		return total;
	}
	else if (len == 0)
	{
		return 0;
	}
	else
	{
		//����
		return -1;
	}
}

int parseRequestLine(char* reqLine, int cfd)
{
	//���
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
		//����404
	}

	if (S_ISREG(st.st_mode))
	{
		//��ͨ�ļ�
		sendHeadMsg(cfd);
		sendFile(cfd, file);
	}
	else if (S_ISDIR(st.st_mode))
	{
		//Ŀ¼
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
