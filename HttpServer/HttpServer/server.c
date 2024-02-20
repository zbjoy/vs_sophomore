#include "server.h"
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <strings.h>
#include <dirent.h>
#include <stdlib.h>

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

					printf("1.��ȡ���ӳɹ�.......\n");
				} while (0);
			}
			else
			{
				//��������
				ret = recvHttpRequest(curfd);
				if (ret == 0)
				{
					//�Ͽ�����
					printf("һ���ͻ��˶Ͽ�����.......\n");
					disConnect(efd, curfd);
				}
				else if(ret == -1)
				{
					//��������
					printf("��������.......\n");
					disConnect(efd, curfd);
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
		printf("temp: %s\n", temp);
		printf("len: %d\nsizeof(buf): %d\n", len, sizeof(buf));
		if (total + len < sizeof(buf))
		{
			//memcpy(buf + total, temp, sizeof(len));
			memcpy(buf + total, temp, len);
			total += len;
		}
		else
		{
			break;
		}
	}
	//printf("2.�������ݳɹ�.......\n");
	//printf("len=%d.......\nerrno=%d\nEAGAIN=%d\n", len, errno, EAGAIN);

	if (len == -1 && errno == EAGAIN)
	{
		char* ptr = strstr(buf, "\r\n");
		//printf("ptr is %s\nbuf is %s\n", ptr, buf);
		//int len = ptr - buf;
		len = ptr - buf;
		//printf("7.test\n");
		//printf("len is %d\n", len);
		buf[len] = '\0';

		//�ɹ���ȡ������
		//printf("3.��ʼ����.......\n");
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
	sscanf(reqLine, "%[^ ] %[^ ]", method, path);

	if (strcasecmp(method, "GET"))
	{
		return -1;
	}
	//printf("is get request.......\n");
	char* file = NULL;
	if (!strcmp(path, "/"))
	{
		file = "./test.html";
	}
	else
	{
		file = path + 1;
	}
	//printf("file is %s\n", file);
	struct stat st;
	int ret = stat(file, &st);
	//printf("ret is %d\n", ret);
	if (ret == -1)
	{
		printf("cfd Ϊ %d �Ŀͻ��˷��ʵ����󲻴���.......\n", cfd);
		//����404
		//sendHeadMsg(cfd, 404, "Not Found", getFileType(".html"), st.st_size);
		sendHeadMsg(cfd, 404, "Not Found", getFileType(".html"), -1);
		sendFile(cfd, "./404.html");
	}

	if (S_ISDIR(st.st_mode))
	{
		//Ŀ¼
		sendHeadMsg(cfd, 200, "OK", getFileType(".html"), -1);
		sendDir(cfd, file);
	}
	else if (S_ISREG(st.st_mode))
	{
		//��ͨ�ļ�
		sendHeadMsg(cfd, 200, "ok", getFileType(file), st.st_size);
		sendFile(cfd, file);
	}

	return 0;
}

int sendHeadMsg(int cfd, int status, const char* descr, const char* type, int length)
{
	char buf[4096] = "";
	//memcpy(buf, "http/1.1 200 ok\r\n", 18);
	sprintf(buf, "http/1.1 %d %s\r\n", status, descr);
	sprintf(buf + strlen(buf), "Content-Type: %s\r\n", type);
	sprintf(buf + strlen(buf), "Content-Length: %d\r\n\r\n", length);

	send(cfd, buf, strlen(buf), 0);
	return 0;
}

int sendFile(int cfd, char* fileName)
{
	int fd = open(fileName, O_RDONLY);
	while (1)
	{
		char buf[1024] = "";
		int num = read(fd, buf, sizeof(buf));
		if (num > 0)
		{
			send(cfd, buf, num, 0);
		}
		else if(num == 0)
		{
			break;
		}
		else
		{
			perror("read error");
			return -1;
		}
	}
	return 0;
}

/*
	int scandir(const char* dirp, struct dirent*** namelist,
	int (*filter)(const struct dirent*),
	int (*compar)(const struct dirent**, const struct dirent**))
*/
int sendDir(int cfd, char* dirName)
{
	struct dirent** namelist;
	int num = scandir(dirName, &namelist, NULL, alphasort);
	char buf[1024] = "";
	sprintf(buf,
		"<html><head><title>%s</title></head><body><table><tr><td>%s</td><td>%s</td>", dirName, "�ļ���", "�ļ���С");
	for (int i = 0; i < num; ++i)
	{
		char filepath[1024] = "";
		sprintf(filepath, "%s/%s", dirName, namelist[i]->d_name);
		struct stat st;
		stat(filepath, &st);
		if (S_ISDIR(st.st_mode))
		{
			sprintf(buf + strlen(buf), "<tr><td><a href=\"%s/\">%s</td><td>%ld</td></tr>", namelist[i]->d_name, namelist[i]->d_name, (long)st.st_size);
		}
		else
		{
			sprintf(buf + strlen(buf), "<tr><td><a href=\"%s\">%s</td><td>%ld</td></tr>", namelist[i]->d_name, namelist[i]->d_name, (long)st.st_size);
		}

		send(cfd, buf, strlen(buf), 0);
		memset(buf, 0, sizeof(buf));
		free(namelist[i]);
	}

	sprintf(buf, "</table></body></html>");
	send(cfd, buf, strlen(buf), 0);
	return 0;
}

int disConnect(int efd, int cfd)
{
	int ret = epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
	if (ret == -1)
	{
		perror("epoll_ctl");
		return -1;
	}
	close(cfd);
	return 0;
}

char* getFileType(const char* fileName)
{
	const char* dot = strrchr(fileName, '.');
	if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
		return "text/html; charset=utf-8";
	if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
		return "image/jpeg";
	if (strcmp(dot, ".gif") == 0)
		return "image/gif";
	if (strcmp(dot, ".png") == 0)
		return "image/png";
	if (strcmp(dot, ".css") == 0)
		return "text/css";
	if (strcmp(dot, ".au") == 0)
		return "audio/basic";
	if (strcmp(dot, ".wav") == 0)
		return "audio/wav";
	if (strcmp(dot, ".avi") == 0)
		return "video/x-msvideo";
	if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
		return "video/quicktime";
	if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
		return "video/mpeg";
	if (strcmp(dot, ".vrml") == 0 || strcmp(dot, ".wrl") == 0)
		return "model/vrml";
	if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
		return "audio/midi";
	if (strcmp(dot, ".mp3") == 0)
		return "audio/mpeg";
	if (strcmp(dot, ".ogg") == 0)
		return "application/ogg";
	if (strcmp(dot, ".pac") == 0)
		return "application/x-ns-proxy-autoconfig";

	return "text/plain; charset=utf-8";
}
