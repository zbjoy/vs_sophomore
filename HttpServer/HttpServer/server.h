#ifndef SERVER_H
#define SERVER_H

int initListenFd(unsigned short port);

int epoll_run(unsigned short port);

int acceptConn(int lfd, int efd);

int recvHttpRequest(int cfd);

int parseRequestLine(char* reqLine, int cfd);

int sendHeadMsg(int cfd);

int sendFile(int cfd, char* file);

#endif