#ifndef SERVER_H
#define SERVER_H

int initListenFd(unsigned short port);

int epoll_run(unsigned short port);

int acceptConn(int lfd, int efd);

int recvHttpRequest(int cfd);

int parseRequestLine(char* reqLine, int cfd);

int sendHeadMsg(int cfd, int status, const char* descr, const char* type, int length);

int sendFile(int cfd, char* file);

int sendDir(int cfd, char* file);

int disConnect(int efd, int cfd);

char* getFileType(const char* fileName);

#endif